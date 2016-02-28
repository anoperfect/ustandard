#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
#define NUM (10*1000*1000)
int main()
{
    struct timeval tvs; 
    struct timeval tve; 
    struct timeval tv; 
    gettimeofday(&tvs, NULL);

    int i = 0;
    while(i++ < NUM) {
        utv_get_monotonic(&tv);
    }

    gettimeofday(&tve, NULL);
    int64_t usec = utv_count(&tvs, &tve);
    double sec = (double)usec / (double)(1000*1000);
    double sec1m = (double)usec / (double)NUM ;
    printf("usec = %lld, sec=%lf, run 1m time need %lf\n", usec, sec, sec1m);

    return 0;
}
