#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
int main(int argc, const char* argv[])
{
    int ret = 0;
    ustandard_test();
    ulogtst("name:[%s].\n", ucmdline_get_name());

    return ret;
}
