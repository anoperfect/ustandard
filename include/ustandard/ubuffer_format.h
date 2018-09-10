#ifndef __UBUFFER_FORMAT_H__
#define __UBUFFER_FORMAT_H__
__BEGIN_DECLS
typedef enum{
    e_ubuffer_char_type_default = 0,
    e_ubuffer_char_type_0x,
    e_ubuffer_char_type_0X,
    e_ubuffer_char_type_0xprint,
    e_ubuffer_char_type_print,
}ubuffer_char_type_e;





int ubuffer_format(char* dest, size_t size_dest, 
        const void* ptr, size_t size, 
        ubuffer_char_type_e type, 
        const char* space, int num_in_line);


/* ->ustandard_init. */
int ubuffer_tostr_init(void);


int ubuffer_tostr_deinit(void);


const char* ubuffer_tostr(const void* ptr, size_t size, 
        ubuffer_char_type_e type, 
        const char* space, int num_in_line);


const char* ubuffer_tostr_easy(const void* ptr, size_t size);
__END_DECLS
#endif /* ubuffer_format.h */
