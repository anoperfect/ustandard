#define ucycm_size_reserve   108
#define ucycm_size_sn        sizeof(long)
#define ucycm_size_compete   16
#define ucycm_size_param     128
#define ucycm_size_private   128

#define __offset_area_sn        (ucycm_size_reserve)
#define __offset_area_compete   (__offset_area_sn + ucycm_size_sn)
#define __offset_area_param     (__offset_area_compete + ucycm_size_compete)
#define __offset_area_private   (__offset_area_param + ucycm_size_param)
#define __offset_area_infos     (__offset_area_private + ucycm_size_private)




#define LEN_NAME    60
#define __align(x)  (((x)+3) &~(3))
#define LEN_CHECKSUM    16

struct ucycm_param {
    key_t                   key; 
    size_t                  size_total;

    void*                   addr;

    long                    n_data_info;
    size_t                  size_additional_info;
    size_t                  size_additional_info_store;
    char                    name[LEN_NAME];
    int                     enable_checksum;

    struct timeval          to_max;     /* if get timeout is NULL, set to this. */
    useconds_t              usec_sleep; /* usleep time while waiting. */
    unsigned long           nmax;       /* nsegment aviliable. */
}cm_param;


struct ucycm_segment {
    unsigned long           offset;
    size_t                  size;
    char                    addtional_data[0];
};


#define __offset_area_data(n, size)  \
(__offset_area_infos + (n)*(sizeof(struct ucycm_segment)+__align((size))))


#define __get_segment_info(cm, sn)              \
((cm)->segments_info + (((sn)-1) % (cm)->cm_param.n_data_info))


#define __get_shared_segment_info(addr, nsegments, size_addi, sn)                   \
            (addr) + __offset_area_infos +                                          \
            (sizeof(struct ucycm_segment)+__align(size_addi)) *                     \
            (((sn)-1) % (nsegments));


static int _uchar_array_check(void* addr, int c, size_t size)
{
    int ret = 0;
    int i;
    for(i=0; i<size; i++) {
        if(((unsigned char*)addr)[i] != c) {
            ulogdbg("addr = %p,v = %d,c=%d.\n", addr, ((unsigned char*)addr)[i], c);
            ret = -1;
            break;
        }
    }

    return ret;
}


static int _ucycm_check_const(void* addr, struct ucycm_param* param)
{
    int ret = 0;

    #if UCYCM_NO_LOG
    return ret;
    #endif//UCYCM_NO_LOG

    ret = _uchar_array_check(addr, '@', ucycm_size_reserve);
    if(0 != ret) {
        ulogerr("_ucycmr_check_const. reserve.\n");
        ret = -1;
        goto finished;
    }

    ret = _uchar_array_check(addr + __offset_area_compete, 'c', ucycm_size_compete);
    if(0 != ret) {
        ulogerr("_ucycmr_check_const. compete.\n");
        ret = -1;
        goto finished;
    }

    if(ucycm_size_param > sizeof(struct ucycm_param)) {
        ret = _uchar_array_check(addr + __offset_area_param + sizeof(struct ucycm_param), 'u', ucycm_size_param > sizeof(struct ucycm_param));
        if(0 != ret) {
            ulogerr("_ucycmr_check_const. param.\n");
            ret = -1;
            goto finished;
        }
    }
    
    ret = _uchar_array_check(addr + __offset_area_private, 'p', ucycm_size_private);
    if(0 != ret) {
        ulogerr("_ucycmr_check_const. private.\n");
        ret = -1;
        goto finished;
    }

    size_t size = param->size_additional_info_store - param->size_additional_info;
    ulognon("size_check = %u.\n", size);
    if(size > 0) {
        int i;
        for(i=0; i<param->n_data_info; i++) {
            struct ucycm_segment* shared_info = __get_shared_segment_info(addr, param->n_data_info, param->size_additional_info, i);
            ulognon("shared_info=%p.\n", shared_info);
            //ret = _uchar_array_check((void*)(shared_info + 1) - size, 's', size);
            ret = _uchar_array_check((void*)(shared_info)+ (sizeof(struct ucycm_segment)+(param->size_additional_info)), 's', size);
                
            if(0 != ret) {
                ulogerr("_ucycmr_check_const. segment info.\n");
                ret = -1;
                goto finished;
            }
            else {
                ulognon("_ucycmr_check_const. OK segment info[%d].\n", i);
            }
        }
    }

finished:
    return ret;
}





