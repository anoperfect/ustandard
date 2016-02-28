#include "ustandard/ustandard.h"
#include "../ucycm_test_public.h"
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


int main()
{
    uslog_sys_set(USLOG_USTANDARD_DEBUG, stdout);
    
    struct ucycm_read* cmr = ucycmr_create(NULL, 0x1234, 1024*1024, 
            UCYCMR_TYPE_NEWEST, NULL, NULL);
    assert(NULL != cmr);
    
    struct timeval to = {1, 0}; 
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

    return 0;
}
