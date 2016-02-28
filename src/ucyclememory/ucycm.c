#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ustrset.h"
#include "ustandard/umd5.h"
#include "ustandard/ucycm.h"
#include "ucycm_public.h"



/*------ data structer. 
    |-reserve-|-sn-|-compete-|-param-|-private-|-n*(size_segment_info+additional)-|-data-|
------*/
struct ucycm {
    struct ucycm_param cm_param;

    unsigned long               sn;
    void*                       addr;
    key_t                       key;
    int                         shm_id;

    unsigned long               offset_data_area;
    unsigned long               size_data_area;
    unsigned long               offset_add_data;
    unsigned long               sn_cycle;

    struct ucycm_segment*       segments_info;
};


static void* _shm_create(key_t key, size_t size, int* pshm_id);
static int _shm_nattch(int shm_id);
static int _copy_data(void* dest, size_t size_dest, 
        const struct ustrset* ss, const void* addr, size_t size, 
        int enable_checksum);





struct ucycm* ucycm_create(void* addr, key_t key, size_t size_total, 
        const char* name, 
        long n_data_info, size_t size_additional_info, 
        int enable_checksum,
        const struct ucycm_param_read* param_read)
{
    struct ucycm* cm = NULL;

    int size_additional_info_store = __align(size_additional_info);
    size_t offset_data_area = __offset_area_data(n_data_info, size_additional_info_store);
    ulogdbg("offset_data_area(%u) = %u + %lu * (%u + %u).\n", 
            offset_data_area, __offset_area_infos, n_data_info, sizeof(struct ucycm_segment),size_additional_info_store);
    if(size_total <= offset_data_area) {
        ulogerr("total size not enough. at lease %u.\n", offset_data_area);
        return cm;
    }

    cm = um_malloc(sizeof(struct ucycm) + n_data_info*sizeof(struct ucycm_segment));
    if(NULL == cm) {
        return cm;
    }

    memset(cm, 0, sizeof(*cm));    

    cm->segments_info = (void*)cm + sizeof(struct ucycm);
    
    cm->shm_id = -1;
    if(NULL == addr) {
        cm->cm_param.key                     = key;
        cm->cm_param.addr                    = NULL;

        ulogdbg("create shared memory on key : 0x%x.\n", key);
        cm->key = key;
        addr = _shm_create(key, size_total, &cm->shm_id);
        if(NULL == addr) {
            um_free(cm);
            cm = NULL;
            return cm;
        }
    }
    cm->cm_param.size_total = size_total;

    /* copy the create param. */
    ulogdbg("sizeof(cm_param) = %u.\n", sizeof(cm->cm_param));
    cm->cm_param.n_data_info                = n_data_info;
    cm->cm_param.size_additional_info       = size_additional_info;
    cm->cm_param.size_additional_info_store = size_additional_info_store;
    um_strcpy(cm->cm_param.name, LEN_NAME, name);
    cm->cm_param.enable_checksum            = enable_checksum?1:0;
    if(NULL != param_read) {
        cm->cm_param.to_max     = param_read->to_max;
        cm->cm_param.usec_sleep = param_read->usec_sleep;
        cm->cm_param.nmax       = param_read->nmax;
    }
    else {
        cm->cm_param.to_max.tv_sec  = 1;
        cm->cm_param.to_max.tv_usec = 0;
        cm->cm_param.usec_sleep = 5000;
        cm->cm_param.nmax       = 30;
    }

    cm->offset_data_area                = offset_data_area;
    cm->size_data_area                  = size_total - offset_data_area;
    cm->offset_add_data                     = 0;
    ulogdbg("offset_data_area = %lu,size_data_area = %lu,offset_add_data = %lu.\n", 
            cm->offset_data_area, cm->size_data_area, cm->offset_add_data);

    cm->addr = addr;
    cm->sn = 0;

    /* update memory. 
       some unused area filled with barrier data, it could be used for test. */ 
    memset(cm->addr, '@', ucycm_size_reserve);
    memcpy(cm->addr + __offset_area_sn, &cm->sn, sizeof(cm->sn));
    memset(cm->addr + __offset_area_compete, 'c', ucycm_size_compete);
    memcpy(cm->addr + __offset_area_param, &cm->cm_param, sizeof(struct ucycm_param));
    if(ucycm_size_param > sizeof(struct ucycm_param)) {
        memset(cm->addr + __offset_area_param + sizeof(struct ucycm_param), 'u', 
                ucycm_size_param > sizeof(struct ucycm_param));
    }
    memset(cm->addr + __offset_area_private, 'p', ucycm_size_private);
    memset(cm->addr + __offset_area_infos, 's', 
        n_data_info * (sizeof(struct ucycm_segment) + size_additional_info_store));
    memset(cm->addr + offset_data_area, 255, cm->size_data_area);

    ulogdbg("check const[%d].\n", __LINE__);
    assert(0 == _ucycm_check_const(cm->addr, &cm->cm_param));
    ulogdbg("check const OK.\n");

    return cm;
}


