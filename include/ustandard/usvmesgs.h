#ifndef __USVMESG_S_H__
#define __USVMESG_S_H__
__BEGIN_DECLS


__ptr_t usvmesgs_register(const char* name, size_t size);


int usvmesgs_withdraw(__ptr_t id);


struct umesg_recv_data {
    const char* sender;
    const void* ptr;
    size_t      size;
};


int usvmesgs_recv(__ptr_t id, struct umesg_recv_data* data, struct timeval* to);


__END_DECLS
#endif /* usvmesgs.h */
