#define UBACKTRACE_ENABLE   1
#include "ustandard/ustandard.h"




int test1(void);
void* test2(void* arg);
void test3(void);

int main()
{
    ubacktrace_set("httpd");

__enter_

    test1();
    sleep(1);

    test2(NULL);

    int i;
    for(i=0; i<10; i++) {
        pthread_t pth;
        pthread_create(&pth, NULL, test2, NULL);
    }

    sleep(1);

    __return_(0);
}


int test1(void)
{__enter_
    __return_(-1);
}


void* test2(void* arg)
{__enter_

    test3();
    __return_(NULL);
}


void test3(void)
{__enter_
    __return_void;
}
