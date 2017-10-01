#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ubuffer_rw.h"


struct ubuffer_rw {
    size_t          size_max;
    expand_type_e   type;  
    expand_arg      arg;

    void*   ptr;
    size_t  total;
    size_t  size;
    size_t  idx;
};


struct ubuffer_rw* ubuffer_rw_create(size_t size_min, size_t size_max, 
    expand_type_e type, expand_arg arg)
{
    struct ubuffer_rw* buffer_rw = NULL;
    void* ptr = NULL;

    if(0 != size_min) {
        ptr = um_malloc(size_min);
        if(NULL == ptr) {
            goto cleanup;
        }
    }
    
    buffer_rw = um_malloc(sizeof(*buffer_rw));
    if(NULL == buffer_rw) {
        goto cleanup;
    }

    buffer_rw->size_max = size_max;
    buffer_rw->type     = type;
    buffer_rw->arg      = arg;
    
    buffer_rw->ptr      = ptr;
    buffer_rw->total    = size_min;
    buffer_rw->size     = 0;
    buffer_rw->idx      = 0;

    return buffer_rw;

cleanup:
    if(NULL != ptr) {
        um_free(ptr);
        ptr = NULL;
    }

    if(NULL != buffer_rw) {
        um_free(buffer_rw);
        buffer_rw = NULL;
    }

    buffer_rw = NULL;
    return buffer_rw;
}
        

int ubuffer_rw_destroy(struct ubuffer_rw* buffer_rw)
{
    int ret = 0;
    if(NULL != buffer_rw) {
        if(NULL != buffer_rw->ptr) {
            um_free(buffer_rw->ptr); 
            buffer_rw->ptr = NULL;
        }

        um_free(buffer_rw);
        buffer_rw = NULL;
    }
    else {
        ulogerr("destroy NULL data.\n");
    }

    return ret;
}


size_t ubuffer_rw_size(struct ubuffer_rw* buffer_rw)
{
    size_t size;
    size = buffer_rw->size - buffer_rw->idx;
    return size;
}


unsigned int to_2power(unsigned int n)
{
    int ret;
    ret = 1;

    while(ret < n) {
        ret = ret*2 ;
    }

    return ret;
}


int _ubuffer_rw_expand(struct ubuffer_rw* buffer_rw, size_t size_need)
{
    int ret = 0;

    size_t size_realloc = (buffer_rw->size-buffer_rw->idx) + size_need;
    int n;
    switch(buffer_rw->type) {
        case expand_type_equal:
        break;

        case expand_type_2power:
        size_realloc = to_2power(size_realloc);
        break;

        case expand_type_percent:
        size_realloc += size_realloc * buffer_rw->arg.percent / 100;
        break;

        case expand_type_const:
        size_realloc += buffer_rw->arg.size_const;
        break;

        case expand_type_add_step:
        n = 1;
        while((buffer_rw->total + n*buffer_rw->arg.size_step)<size_realloc) {
            n ++;
        }
        size_realloc = buffer_rw->total + n*buffer_rw->arg.size_step;
        break;

        case expand_type_define:
        size_realloc = buffer_rw->arg.expand(
                buffer_rw->total, size_realloc, buffer_rw->size_max);
        break;

        default:
        break;
    }

    if(size_realloc >= buffer_rw->size_max) {
        size_realloc = buffer_rw->size_max;
    }

    if(size_realloc > buffer_rw->total) {
        void* ptr = um_realloc(buffer_rw->ptr, size_realloc);
        if(NULL != ptr) {
            buffer_rw->ptr      = ptr;
            buffer_rw->total    = size_realloc;
        }
        else {
            ulogerr("%s realloc error(%zd).\n", __FUNCTION__, size_realloc);
            ret = -1;
        }
    }
    ulogerr("expand to %zd.\n", buffer_rw->total);

    return ret;
}


size_t ubuffer_rw_write(struct ubuffer_rw* buffer_rw, const void* ptr, size_t size)
{
    size_t retsize = 0;

    size_t size_buffer = buffer_rw->size - buffer_rw->idx;
    if(0 != buffer_rw->idx) {
        memcpy(buffer_rw->ptr, buffer_rw->ptr+buffer_rw->idx, size_buffer);
        buffer_rw->idx = 0;
        buffer_rw->size = size_buffer;
    }

    if((buffer_rw->total - buffer_rw->size) < size) {
        _ubuffer_rw_expand(buffer_rw, size);
    }

    retsize = um_min((buffer_rw->total - buffer_rw->size), size);
    memcpy(buffer_rw->ptr + buffer_rw->size, ptr, retsize);
    buffer_rw->size += retsize;

    return retsize;
}


size_t ubuffer_rw_read(struct ubuffer_rw* buffer_rw, void* ptr, size_t size)
{
    size_t retsize = 0;

    size_t size_buffer = buffer_rw->size - buffer_rw->idx;
    if(0 == size_buffer) {

    }
    else if(size_buffer >= size) {
        retsize = size;
        memcpy(ptr, buffer_rw->ptr+buffer_rw->idx, retsize);
        buffer_rw->idx += retsize;
    }
    else {
        retsize = size_buffer;
        memcpy(ptr, buffer_rw->ptr+buffer_rw->idx, retsize);
        buffer_rw->idx  = 0;
        buffer_rw->size = 0;
    }

    return retsize;
}
