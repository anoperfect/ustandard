#include "ustandard/ustandard.h"
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
