#ifndef __UBACKTRACE_H__
#define __UBACKTRACE_H__
__BEGIN_DECLS


void ubacktrace_set(const char* pathname);

int ubacktrace_enter(const char* name, const char* file, int line);
int ubacktrace_exit(const char* name, const char* file, int line);

    #define __enter_        ubacktrace_enter(__FUNCTION__, __FILE__, __LINE__);
    #define __return_       return ubacktrace_exit(__FUNCTION__, __FILE__, __LINE__)?0:
    #define __return_do(x)  do{ubacktrace_exit(__FUNCTION__, __FILE__, __LINE__); return (x);}while(0)
    #define __return_void   do{ubacktrace_exit(__FUNCTION__, __FILE__, __LINE__); return    ;}while(0);

#ifdef UBACKTRACE_DISABLE
    #undef __enter_        
    #undef __return_       
    #undef __return_do(x)  
    #undef __return_void   

    #define __enter_        
    #define __return_       return 
    #define __return_do(x)  return x
    #define __return_void   return
#endif


__END_DECLS
#endif /* ubacktrace.h */
