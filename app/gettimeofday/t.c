#include "ustandard/ustandard.h"




#define NUM 1024*1024
int main(int argc, char* argv[])
{
    //struct timeval tv;

    struct timeval tv0;
    long long usec;
    int idx;

    while(1) {

    gettimeofday(&tv0, NULL);
    for(idx=0; idx<NUM; idx++) {
        uloginf("1234567890\n");
    }
    usec = utv_expired(&tv0);
    ulogerr("%lld\n", usec);

    gettimeofday(&tv0, NULL);
    for(idx=0; idx<NUM; idx++) {
        long long usec = utv_expired(&tv0);
        uloginf("[%lld.%06lld]1234567890\n", usec/1000000, usec%1000000);
    }
    usec = utv_expired(&tv0);
    ulogerr("%lld\n", usec);

    }




    return 0;

}
