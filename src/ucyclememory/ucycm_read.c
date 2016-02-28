#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/umd5.h"
#include "ustandard/ucycm_read.h"
#include "ucycm_public.h"
struct ucycm_read {
    /* the writer setting. */
    struct ucycm_param cm_param; 

    /* settings from ucycmr_create. */
    int always_read_new;
    void*                       addr;
    key_t                       key;

    /* keep the shared memory id. */
    int                         shm_id;

    /* current read sn. maintenanced while ucycmr_get calling. */
    unsigned long               sn;
};


static void* _shm_attach(key_t key, size_t size, int* pshm_id);


struct ucycm_read* ucycmr_create(void* addr, key_t key, size_t size_total, 
        int always_read_new)
{
    struct ucycm_read* cmr = NULL;
    cmr = um_malloc(sizeof(*cmr));
    if(NULL == cmr) {
        return cmr;
    }

    if(NULL == addr) {
        ulogdbg("create shared memory on key : 0x%x.\n", key);
        cmr->key = key;
        addr = _shm_attach(key, size_total, &cmr->shm_id);
        if(NULL == addr) {
            ulogerr("shared memory attach error(key=0x%x, size=%u).\n", 
                    key, size_total);
            um_free(cmr);
            cmr = NULL;
            return cmr;
        }
    }
    else {
        cmr->shm_id = -1;
    }

    cmr->addr = addr;
    cmr->sn = 0;

    cmr->always_read_new = always_read_new;

    /* get the param. */
    memcpy(&cmr->cm_param, addr + __offset_area_param, sizeof(cmr->cm_param));
    ulogdbg("key: 0x%x, size: %u, addr:%p, n:%ld, size_addi:%u, size_addi_s:%u, name:%s, enabled_check_sum:%d, to_max:%lu:%06lu, usec_sleep:%u, nmax=%lu.\n", 
            cmr->cm_param.key, 
            cmr->cm_param.size_total, 
            cmr->cm_param.addr, 
            cmr->cm_param.n_data_info, 
            cmr->cm_param.size_additional_info, 
            cmr->cm_param.size_additional_info_store, 
            cmr->cm_param.name, 
            cmr->cm_param.enable_checksum, 
            cmr->cm_param.to_max.tv_sec, 
            cmr->cm_param.to_max.tv_usec, 
            cmr->cm_param.usec_sleep, 
            cmr->cm_param.nmax);

    return cmr;
}


