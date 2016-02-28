#ifndef __UMP_SIMPLE_H__
#define __UMP_SIMPLE_H__
__BEGIN_DECLS
struct ump_simple;
struct ump_simple* ump_simple_create(size_t total);
int ump_simple_destroy(struct ump_simple* ump);

void* ump_simple_alloc(struct ump_simple* ump, size_t size);
int ump_simple_free(struct ump_simple* ump, void* ptr);
int ump_simple_query(struct ump_simple* ump, char* str, size_t size);
__END_DECLS
#endif//ump_simple.h
