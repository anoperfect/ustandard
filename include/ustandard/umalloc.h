#ifndef __UMALLOC_H__
#define __UMALLOC_H__
__BEGIN_DECLS


#ifndef DISABLE_UMALLOC

#define um_malloc(size)         umalloc(size, __FILE__, __FUNCTION__, __LINE__)
#define um_free(ptr)            ufree(ptr, __FILE__, __FUNCTION__, __LINE__)
#define um_free_check(ptr)      if(ptr) {ufree(ptr, __FILE__, __FUNCTION__, __LINE__); ptr = NULL;}
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
#define um_free_check(ptr)          if(ptr) {free(ptr); ptr = NULL;}
#define um_realloc(ptr, size)       realloc(ptr, size)

#endif


size_t umalloc_query(char* s, size_t size);
int umalloc_query_clear(void);
int umalloc_enable_monitor(int is_enabled); /*0 minitor off, other on. default 0. */
int umalloc_verbose_output(FILE* fp);







__END_DECLS
#endif /* umalloc.h */
