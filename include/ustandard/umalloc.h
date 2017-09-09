#ifndef __UMALLOC_H__
#define __UMALLOC_H__
__BEGIN_DECLS


#ifndef DISABLE_UMALLOC

#define um_malloc(size)         umalloc(size, __FILE__, __FUNCTION__, __LINE__)
#define um_free(ptr)            ufree(ptr, __FILE__, __FUNCTION__, __LINE__)
#define um_realloc(ptr, size)   urealloc(ptr, size, __FILE__, __FUNCTION__, __LINE__)

void* umalloc(size_t size, 
        const char* file, const char* function, unsigned int line);

int ufree(void* ptr, 
        const char* file, const char* function, unsigned int line);

void* urealloc(void* ptr, size_t size, 
        const char* file, const char* function, unsigned int line);


#else

#define um_malloc(size)             malloc(size)
#define um_free(ptr)                free(ptr)
#define um_realloc(ptr, size)       realloc(ptr, size)

#endif


size_t umalloc_query(char* s, size_t size);
int umalloc_query_clear(void);
int umalloc_enable(int set);

#define um_umalloc_perror(size) ulogerr("umalloc(%u) error at %s:%u.\n", size, __FILE__, __LINE)




__END_DECLS
#endif//umalloc.h
