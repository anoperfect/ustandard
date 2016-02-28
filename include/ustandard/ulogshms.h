#ifndef __ULOGSHMS_H__
#define __ULOGSHMS_H__
__BEGIN_DECLS


struct ulogshms* ulogshms_create(key_t key, size_t size, int nblock);
int ulogshms_destroy(struct ulogshms* s);
int ulogshms_printf(struct ulogshms* s, const char* format, ...);
int ulogshms_line(struct ulogshms* s, char name, unsigned short int line);
int ulogshms_data(struct ulogshms* s, char name, const void* p, size_t size);

int ulogshmc(key_t key, size_t size, 
        int (*cb_printf)(const void* p, size_t size), 
        int (*cb_line)(char name, unsigned short int line), 
        int (*cb_data)(char name, const void* p, size_t size));




__END_DECLS
#endif/* ulogshms.h */


