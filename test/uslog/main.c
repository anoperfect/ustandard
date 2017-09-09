#include "ustandard/ustandard.h"
#undef USLOG_MODULE
#define USLOG_MODULE    1

int uslog_test(void)
{
    int ret = 0;
#if 0
    struct uslog_config config = {0};
    config.fp = stdout;
    config.item = USLOG_ENABLE_ALL;
    uslog_set_config(__vd, &config);

    config.fp = stdout;
    config.item = USLOG_ENABLE_FILEINFO;
    uslog_set_config(USLOG_MODULE_LEVEL(USLOG_MODULE_USTANDARD, __vd), &config);
#endif

    ulognon("%s\n", "__v0");

    ulogtst("%s\n", "__vt");

    uloginf("%s\n", "__vi");

    ulogdbg("%s\n", "__vd");

    ulogerr("%s\n", "__ve");

    struct ucycm* cm = ucycm_create(NULL, ftok(".", 1), 1024*1024, 
            "test", 1024, 12, 1, NULL);
    ulogdbg("cm=%p\n", cm);
    ucycm_destroy(cm);
    cm = NULL;




    printf("[->finished]\n");

    return ret;
}


int main()
{
    uloginf("start.\n");
    uslog_test();    

    return 0;
}
