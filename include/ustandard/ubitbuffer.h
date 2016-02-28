#ifndef __UBITBUFFER_H__
#define __UBITBUFFER_H__
__BEGIN_DECLS


int ubit_get(const void* ptr, unsigned int idx);
uint64_t ubit_gets(const void* ptr, unsigned int idx, unsigned int len);
int ubit_set(void* ptr, unsigned int idx, int v01);
int ubit_sets(void* ptr, unsigned idx, unsigned int len, uint64_t v);


typedef enum {
    ubit_output_type_2 = 0,
    ubit_output_type_16,
    ubit_output_type_10,
}ubit_output_type_e;


struct ubit_parse* ubit_parse_set(const void* ptr, size_t size);
int ubit_parse_reset(struct ubit_parse* parse, const void* ptr, size_t size);
int64_t ubit_parse_r(struct ubit_parse* parse, const char* name, size_t length, 
        ubit_output_type_e type, 
        char* dest, size_t size);
int64_t ubit_parse(struct ubit_parse* parse, const char* name, size_t length, 
        ubit_output_type_e type);
int ubit_parse_skip(struct ubit_parse* parse, unsigned int n);
int ubit_parse_finish(struct ubit_parse* parse);


int ubit_trans01(char* dest, size_t size, const void* ptr, size_t n);
int ubit_trans01_uint(char* dest, size_t size, unsigned int uv);
int ubit_trans01_uint64(char* dest, size_t size, uint64_t uv);


__END_DECLS
#endif//  ubitbuffer.h
