#include "ustandard/ustandard.h"




int main()
{
    int ret;

    struct ustrset* ss = ustrset_create(1024, 1024, 10*1024, 10*1024);
    ret = ustrset_copy(ss, "1", 1);                             assert(0 == ret);
    ret = ustrset_clear(ss);                                    assert(0 == ret);
    ret = ustrset_copy(ss, "1", 1);                             assert(0 == ret);
    ret = ustrset_copy(ss, "23", 2);                            assert(0 == ret);
    ret = ustrset_copy(ss, "456", 3);                           assert(0 == ret);

    ret = ustrset_dup(ss, "xxxxxx", 6);                         assert(0 == ret);
    ret = ustrset_dup(ss, "yy", 2);                             assert(0 == ret);
    ret = ustrset_dup(ss, "z", 1);                              assert(0 == ret);

    #define LEN_BUF     1024

    size_t size_total = ustrset_len(ss);
    printf("size_total = %u.\n", size_total);

    char buf[LEN_BUF];
    size_t len;
    ret = ustrset_combine(buf, LEN_BUF-1, &len, ss);            assert(0 == ret);
    buf[len] = '\0';
    printf("buf = %s .\n", buf); 
    
    ret = ustrset_destory(ss);                                   assert(0 == ret);


    return 0;
}
