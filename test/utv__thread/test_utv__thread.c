#include "ustandard/ustandard.h"
void* func(void* arg)
{
    ttv.tv_sec = 1;
    ttv.tv_usec = 1;


    while(1) {
        uloginf("sub  thread : %zd:%06zd\n", ttv.tv_sec, ttv.tv_usec);
        sleep(1);
    }
}



int main(int argc, char* argv[])
{
    pthread_t pth;
    pthread_create(&pth, NULL, func, NULL);

    while(1) {
        uloginf("main thread : %zd:%06zd\n", ttv.tv_sec, ttv.tv_usec);
        sleep(1);
    }
    


    return 0;
}
