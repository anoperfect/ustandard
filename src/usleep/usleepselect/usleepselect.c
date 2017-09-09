#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/usleepselect.h"


int usleepselect(int usec)
{
    struct timeval tv;
    tv.tv_sec =     usec / 1000000;
    tv.tv_usec =    usec % 1000000;

    int ret = select(0, NULL, NULL, NULL, &tv);
    if(0 != ret) {
        ulogerr("usleepselect : %s.\n", "select error");
        return -1;
    }

    return 0;
}
