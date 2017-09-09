#ifndef __USTREAM_H__
#define __USTRSET_H__



#include <stdio.h>
__BEGIN_DECLS


int ustream_dup_from_file(const char* path, void* ptr, size_t size, size_t* size_copy);
int ustream_dup_to_file(const char* path, const void* ptr, size_t size);
int ustream_append_to_file(const char* path, const void* ptr, size_t size);


__END_DECLS

#endif//__USTRSET_H__
