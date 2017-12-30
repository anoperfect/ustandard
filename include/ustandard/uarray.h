#ifndef __UARRAY_H__
#define __UARRAY_H__
__BEGIN_DECLS


struct uarray* uarray_create(size_t size_entry);
struct uarray* uarray_create_detail(size_t size_entry, int limit, int num_init, int num_expand, upf_description des);
void uarray_destroy(struct uarray* array);

int uarray_count(struct uarray* array);


/*
    idx : -1 or >=count would be add to tailer. 
 */
int uarray_add_at(struct uarray* array, int at, void* p);


int uarray_delete_at(struct uarray* array, int at);
int uarray_clear(struct uarray* array, int count, int total);

void* uarray_at(struct uarray* array, int at);
int uarray_value(struct uarray* array, int at, void* p);

int uarray_description(struct uarray* array, const char* title, char* dest, size_t size);



__END_DECLS
#endif /* uarray.h */