struct ucycm_dat1a {
    void* addr;
    unsigned long size;
    struct ustrset* ss;

    void* additional;
};


int ucycm_add(struct ucycm* cm, 
        const struct ustrset* ss, const void* addr, size_t size,
        const void* additional)
{
    int ret = 0;

    if(!((NULL != addr && size > 0) || NULL != ss )) {
        ret = -1;
        return ret;
    }

    if(NULL != ss) {
        size = ustrset_len(ss);
    }

    size_t size_checksum = cm->cm_param.enable_checksum?LEN_CHECKSUM:0;

    int cycled = 0;

    if(cm->size_data_area < size+size_checksum) {
        ulogerr("not enough space.\n");
        ret = -1;
        return ret;
    }

    cm->sn ++;

    ulogdbg("ucycm_add : [%lu] %u.\n", cm->sn, size);
    ulogdbg("before added : size_data_area = %lu,offset_add_data = %lu.\n", 
            cm->size_data_area, cm->offset_add_data);

    ulogdbg("check const[%d].\n", __LINE__);
    assert(0 == _ucycm_check_const(cm->addr, &cm->cm_param));
    ulogdbg("check const OK.\n");

    struct ucycm_segment* s_info = __get_segment_info(cm, cm->sn);

    s_info->size = size;

    #define UCYCM_COUNT_SN_MIN  0
    if(1 == cm->sn) {
        cm->offset_add_data = 0;
        cm->sn_cycle = cm->sn;
    }
    else { 
        struct ucycm_segment* s_info_prev; 
        s_info_prev = __get_segment_info(cm, cm->sn-1);
        if((cm->size_data_area - cm->offset_add_data) >= (size+size_checksum)) {
            #if UCYCM_COUNT_SN_MIN
            int i;
            struct ucycm_segment* s_info_i; 
            for(i=s_info_prev->sn_min; i<cm->sn_cycle; i++) {
                s_info_i = __get_segment_info(cm, i);
                ulognon("offset of %d is %lu.\n", i, s_info_i->offset);
                if(s_info_i->offset >= (cm->offset_add_data + size+size_checksum)) {
                    ulogdbg("break when i = %d.\n", i);
                    break;
                }
            }
            s_info->sn_min = i; 
            #endif
        }
        else {
            #if UCYCM_COUNT_SN_MIN
            for(i=s_info_prev->sn_max; i>=s_info_prev->sn_min; i--) {
                s_info_i = __get_segment_info(cm, i);
                ulognon("offset of %d is %lu.\n", i, s_info_i->offset);
                if(s_info_i->offset < (size+size_checksum)) {
                    ulogdbg("break when i = %d.\n", i);
                    break;
                }
            }
            s_info->sn_min = i+1;
            #endif

            cm->offset_add_data = 0;
            cycled = 1;
            cm->sn_cycle = cm->sn;
        }
    }

    #if UCYCM_COUNT_SN_MIN
    /* aviliable sn_min should be suit to the n_data_info . */
    if((s_info->sn_max - s_info->sn_min) >= cm->cm_param.n_data_info) {
        s_info->sn_min = s_info->sn_max - cm->cm_param.n_data_info + 1;
    }
    #endif

    s_info->offset = cm->offset_add_data;

    /* copy data. */
    _copy_data(cm->addr + cm->offset_data_area + cm->offset_add_data, size, 
            ss, addr, size, cm->cm_param.enable_checksum);

    /* update segment info. */
    struct ucycm_segment* shared_s_info 
            = __get_shared_segment_info(cm->addr, cm->cm_param.n_data_info, 
                    cm->cm_param.size_additional_info, cm->sn);
    memcpy(shared_s_info, s_info, sizeof(*shared_s_info));
    memcpy(shared_s_info->addtional_data, additional, 
            cm->cm_param.size_additional_info);

    /* update sn. */
    memcpy(cm->addr + __offset_area_sn, &cm->sn, sizeof(cm->sn));

    cm->offset_add_data += (size+size_checksum);

    ulogdbg("check const[%d].\n", __LINE__);
    assert(0 == _ucycm_check_const(cm->addr, &cm->cm_param));
    ulogdbg("check const OK.\n");

    ulogdbg("after added : key=%x.\n", cm->key);
    ulogdbg("after added : offset = %lu, offset_add_data = %lu%s.\n",
            s_info->offset, 
            cm->offset_add_data, cycled?"(cycled)":"");

    #if UCYCM_COUNT_SN_MIN
    ulogdbg("after added : [%lu, %lu].\n",
            s_info->sn_min, s_info->sn_max);
    #endif

    return ret;
}


