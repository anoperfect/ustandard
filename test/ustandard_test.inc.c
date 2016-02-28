int ustandard_test(void)
{
    int ret = 0;
    uslog_sys_set(__vt, stdout);
    return ret;
}


#define __assert(x) assert(x); if(x) { ulogtst("test OK.\n"); } else { assert(x); }
