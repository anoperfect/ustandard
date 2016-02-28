#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
int main(int argc, char* argv[])
{
    if(argc != 3) {
        printf("usage : printt pathname 1024.\n");
        exit(1);
    }

    const char* pathname = argv[1];
    int meganum = atol(argv[2]);
    FILE* fp = fopen(argv[1], "a");
    assert(NULL != fp);

    struct timeval s, e;
    gettimeofday(&s, NULL);

    int size = 0;
    while(size < meganum*1024*1024) {
        size += fwrite("logloglog1", 1, 10, fp);
        //fflush(fp);
    }

    fclose(fp);

    gettimeofday(&e, NULL);
    double sec = (double)(e.tv_sec-s.tv_sec) + (double)(e.tv_usec-s.tv_usec)/1000000.0;
    printf("used : %lf.\n", sec);

    
    return 0;
}
