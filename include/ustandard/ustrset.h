#ifndef __USTRSET_H__
#define __USTRSET_H__
#include <stdio.h>
__BEGIN_DECLS
struct ustrset;
struct ustrset* ustrset_create(int num, int max_num, size_t size_buf, size_t max_buf);
int ustrset_copy(struct ustrset* ss, const void* buf, size_t size);
int ustrset_dup(struct ustrset* ss, const void* buf, size_t size);
int ustrset_clear(struct ustrset* ss);
int ustrset_destroy(struct ustrset* ss);
size_t ustrset_len(const struct ustrset* ss);
int ustrset_combine(void* dest, size_t size_dest, size_t* size_copy, 
        const struct ustrset* ss);
int ustrset_link(struct ustrset* dest, const struct ustrset* src);

struct ustrseg
{
    const void*     buf;
    size_t          size;
};

int ustrset_num(const struct ustrset* ss);
struct ustrseg* ustrset_getseg(const struct ustrset* ss, int idx);

int ustrset_fwrite(const struct ustrset* ss, FILE* fp);

__END_DECLS

#endif//__USTRSET_H__
