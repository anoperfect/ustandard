#include "ustandard/ustandard.h"
#include "../ucycm_test_public.h"
int main()
{
    int ret = 0;

    setbuf(stderr, NULL);
    uslog_sys_set(1, stdout);

    struct ucycm* cm = ucycm_create(NULL, 0x1234, 1024*1024, 
            "test", 
            300, 64, 
            1, 
            NULL);
    assert(NULL != cm);

    int additional[16] = {1, 0};
    struct timeval tv_real;
    struct timeval tv_mono;

    void* c = malloc(3*1024*1024);
    assert(NULL != c);

    int t = 0;
    while(1) {
        size_t size = urandft(1, (1024*1024 - 29184 - 16)/(t%10 + 1));
        printf("\n---------------------------------------------------------------------------------------ucycm_add. size = %u\n", size);
        ret = ucycm_add(cm, &tv_real, &tv_mono, NULL, c, size, additional);
        assert(0 == ret);

        t++;
        if(0 == t%(1024*1)) {
            fprintf(stderr, "%6d", t/(1024*1));
        }
    }

    ucycm_destroy(cm);
    cm = 0;
    return 0;
}
