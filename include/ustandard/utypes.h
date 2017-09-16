#ifndef __USTANDARD_TYPES_H__
#define __USTANDARD_TYPES_H__




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

    bool pointer_on_use; /* indicate pointer p on use, or payload on use. */
    
    size_t size;
    char payload[0];
};

















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







































#endif//__USTANDARD_TYPES_H__
