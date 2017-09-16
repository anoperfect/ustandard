#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ustring_regex.h"


/*
    return : found ranges. 
            -1: regcomp return error. usually it the regex string invalid. or input value error.
             0: not found.
           > 0: found number. it would not larger then nmax. so if return value equal to nmax, maybe there're more matched pattern.
 */
long ustrreg_find(const char* s, 
        const char* regex, 
        struct urange* range,
        long nmax)
{
    uslog_check_arg(NULL != s,      -1);
    uslog_check_arg(NULL != regex,  -1);
    uslog_check_arg(NULL != range,  -1);
    uslog_check_arg(nmax > 0,       -1);

    int retn = 0;

    regex_t reg = {0};
    int ret = regcomp(&reg, regex, REG_EXTENDED);
    if(0 != ret) {
        ulogerr("regex pattern error(%s).\n", regex);
        retn = -1;
        goto finish;
    }

    const char* t = s;

    while(1) {
        regmatch_t match;
        ret = regexec(&reg, t, 1, &match, 0);
        if(0 != ret) {
            break;
        }

        if(retn < nmax) {
            range[retn].location = (t-s) + match.rm_so;
            range[retn].length = match.rm_eo - match.rm_so;

            retn ++;
            t += match.rm_eo;
        }
        else {
            break;
        }
    }

finish:
    regfree(&reg);
    return retn;
}
















