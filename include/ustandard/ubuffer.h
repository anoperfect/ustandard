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


/* not test. */
void* ubuf_move_left(void* p, size_t size_src, size_t size_dest);


void* ubuf_move_right(void* p, size_t size, size_t n);









/*
    return : found ranges. 
            -1: input value invalid. 
             0: not found.
           > 0: found number. it would not larger then nmax. so if return value equal to nmax, maybe there're more matched pattern.
 */
long ubuffer_find(const void* s, size_t size, 
        const char* needle, size_t size_needle, 
        struct urange* range,
        long nmax);


int ubuf_dup_from_bufs(
    		void** ppdest, size_t* psize, 
            struct ucbuf* bufs, long num);


/* use um_free(*ppdest) to free the return dest value. */
int ubuffer_replace(const void* src, size_t len_src, 
    		void** ppdest, size_t* len_dest, 
            struct urange* ranges, long nranges,
            struct ucbuf* bufs_replace, long nbufs_replace);










__END_DECLS
#endif /* ubuffer.h */
