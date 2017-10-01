#ifndef __UDATAOPERATE_H__
#define __UDATAOPERATE_H__
__BEGIN_DECLS
typedef int (*udata_init)(void* p, size_t size);
typedef int (*udata_deinit)(void* p, size_t size);

typedef void* (*udata_dup)(void* p, size_t size);

typedef int (*udata_copy)(void* dest, void* src, size_t size);
typedef int (*udata_cmp)(void* p1, void* p2, size_t size);
__END_DELCS
#endif /* udataoperate.h */
