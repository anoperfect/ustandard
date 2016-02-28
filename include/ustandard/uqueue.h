#ifndef __UQUEUE_H__
#define __UQUEUE_H__
__BEGIN_DECLS

typedef void* (*udata_copy)(void* dest, const void *src, size_t n);
typedef int (*udata_cmp)(void* s1, void* s2, size_t n);

struct uqueue;
struct uqueue* uqueue_create(int total, size_t size, 
        udata_copy copy); 

int uqueue_destroy(struct uqueue* queue);

int uqueue_num(struct uqueue* queue);

int uqueue_push(struct uqueue* queue, void* item);

void* uqueue_pop(struct uqueue* queue);

int uqueue_clear(struct uqueue* queue);

/* 
   sometimes maybe need different compare function, 
   so define udata_cmp here, not in create. 
*/
void* uqueue_pop_with_compare(struct uqueue* queue, udata_cmp cmp, void* arg);

void* uqueue_get(struct uqueue* queue, int index);

__END_DECLS
#endif//uqueue.h
