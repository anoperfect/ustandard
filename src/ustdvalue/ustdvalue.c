#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ustdvalue.h"

int kstd_pid;







int ustdvalue_init(void)
{
    int ret = 0;
    kstd_pid = getpid();



    return ret;
}
