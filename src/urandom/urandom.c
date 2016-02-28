#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/urandom.h"


int urand(void)
{
    int ret;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    srand(tv.tv_sec + tv.tv_usec);
    ret = rand();

    return ret;
}


int urandft(int f, int t)
{
    int ret;

    ret = urand();
    ret = f + ret%(t-f+1);

    return ret;
}
