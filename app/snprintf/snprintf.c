#include "ustandard/ustandard.h"
int main()
{
    char tmp[10];
    int n = snprintf(tmp, 10, "%s", "111");
    uloginf("n=%d\n", n);

    n = snprintf(tmp, 10, "%s", "11111111111111111111");
    uloginf("n=%d\n", n);

    return 0;
}
