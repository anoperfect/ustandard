#ifndef __USTANDARD_TYPES_H__
#define __USTANDARD_TYPES_H__
__BEGIN_DECLS

#if HAVE_STDBOOL_H
    #include <stdbool.h>
#else
    #ifndef __cplusplus

        #define bool	_Bool
        #define true	1
        #define false	0

    #else /* __cplusplus */

        /* Supporting <stdbool.h> in C++ is a GCC extension.  */
        #define _Bool	bool
        #define bool	bool
        #define false	false
        #define true	true

    #endif /* __cplusplus */

    /* Signal that all the definitions are present.  */
    #define __bool_true_false_are_defined	1

#endif 

typedef unsigned char uchar_t;


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





typedef void (*upfree_func)(void* p);



typedef int (*updescription)(char* str, size_t size, void* p);


typedef int (*upf_description)(char* str, size_t size, void* p);





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
