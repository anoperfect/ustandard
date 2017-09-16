#ifndef __ULINKLIST_H__
#define __ULINKLIST_H__
__BEGIN_DECLS


struct ulinklist* ulinklist_create(void);
int ulinklist_destroy(struct ulinklist* list);

int ulinklist_add_header(struct ulinklist* list, int payload);
int ulinklist_add_tailer(struct ulinklist* list, int payload);

int ulinklist_next(struct ulinklist* list);
int ulinklist_prev(struct ulinklist* list);





__END_DECLS
#endif //ulinklist.h
