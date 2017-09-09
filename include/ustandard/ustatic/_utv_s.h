#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int _utv_get_monotonic(struct timeval* tv)
{
    int ret = 0;

    struct timespec ts;
    ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    tv->tv_sec  = ts.tv_sec;
    tv->tv_usec = ts.tv_nsec/(long)1000;

    return ret;
}


static struct timeval _utv_count_tv(struct timeval* tstart, struct timeval* tend)
{
    struct timeval ret_timeval;

    if(tend->tv_usec >= tstart->tv_usec) {
        ret_timeval.tv_sec = tend->tv_sec - tstart->tv_sec;
        ret_timeval.tv_usec = tend->tv_usec - tstart->tv_usec;
    }
    else {
        ret_timeval.tv_sec = tend->tv_sec - tstart->tv_sec - 1;
        ret_timeval.tv_usec = (tend->tv_usec + 1000000) - tstart->tv_usec;
    }

    return ret_timeval;
}

