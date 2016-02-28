#include "ustandard/ustandard.h"




#define __log(fmt...) fprintf(fp, fmt)
#define PATH_PID    "/var/run/streammgrd.pid"
int main()
{
    int ret;

    FILE* fp = fopen("log", "w");
    assert(NULL != fp);
    setbuf(fp, NULL);

    ret = upid_set(PATH_PID);
    if(0 == ret) {
        __log("OK.\n");
    }
    else {
        __log("LOCK FAILED.\n");
    }
    __log("pid=%d\n", getpid());

    daemon(0, 0);

    __log("pid=%d\n", getpid());
    usleep(10*1000);
    ret = upid_set(PATH_PID);
    if(0 == ret) {
        __log("OK.\n");
    }
    else {
        __log("LOCK FAILED.\n");
    }




    sleep(20);

    return 0;
}
