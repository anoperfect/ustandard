#ifndef __USVMESG_C_H__
#define __USVMESG_C_H__
__BEGIN_DECLS


int usvmesgc_send(const char* sender, const char* recver, 
        const void* p, size_t size);


__END_DECLS
#endif /* usvmesgc.h */
