#ifndef __ULINKLIST_H__
#define __ULINKLIST_H__
__BEGIN_DECLS


struct ulinklist* ulinklist_create(void);
struct ulinklist* ulinklist_create_detail(uvdata_free_func data_free);
int ulinklist_destroy(struct ulinklist* list, bool clear);


int ulinklist_add_header(struct ulinklist* list, void* p);
int ulinklist_add_header_by_copy(struct ulinklist* list, void* p, size_t size);


int ulinklist_add_tailer(struct ulinklist* list, void* p);
int ulinklist_add_tailer_by_copy(struct ulinklist* list, void* p, size_t size);


struct uvdata* ulinklist_next(struct ulinklist* list);
struct uvdata* ulinklist_prev(struct ulinklist* list);


int ulinklist_length(struct ulinklist* list);
struct uvdata* ulinklist_header(struct ulinklist* list);
struct uvdata* ulinklist_tailer(struct ulinklist* list);


int ulinklist_add_after(struct ulinklist* list, struct uvdata* data, void* p);
int ulinklist_add_after_by_copy(struct ulinklist* list, struct uvdata* data, void* p, size_t size);

int ulinklist_add_before(struct ulinklist* list, struct uvdata* data, void* p);
int ulinklist_add_before_by_copy(struct ulinklist* list, struct uvdata* data, void* p, size_t size);

int ulinklist_remove_data(struct ulinklist* list, struct uvdata* data, bool clear);

struct uvdata* ulinklist_at(struct ulinklist* list, int idx);







__END_DECLS
#endif /* ulinklist.h */
