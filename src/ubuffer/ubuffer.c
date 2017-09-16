#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ubuffer.h"
int ubuffer_split_by_char(const void* src, size_t size, 
        int c, 
        struct ubuffer_data* data, 
        int nmax)
{
    int retn = 0;

    const void* src_end = src + size;
    const void* s = src;
    const void* e;
    int n = 0;
    while(s < src_end) {
        e = memchr(s, c, src_end - s);
        if(NULL != e) {
            if(e > s && n<nmax) {
                data[n].ptr = s;
                data[n].size = e - s;
                n ++;
            }
            s = e + 1;
        }
        else {
            if(src_end > s && n<nmax) {
                data[n].ptr = s;
                data[n].size = src_end - s;
                n ++;
            }
            break;
        }
    }
    retn = n;
    return retn;
}


void* ubuf_move_left(void* p, size_t size_src, size_t size_dest)
{
    if(NULL != p && size_src > size_dest) {
        int i;
        size_t offset = size_src - size_dest;
        for(i=0; i<(int)size_dest; i++) {
            ((unsigned char*)p)[i] = ((unsigned char*)p+offset)[i];
        }
    }

    return p;
}


void* ubuf_move_right(void* p, size_t size, size_t n)
{
    if(NULL != p && size > 0 && n > 0) {
        int i;
        for(i=size-1; i>=0; i--) {
            ((unsigned char*)p)[i+n] = ((unsigned char*)p)[i];
        }
    }

    return p;
}
































void _ubufs_add(struct ucbuf* bufs, long total, long* pnumber, struct ucbuf add)
{
    if(!(NULL != add.p && add.size > 0)) {
        return ;
    }

    if(*pnumber < total) {
        bufs[*pnumber] = add;
        (*pnumber) ++;
    }
    else {
        ulogerr("array capacity full.\n");
    }
}


void _ubuf_dup_from_bufs(
    		void** ppdest, size_t* psize, 
            struct ucbuf* bufs, long num)
{
    *psize = 0;

    int i;
    for(i=0; i<num; i++) {
        *psize += bufs[i].size;
    }

    *ppdest = um_malloc(*psize);

    size_t loc = 0;
    for(i=0; i<num; i++) {
        memcpy((*ppdest) + loc, bufs[i].p, bufs[i].size);
        loc += bufs[i].size;
    }
}


/* use um_free(*ppdest) to free the return dest value. */
int ubuffer_replace(const void* src, size_t len_src, 
    		void** ppdest, size_t* len_dest, 
            struct urange* ranges, long nranges,
            struct ucbuf* bufs_replace, long nbufs_replace)
{
    int ret = 0;

    struct ucbuf buf;
    struct ucbuf buf_to;

    long num = 0;
    long total = (2*nranges + 1);
    struct ucbuf* bufs = um_malloc(sizeof(*bufs) * total);
    
	int i;
    for(i=0; i<nranges; i++) {
        if((ranges[i].location + ranges[i].length) > len_src) {
            ulogerr("replace range invalid(total:%zd, location:%ld, length:%ld).\n", 
                    len_src, ranges[i].location, ranges[i].length);
            ret = -1;
            break;
        }

        /* before replace segment. */
        if(0 == i) {
            buf.p = src;
            buf.size = ranges[i].location;
        }
        else {
            /* need to check with previous. */
            if(ranges[i].location < (ranges[i-1].location + ranges[i-1].length)) {
                ulogerr("replace range invalid(prev location:%ld, length:%ld, location:%ld, length:%ld).\n", 
                        ranges[i-1].location, ranges[i-1].length, 
                        ranges[i].location, ranges[i].length);
                ret = -1;
                break;
            }

            buf.p = src + (ranges[i-1].location + ranges[i-1].length);
            buf.size = ranges[i].location - (ranges[i-1].location + ranges[i-1].length);
        }

        if(i<nbufs_replace) {
            buf_to.p    = bufs_replace[i].p;
            buf_to.size = bufs_replace[i].size;
        }
        else {
            buf_to.p    = bufs_replace[nbufs_replace-1].p;
            buf_to.size = bufs_replace[nbufs_replace-1].size;
        }

        _ubufs_add(bufs, total, &num, buf);
        _ubufs_add(bufs, total, &num, buf_to);

        if(i == nranges-1) {
            buf.p = src + (ranges[i].location + ranges[i].length);
            buf.size = len_src - (ranges[i].location + ranges[i].length);
            _ubufs_add(bufs, total, &num, buf);
        }
    }

    if(0 == ret) {
        _ubuf_dup_from_bufs(ppdest, len_dest, bufs, num);
    }
    um_free_check(bufs);

    return ret;
}