int ucycm_nattch(struct ucycm* cm)
{
    int n = 0;

    if(-1 != cm->shm_id) {
        n = _shm_nattch(cm->shm_id);
    }

    return n;
}


int ucycm_destroy(struct ucycm* cm)
{
    int ret = 0;
    if(NULL == cm) {
        return ret;
    }

    if(-1 != cm->shm_id) {
        shmdt(cm->addr);
        cm->addr = NULL;
        shmctl(cm->shm_id, IPC_RMID, 0);
        cm->shm_id = -1;
    }

    um_free(cm);
    cm = NULL;

    return ret;
}


void* _shm_create(key_t key, size_t size, int* pshm_id)
{
    void* addr = NULL;
    int shm_id;

    ulogdbg("key=0x%x, size=%d.\n", key, size);
    shm_id=shmget(key, size, 0600 | IPC_CREAT);

    if(shm_id==-1) {
        int eno = errno;
        ulogerr("shmget error. (shmget return shm_id = %d <%s>).\n", shm_id, strerror(eno));
        *pshm_id = -1;
        return NULL;
    }

    addr = shmat(shm_id, NULL, 0);
    if(NULL == addr) {
        int eno = errno;
        ulogerr("shmat error. (shmat return NULL. <%s>).\n", strerror(eno));
        *pshm_id = -1;
        return NULL;
    }

    *pshm_id = shm_id;
    ulogdbg("shm_id = %d, addr = [%p,%p).\n", shm_id, addr, (void*)(addr+size));
    return addr;
}


int _shm_nattch(int shm_id)
{
    int ret = 0; 

    struct shmid_ds buf;
    ret = shmctl(shm_id, IPC_STAT, &buf);
    if(0 != ret) {
        ulogerr("shmctl IPC_STAT on (%d) error.\n", shm_id);
        ret = -1;
    }
    else {
        ret = buf.shm_nattch;
    }

    return ret;
}


int _copy_data(void* dest, size_t size_dest, 
        const struct ustrset* ss, const void* addr, size_t size, 
        int enable_checksum)
{
    int ret = 0;  

    ulogdbg("_copy_data start. to %p with %u.\n", dest, size);
    if(NULL != ss) {
        size_t size_copy;
        int ret = ustrset_combine(dest, size_dest, &size_copy, ss);
        assert(0 == ret && size_copy == size);
    }
    else {
        memcpy(dest, addr, size);
    }

    if(enable_checksum) {
        umd5_calc(dest+size, dest, size);
    }
    ulogdbg("_copy_data finish.\n");

    return ret;
}
