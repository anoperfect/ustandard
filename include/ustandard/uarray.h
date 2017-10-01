#ifndef __UARRAY_H__
#define __UARRAY_H__
__BEGIN_DECLS


struct uarray* uarray_create(size_t size_entry, int limit);
struct uarray* uarray_create_detail(size_t size_entry, int limit, updescription des);
void uarray_destroy(struct uarray* array);

int uarray_add_at(struct uarray* array, int at, void* p);


/*
    idx : -1 or >=count would be add to tailer. 
 */
int uarray_delete_at(struct uarray* array, int at);

void* uarray_at(struct uarray* array, int at);

int uarray_description(struct uarray* array, const char* title, char* dest, size_t size);

__END_DECLS
#endif /* uarray.h */
