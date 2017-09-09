#include "ustandard/ustandard.h"
struct test_ucycm_additional {
    unsigned char c1[10];
    unsigned long sn;
    char s1[36];
    unsigned char c2[1];
};
static int _uchar_array_check(void* addr, int c, size_t size)
{
    int ret = 0;
    int i;
    for(i=0; i<size; i++) {
        if(((unsigned char*)addr)[i] != c) {
            ulogdbg("addr = %p,v = %d,c=%d.\n", addr, ((unsigned char*)addr)[i], c);
            ret = -1;
            break;
        }
    }

    return ret;
}


static void* run_ucycm(void* arg);
static void* run_ucycm_read(void* arg);

static int kcreated = 0;

int main()
{
    uslog_sys_set(__vd, stdout);
    
    pthread_t pth;
    int ret = 0;

    ret = pthread_create(&pth, NULL, run_ucycm, NULL);
    assert(0 == ret);

    ret = pthread_create(&pth, NULL, run_ucycm_read, NULL);
    assert(0 == ret);

    while(1) {
        sleep(10);
    }

    return 0;
}


void* run_ucycm(void* arg)
{
    int ret = 0;

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
    kcreated = 1;

    struct timeval tv_real;
    struct timeval tv_mono;

    void* c = malloc(3*1024*1024);
    assert(NULL != c);

    int t = 1;
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

    return NULL;
}


void* run_ucycm_read(void* arg)
{    
    while(!kcreated) {
        usleep(1*1000);
    }
    struct ucycm_read* cmr = ucycmr_create(NULL, 0x1234, 1024*1024, 
            UCYCMR_TYPE_NEWEST, NULL, NULL);
    assert(NULL != cmr);
    
    struct timeval to = {3, 0}; 
    unsigned long sn;
    struct ucycm_read_segment_info info; 
    struct test_ucycm_additional add[2];
    memset(add, 222, sizeof(struct test_ucycm_additional)*2);
    int ret;
    
    while(1) {
        ret = ucycmr_get(cmr, &to, &sn, &info, add);
        assert(0 == ret);

        /* check info */
        ulogdbg("sn = %lu.\n", sn);
        ulogdbg("info: [%lu:%06lu] [%lu:%06lu], addr=%p, size=%u, [%lu, %lu].\n", info.tv_real.tv_sec, info.tv_real.tv_usec, info.tv_mono.tv_sec, info.tv_mono.tv_usec, info.addr, info.size, info.sn_min, info.sn_max);

        /* check data. */
        ret = _uchar_array_check(info.addr, sn%255, info.size);
        assert(0 == ret);

        /* check additonal. */
        assert(0 == _uchar_array_check(add[0].c1, 1, 10));
        assert(0 == _uchar_array_check(add[0].c2, 100, 1));
        assert(add[0].sn == sn);
        char s[36];
        memset(s, 111, 36);
        snprintf(s, 36, "%10lu %10lu", sn, sn);
        assert(0 == memcmp(add[0].s1, s, 36));
        assert(((unsigned char*)(add+1))[0] == 'x');
        assert(0 == _uchar_array_check(
                    (void*)(add+1)+1, 222, sizeof(struct test_ucycm_additional)-1));

        printf("\n");
    }

    ret = ucycmr_destroy(cmr);
    cmr = NULL;

    return NULL;
}









