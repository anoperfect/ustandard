#ifndef __USTANDARD_DEBUG_H__
#define __USTANDARD_DEBUG_H__
#define USLOG_ENABLE            1
#if USLOG_ENABLE
    
    #include "ustandard/ustandard_sys.h"
    #include "ustandard/uslog.h"
    extern struct uslog_data* klog_ustandard;
    #define ulognon(fmt...)     __uslog(klog_ustandard, 0, fmt)
    #define ulogtst(fmt...)     __uslog(klog_ustandard, 1, fmt)
    #define uloginf(fmt...)     __uslog(klog_ustandard, 2, fmt)
    #define ulogdbg(fmt...)     __uslog(klog_ustandard, 3, fmt)
    #define ulogerr(fmt...)     __uslog(klog_ustandard, 4, fmt)

#else 

    #define ulognon(fmt...)
    #define ulogtst(fmt...)
    #define uloginf(fmt...)     fprintf(stdout, fmt)
    #define ulogdbg(fmt...)
    #define ulogerr(fmt...)     fprintf(stderr, fmt)

#endif

#endif /* ustandard_debug.h */
