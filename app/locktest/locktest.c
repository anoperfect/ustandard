#include "ustandard/ustandard.h"




int knumber = 0;
struct timeval ktv;
pthread_mutex_t klock = PTHREAD_MUTEX_INITIALIZER;

int ktimes = 100000000;

int addfunc(void)
{
    int t = 60;
    while(t--) {

    }
    //free(malloc(10));
    return 0;
}

void count_no_lock(void)
{
    while(1) {
        knumber ++;
        addfunc();
        if(knumber >= ktimes) {
            uloginf("%lf\n", (double)utv_expired(&ktv)/1000000.0);
            break;
        }
    }
}


void count_lock(void)
{
    while(1) {
        pthread_mutex_lock(&klock);
        knumber ++;
        addfunc();
        if(knumber >= ktimes) {
            uloginf("%lf\n", (double)utv_expired(&ktv)/1000000.0);
            pthread_mutex_unlock(&klock);
            break;
        }
        pthread_mutex_unlock(&klock);
    }
}



void* func(void* arg)
{
    count_lock();
    return NULL;
}


int main(int argc, char* argv[])
{
    knumber = 0;
    gettimeofday(&ktv, NULL);
    count_no_lock();

    knumber = 0;
    gettimeofday(&ktv, NULL);
    count_lock();

    knumber = 0;
    gettimeofday(&ktv, NULL);

    int t = 10;
    while(t--) {
        pthread_t pth;
        pthread_create(&pth, NULL, func, NULL);
    }

    count_lock();

    
    while(1) {
        sleep(1);
    }



    return 0;
}
