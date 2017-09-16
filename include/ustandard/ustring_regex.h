#ifndef __USTRING_REGEX_H__
#define __USTRING_REGEX_H__
__BEGIN_DECLS


/*
    return : found ranges. 
            -1: regcomp return error. usually it the regex string invalid. or input value error.
             0: not found.
           > 0: found number. it would not larger then nmax. so if return value equal to nmax, maybe there're more matched pattern.

 */
long ustrreg_find(const char* s, 
        const char* regex, 
        struct urange* range,
        long nmax);

__END_DECLS
#endif /* ustring_regex.h */
