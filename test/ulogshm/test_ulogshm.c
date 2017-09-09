#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
int main()
{
    ustandard_test();

    //uslog_sys_set(__vd, stdout);
    int ret = 0;

    struct ulogshms* s = ulogshms_create(0x1234, 1204, 9);
    __assert(NULL != s);

    //ret = ulogshmc(0x1234, 1024, NULL, NULL, NULL);
    __assert(0 == ret);

    ulogshms_printf(s, "A.\n");
    ret = ulogshmc(0x1234, 1204, NULL, NULL, NULL);
    __assert(0 == ret);

    ulogshms_printf(s, "B");
    ret = ulogshmc(0x1234, 1204, NULL, NULL, NULL);
    __assert(0 == ret);

    ulogshms_printf(s, "Hello Kitty.");
    ret = ulogshmc(0x1234, 1204, NULL, NULL, NULL);
    __assert(0 == ret);

    ulogshms_line(s, 'x', __LINE__);
    ret = ulogshmc(0x1234, 1204, NULL, NULL, NULL);
    __assert(0 == ret);

    printf("\n\n");
    ulogshms_printf(s, "123456.\n");
    ulogshms_printf(s, "123456.\n");
    ulogshms_printf(s, "123456.\n");
    ulogshms_printf(s, "123456.\n");
    ulogshms_printf(s, "123456.\n");
    ulogshms_printf(s, "123456.\n");
    ulogshms_printf(s, "123456.\n");
    ulogshms_printf(s, "123456");
    ulogshms_printf(s, "12345xx");
    ulogshms_printf(s, "123456.\n");
    ulogshms_printf(s, "123456.\n");
    ulogshms_line(s, 'y', __LINE__);
    ulogshms_printf(s, "Hello Kitty.");
    ulogshms_data(s, 'u', "Hello Kitty.", 12);
    ulogshms_line(s, 'y', __LINE__);
    ret = ulogshmc(0x1234, 1204, NULL, NULL, NULL);
    __assert(0 == ret);






    ulogshms_destroy(s);
    s = NULL;

    return 0;
}
