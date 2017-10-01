#ifndef __USTANDARD_TYPES_H__
#define __USTANDARD_TYPES_H__
__BEGIN_DECLS



#define HAVE__BOOL  1
#if HAVE_STDBOOL_H
    #include <stdbool.h>
#else
    #if !HAVE__BOOL
        #ifdef __cplusplus
            typedef bool _Bool;
        #else
            typedef unsigned char _Bool;
        #endif
    #endif

    #define bool _Bool
    #define false 0
    #define true 1
    #define __bool_true_false_are_defined 1
#endif 


typedef unsigned char uchar_t;


struct udata_const {
    const void* ptr;
    size_t      size;
};



struct uvdata{
    void* p;
    size_t size; /* indicate the payload size. if use reference pointer, size would not be used. */

    bool use_payload; /* indicate use payload to copy data to payload, or just use reference pointer to p. */
    bool sign_remove; /* while data struct query, can not delete while query. so use this sign call xxx_remove_signed to remove.*/
    
    char payload[1];
};


/* how to descripe the node. */
typedef void (*uvdata_descripe_func)(struct uvdata*);

/* how to free the node. */
typedef void (*uvdata_free_func)(struct uvdata*);









typedef int (*updescription)(char* str, size_t size, void* p);







struct urange {
    long location;
    long length;
};





/*
    a buffer segment without modify permit. 
 */
struct ucbuf {
    const void*     p;  /* addr. */
    size_t          size;    
};




#define KLEN_PATH       (1024)






































__END_DECLS
#endif /* utypes.h */
