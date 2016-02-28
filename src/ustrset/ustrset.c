#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ustrset.h"


struct ustrset
{
    void*               buf;
    size_t              size_buf;
    size_t              max_buf;

    struct ustrseg*     segs;
    int                 num_segs;
    int                 max_segs;

    int                 num;
    int                 index;
};


/* not implement buffer and segs expand function. */
struct ustrset* ustrset_create(int num, int max_num, size_t size_buf, size_t max_buf)
{
    struct ustrset* ss = um_malloc(sizeof(struct ustrset));
    if(NULL == ss) {
        goto error;
    }
    memset(ss, 0, sizeof(struct ustrset));

    ss->size_buf    = size_buf;
    ss->max_buf     = max_buf;
    ss->num_segs    = num;
    ss->max_segs    = max_num;

    ss->buf         = um_malloc(ss->size_buf);
    if(NULL == ss->buf) {
        goto error;
    }

    ss->segs        = um_malloc(ss->num_segs * sizeof(struct ustrseg));
    if(NULL == ss->segs) {
        goto error;
    }

    ss->num = 0;
    ss->index = 0;

    goto finish;

error:
    if(NULL != ss && NULL != ss->buf) {
        um_free(ss->buf);
        ss->buf = NULL;
    }

    if(NULL != ss && NULL != ss->segs) {
        um_free(ss->segs);
        ss->segs = NULL;
    }

    if(NULL != ss) {
        um_free(ss);
        ss = NULL;
    }

    return NULL;

finish:
    return ss;
}


int ustrset_copy(struct ustrset* ss, const void* buf, size_t size)
{
    int ret;
    ret = 0;

    if(ss->num >= ss->num_segs) {
        ulogerr("ustrset : segs full, copy failed\n");
        ret = -1;
        return ret;
    }

    ss->segs[ss->num].buf = buf;
    ss->segs[ss->num].size = size;
    ss->num ++ ;

    return ret;
}


int ustrset_dup(struct ustrset* ss, const void* buf, size_t size)
{
    int ret;
    ret = 0;

    if(ss->num >= ss->num_segs) {
        ulogerr("ustrset : segs full, copy failed\n");
        ret = -1;
        return ret;
    }

    if((ss->size_buf - ss->index) < size) {
        ulogerr("ustrset : internal buffer full, copy failed\n");
        ret = -1;
        return ret;
    }

    memcpy(ss->buf+ss->index, buf, size);

    ss->segs[ss->num].buf = ss->buf+ss->index;
    ss->segs[ss->num].size = size;
    ss->num ++ ;
    ss->index += size;

    return ret;
}


int ustrset_clear(struct ustrset* ss)
{
    int ret;
    ret = 0;

    ss->num = 0;
    ss->index = 0;

    return ret;
}


int ustrset_destroy(struct ustrset* ss)
{
    int ret;
    ret = 0;

    if(NULL != ss && NULL != ss->buf) {
        um_free(ss->buf);
        ss->buf = NULL;
    }

    if(NULL != ss && NULL != ss->segs) {
        um_free(ss->segs);
        ss->segs = NULL;
    }

    if(NULL != ss) {
        um_free(ss);
        ss = NULL;
    }

    ss = NULL;

    return ret;
}


size_t ustrset_len(const struct ustrset* ss)
{
    size_t size_total = 0;
    int i;
    for(i=0; i<ss->num; i++)
    {
        size_total += ss->segs[i].size;
    }

    return size_total;
}


int ustrset_combine(void* dest, size_t size_dest, size_t* size_copy, 
        const struct ustrset* ss)
{
    int ret;
    ret = 0;

    size_t size_ss = ustrset_len(ss);
    if(size_ss > size_dest) {
        ulogerr("dest string not enough.\n");
        ret = -1;
        return ret;
    }

    int i = 0;
    int index_dest = 0;
    for(i=0; i<ss->num; i++)
    {
        memcpy(dest+index_dest, ss->segs[i].buf, ss->segs[i].size);
        index_dest += ss->segs[i].size ;
    }

    if(NULL != size_copy) {
        *size_copy = size_ss;
    }

    return ret;
}


int ustrset_link(struct ustrset* dest, const struct ustrset* src)
{
    int ret;
    ret = 0;

    int i;
    for(i=0; i<src->num; i++)
    { 
        if(dest->num >= dest->num_segs) {
            ulogerr("ustrset : segs full, copy failed\n");
            ret = -1;
            return ret;
        }

        dest->segs[dest->num + i].buf = src->segs[i].buf;
        dest->segs[dest->num + i].size = src->segs[i].size;
    }

    dest->num += src->num ;

    return ret;
}


int ustrset_num(const struct ustrset* ss)
{
    int ret_num;
    ret_num = ss->num;

    return ret_num;
}


struct ustrseg* ustrset_getseg(const struct ustrset* ss, int idx)
{
    struct ustrseg* ret_seg;

    if(idx>=0 && idx < ss->num) {
        ret_seg = &ss->segs[idx] ; 
    }
    else {
        ulogerr("ustrset_getseg. idx invalid.\n");
        ret_seg = NULL;
    }

    return ret_seg;
}


int ustrset_fwrite(const struct ustrset* ss, FILE* fp)
{
    int ret_nwrite;
    ret_nwrite = 0;

    if(NULL == fp) {
        ulogerr("ustrset_fwrite. fwrite return faild.\n");
        ret_nwrite = -1;
        return ret_nwrite;
    }

    int i = 0;
    int n;
    for(i=0; i<ss->num; i++)
    {
        n = fwrite(ss->segs[i].buf, 1, ss->segs[i].size, fp);
        if(n <= 0 || n != ss->segs[i].size) {
            ulogerr("ustrset_fwrite. fwrite return faild.\n");
            break;
        }
        fflush(fp);
        ret_nwrite += n;
    }

    return ret_nwrite;
}
