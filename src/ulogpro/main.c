#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include "ulogpro.h"
int main(int argc, char* argv[])
{
do{
    ulog_init(argc, argv);
}while(0);
    //ulogdbg("debug test.\n");
    //uloginf("info test.\n");
    //ulogerr("error test.\n");

    int i;
    int test_times = 1024*1024*100;
    struct timeval s, e;
    gettimeofday(&s, NULL);
    for(i=0; i<test_times; i++) {
        ulog("content", __FUNCTION__, __FILE__, __LINE__, "%s-----%s\n", __FUNCTION__, __FILE__);
    }
    gettimeofday(&e, NULL);

    int64_t usec = (int64_t)(e.tv_sec-s.tv_sec)*(int64_t)1000000 + (int64_t)(e.tv_usec-s.tv_usec);
    double sec = (double)usec/1000000.0;
    printf("sec = %lf.\n", sec);

    return 0;
}
