#ifndef __UBUFFER_H__
#define __UBUFFER_H__
__BEGIN_DECLS
struct ubuffer_data {
    const void*   ptr;
    size_t  size;
};


int ubuffer_split_by_char(const void* src, size_t size, 
        int c, 
        struct ubuffer_data* data, 
        int nmax);

__END_DECLS
#endif//ubuffer.h
