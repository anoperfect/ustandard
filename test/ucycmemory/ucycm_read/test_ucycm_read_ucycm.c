#include "ustandard/ustandard.h"
#include "../ucycm_test_public.h"
int main()
{
    int ret = 0;

    setbuf(stderr, NULL);
    uslog_sys_set(1, stdout);

    struct test_ucycm_additional add[2];
    memset(add, 111, sizeof(struct test_ucycm_additional)*2);
    memset(add[0].c1, 1, 10);
    memset(add[0].c2, 100, 1);
    memset(&add[1], 'x', 4);

    struct ucycm* cm = ucycm_create(NULL, 0x1234, 1024*1024, 
            "test", 
            300, sizeof(add[0])+1, 
            1, 
            NULL);
    assert(NULL != cm);

    struct timeval tv_real;
    struct timeval tv_mono;

    void* c = malloc(3*1024*1024);
    assert(NULL != c);

    int t = 0;
    while(1) {
        size_t size = urandft(1, (1024*1024 /2 - 29184)/(t%10 + 1));

        gettimeofday(&tv_real, NULL);
        utv_get_monotonic(&tv_mono);

        add[0].sn = t;
        snprintf(add[0].s1, 36, "%10d %10d", t, t);

        memset(c, t%255, size);

        ret = ucycm_add(cm, &tv_real, &tv_mono, NULL, c, size, add);
        assert(0 == ret);

        t++;
        if(0 == t%(1024*1)) {
            fprintf(stderr, "%6d", t/(1024*1));
        }

        usleep(30*1000);
    }

    ucycm_destroy(cm);
    cm = NULL;


    return 0;
}

