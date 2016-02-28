#ifndef __UCYCSHM_H__
#define __UCYCSHM_H__
#include <stdio.h>
__BEGIN_DECLS
#include "ustandard/ustrset.h"
struct ucycm_param_read {
    struct timeval          to_max;     /* if get timeout is NULL, set to this. */
    useconds_t              usec_sleep; /* usleep time while waiting. */
    unsigned long           nmax;       /* nsegment aviliable. */
};

struct ucycm;



struct ucycm* ucycm_create(void* addr, key_t key, size_t size_total, 
        const char* name, 
        long n_data_info, size_t size_additional_info, 
        int enable_checksum,
        const struct ucycm_param_read* param_read);


int ucycm_add(struct ucycm* cm, 
        const struct ustrset* ss, const void* addr, size_t size,
        const void* additional);


int ucycm_nattch(struct ucycm* cm);


int ucycm_destroy(struct ucycm* cm);


__END_DECLS
#endif//ucycshm.h
