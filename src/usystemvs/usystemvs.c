#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/usystemvs.h"


int usystemvs(const char *fmt, ...)
{
    int ret = 0;
    int n;

    #define LEN_CMD     1024
    char cmd[LEN_CMD];

    va_list ap;
    va_start(ap, fmt);
    n = vsnprintf(cmd, LEN_CMD, fmt, ap);
    va_end(ap);

    if(n>-1 && n< LEN_CMD) {
        ret = system(cmd);
    }
    else {
        ulogerr("systemc cmd too long.\n");
        ret = -1;
    }

    return ret;
}
