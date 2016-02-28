#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
struct _ugetidx_easy_data {
    int* v;
    int num;
    int total;
};
static uintptr_t _ugetidx_easy_init(int total)
{
    uintptr_t p = (uintptr_t)0;

    struct _ugetidx_easy_data* sort = um_malloc(sizeof(struct _ugetidx_easy_data) + total*sizeof(int));
    if(NULL != sort) {
        sort->v = (void*)sort + sizeof(struct _ugetidx_easy_data);
        sort->num = 0;
        sort->total = total;
        memset(sort->v, 'x', total*sizeof(int));

        p = (uintptr_t)sort;
    }

    return p;
}


static int _ugetidx_easy(uintptr_t p, int v)
{
    int idx = -1;
    if((uintptr_t)0 != p) {
        struct _ugetidx_easy_data* sort = (struct _ugetidx_easy_data*)p;
        int i;
        for(i=0; i<sort->total; i++) {
            if(v == sort->v[i]) {
                idx = i;
                return idx;
            }
        }

        if(sort->num < sort->total) {
            idx = sort->num;
            sort->num ++ ;
            sort->v[idx] = v;
        }
    }

    return idx;
}


static int _ugetidx_easy_deinit(uintptr_t p)
{
    int ret = 0;
    if((uintptr_t)0 != p) {
        struct _ugetidx_easy_data* sort = (struct _ugetidx_easy_data*)p;
        um_free(sort);
    }
    return ret;
}