int ucycmr_get(struct ucycm_read* cmr, 
        const struct timeval* to, 
        unsigned long* sn, 
        struct ucycm_read_segment_info* info, 
        void* addtional_info)
{
    int ret = 0;

    assert(0 == _ucycm_check_const(cmr->addr, &cmr->cm_param));

    unsigned long sn_read;
    unsigned long sn_get = 0;

    ulogdbg("cmr->key = 0x%x.\n", cmr->key);
    ulogdbg("cmr->sn = %lu.\n", cmr->sn);

    /* read the sn on writer. */
    memcpy(&sn_read, cmr->addr + __offset_area_sn, sizeof(sn_read));

    /* at the first time, always reture the reading segment. */
    if(0 == cmr->sn) {
        ulogdbg("ucycmr first time read.\n");
        if(0 == sn_read) {
            ulogdbg("ucycmr read sn is 0, no data on ucycm.\n");
            ret = UCYCMR_ERROR_NOT_READY;
        }
        else {
            sn_get = sn_read;
            ret = 0;
        }

        goto finish;
    }

    if(cmr->always_read_new) {
        if(0 == sn_read) {
            ulogdbg("ucycmr read sn is 0.\n");
            ret = UCYCMR_ERROR_NOT_READY;
        }
        else {
            sn_get = sn_read;
            ret = 0;
        }

        goto finish;
    }
        
    long usec_to_max;
    if(NULL != to) {
        usec_to_max = to->tv_sec * 1e6 + to->tv_usec;
    }
    else {
        to = &cmr->cm_param.to_max;
        usec_to_max = to->tv_sec * 1e6 + to->tv_usec;
    }

    while(1) {
        if(sn_read == cmr->sn) {
            ulognon("no new data.\n");
            usleep(cmr->cm_param.usec_sleep);
            usec_to_max -= cmr->cm_param.usec_sleep; 
        }
        else if(sn_read > cmr->sn) {
            ulogdbg("sn_read > cmr->sn\n");
            ulognon("cmr->nmax=%lu.\n", cmr->cm_param.nmax);
            ulognon("sn_read - cmr->sn <= cmr->cm_param.nmax = %d.\n", 
                    sn_read - cmr->sn <= cmr->cm_param.nmax);

            if(((sn_read - cmr->sn) <= cmr->cm_param.nmax)) {
                ulogdbg("(sn_read - cmr->sn <= cmr->cm_param.nmax)\n");
                sn_get = cmr->sn + 1;
            }
            else {
                ulogdbg("ucycmr skip to %lu\n", sn_get);
                sn_get = sn_read;
            }

            break;
        }
        else {
            ulogerr("ucycmr_get. sn error(%lu,%lu).\n", sn_read, cmr->sn);
            ret = UCYCMR_ERROR_WRITER;
            break;
        }

        if(usec_to_max < 0) {
            ret = UCYCMR_ERROR_TIMEOUT;
            break;
        }

        memcpy(&sn_read, cmr->addr + __offset_area_sn, sizeof(sn_read));
        ulogdbg("sn_read = %lu.\n", sn_read);
    }

finish:
    ulogdbg("&&&&&&&&&& sn_get = %lu.\n", sn_get);
    if(0 == ret) {
        *sn = sn_get;
        cmr->sn = sn_get;
        ret = ucycmr_info(cmr, sn_get, info, addtional_info);
        if(0 != ret) {
            ulogerr("checksum error.\n");
            ret = UCYCMR_ERROR_CHECKSUM;
        }
    }

    return ret;
}


int ucycmr_info(struct ucycm_read* cmr, 
        unsigned long sn, 
        struct ucycm_read_segment_info* info, 
        void* addtional_info)
{
    int ret = 0;

    struct ucycm_segment* shared_info = 
    __get_shared_segment_info(
            cmr->addr, 
            cmr->cm_param.n_data_info, 
            cmr->cm_param.size_additional_info, 
            sn);
    if(NULL != info) {
        info->addr      = cmr->addr + 
                __offset_area_data(cmr->cm_param.n_data_info, 
                        cmr->cm_param.size_additional_info) + shared_info->offset;
        info->size      = shared_info->size;
    #if 0
        info->tv_real   = shared_info->tv_real;
        info->tv_mono   = shared_info->tv_mono;
        info->sn_min    = shared_info->sn_min;
        info->sn_max    = shared_info->sn_max;
    #endif

        if(cmr->cm_param.enable_checksum) {
            unsigned char md5sum[LEN_CHECKSUM];
            umd5_calc(md5sum, info->addr, info->size);
            if(0 != memcmp(md5sum, info->addr+info->size, LEN_CHECKSUM)) {
                ulogerr("checksum not equal.\n");
                ret = -1;
            }
        }
    }

    if(NULL != addtional_info) {
        memcpy(addtional_info, shared_info->addtional_data, 
                cmr->cm_param.size_additional_info);
    }

    return ret;
}


int ucycmr_destroy(struct ucycm_read* cmr)
{
    int ret = 0;

    if(NULL == cmr) {
        return ret;
    }

    if(-1 != cmr->shm_id) {
        shmdt(cmr->addr);
        cmr->addr = NULL;
    }
    
    um_free(cmr);
    cmr = NULL;

    return ret;
}



void* _shm_attach(key_t key, size_t size, int* pshm_id)
{
    void* addr = NULL;
    int shm_id;

    ulogdbg("key=0x%x, size=%d.\n", key, size);
    shm_id=shmget(key, size, 0600 | IPC_EXCL );

    if(shm_id==-1)
    {
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
