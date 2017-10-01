#ifndef __UCYCM_READ_H__
#define __UCYCM_READ_H__
__BEGIN_DECLS



struct ucycm_read_segment_info {
        void*                   addr;
        size_t                  size;
#if 0
        struct timeval          tv_real;
        struct timeval          tv_mono;
        unsigned long           sn_min;
        unsigned long           sn_max;
#endif
};
struct ucycm_read;
struct ucycm_read* ucycmr_create(void* addr, key_t key, size_t size_total, 
        int always_read_new);


#define UCYCMR_ERROR_NOT_READY      (-1)
#define UCYCMR_ERROR_WRITER         (-2)
#define UCYCMR_ERROR_SYSTEM         (-3)
#define UCYCMR_ERROR_TIMEOUT        (-4)
#define UCYCMR_ERROR_CHECKSUM       (-5)
int ucycmr_get(struct ucycm_read* cmr, 
        const struct timeval* to, 
        unsigned long* sn, 
        struct ucycm_read_segment_info* info, 
        void* addtional_info);


int ucycmr_info(struct ucycm_read* cmr, 
        unsigned long sn, 
        struct ucycm_read_segment_info* info, 
        void* addtional_info);


int ucycmr_destroy(struct ucycm_read* cmr);




__END_DECLS
#endif /* ucycm_read.h */
