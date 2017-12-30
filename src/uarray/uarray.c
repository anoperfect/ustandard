#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ustring.h"
#include "ustandard/uarray.h"

struct uarray {
    int total;      /* current total number. */
    int count;      /* count. */
    int limit;      /* expand limit. 0 mean no limit.*/


    updescription des;

    size_t size_entry;
    void *p;

    size_t num_init;
    size_t num_expand;
};


#define UARRAY_NUMBER_INIT     16
#define UARRAY_NUMBER_EXPAND   16
struct uarray* _uarray_create(size_t size_entry, int limit, int num_init, int num_expand)
{
    struct uarray* array = um_malloc(sizeof(*array));
    if(array) {
        array->size_entry = size_entry;
        array->des = NULL;

        array->count = 0;

        array->limit = limit;
        array->num_init     = 0;
        array->num_expand   = num_expand;
        
        if(num_init > 0) {
            num_init = limit>0?um_min(num_init, limit):num_init;
            array->num_init = num_init;
            array->total = array->num_init;
            array->p = um_malloc(array->total * array->size_entry);
            if(!array->p) {
                um_free(array);
                array = NULL;
            }
        }
    }

    return array;
}

struct uarray* uarray_create(size_t size_entry)
{
    return _uarray_create(size_entry, 0, UARRAY_NUMBER_INIT, UARRAY_NUMBER_EXPAND);
}


struct uarray* uarray_create_detail(size_t size_entry, int limit, int num_init, int num_expand, updescription des)
{
    struct uarray* array = _uarray_create(size_entry, limit, num_init, num_expand);
    if(array) {
        array->des = des;
    }

    return array;
}


void uarray_destroy(struct uarray* array)
{
    if(array) {
        um_free_check(array->p);
        um_free_check(array);
    }
}


int uarray_count(struct uarray* array)
{
    return array->count;
}


int _uarray_expand(struct uarray* array)
{
    int ret = 0;

    /* allow to expand. */
    if(0 == array->limit || array->total < array->limit) {
        int num_expand = array->num_expand;
        if(0 != array->limit) {
            num_expand = um_min(num_expand, array->limit - array->total);
        }

        void* tmp = um_realloc(array->p, array->size_entry * (array->total + num_expand));
        if(tmp) {
            array->total += num_expand;
            array->p = tmp;
        }
    }
    else {
        ulogerr("uarray expand failed, array full.\n");
        ret = -1;
    }

    return ret;
}


/*
    idx : -1 or >=count would be add to tailer. 
 */
int uarray_add_at(struct uarray* array, int at, void* p)
{
    int ret = 0;
    if(array->count >= array->total) {
        ret = _uarray_expand(array);
        if(-1 == ret) {
            ulogerr("uarray full.\n");
            return ret;
        }
    }

    /* just add at tailer, or need to move the entry after idx. */
    if(-1 == at || at >= array->count) {
        at = array->count;
    }
    else {
        /* move the entry after idx. */
        int idx;
        for(idx=array->count-1; idx>=at; idx--) {
            memcpy(array->p + array->size_entry * (idx+1), array->p + array->size_entry * idx, array->size_entry);
        }
    }

    memcpy(array->p + array->size_entry * at, p, array->size_entry);
    array->count ++;

    return ret;
}


/*
    idx : -1 or >=count would be add to tailer. 
 */
int uarray_delete_at(struct uarray* array, int at)
{
    int ret = 0;

    if(at >= 0 && at<array->count) {
        int idx;
        for(idx=at+1; idx<array->count; idx++) {
            memcpy(array->p + array->size_entry * idx, array->p + array->size_entry * (idx+1), array->size_entry);
        }

        array->count --;
    }

    return ret;
}


int uarray_clear(struct uarray* array, int count, int total)
{
    int ret = 0;

    if(count >= 0 && count <= array->count) {
        array->count = count;
    }

    if(total != -1
        && total < array->total
        && total >= array->count) {
        void* tmp = um_realloc(array->p, array->size_entry * total);
        if(tmp) {
            array->total = total;
        }
    }

    return ret;
}


void* uarray_at(struct uarray* array, int at)
{
    void* retp = NULL;

    if(at >= 0 && at < array->count) {
        retp = array->p + array->size_entry * at;
    }

    return retp;
}


int uarray_value(struct uarray* array, int at, void* p)
{
    int ret = 0;

    if(at >= 0 && at < array->count) {
        memcpy(p, array->p + array->size_entry * at, array->size_entry);
    }

    return ret;
}


int uarray_description(struct uarray* array, const char* title, char* dest, size_t size)
{
    int ret = 0;

    snprintf(dest, size, "count:%d, total:%d.\n", array->count, array->total);
    int idx;
    for(idx=0; idx<array->count; idx++) {
        char tmp[1024];
        array->des(tmp, 1024, array->p + array->size_entry * idx);
        ustrcat_format(dest, size, "%2d : %s\n", idx, tmp);
    }

    return ret;
}




