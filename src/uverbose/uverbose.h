#ifndef __UVERBOSE_H__
#define __UVERBOSE_H__
__BEGIN_DECLS


extern FILE* kv_debug;
extern FILE* kv_info;
extern FILE* kv_error;


void uverbose_set(const char* level, FILE* fp)



__END_DECLS
#endif//uverbose.h
