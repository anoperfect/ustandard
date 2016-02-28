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
#if 0
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif


struct udata_const {
    const void* ptr;
    size_t      size;
};

























#define KLEN_PATH       (1024)







































#endif//__USTANDARD_TYPES_H__
