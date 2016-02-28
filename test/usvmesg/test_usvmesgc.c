#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
int main()
{
    #define LEN_STR 1024
    char str[LEN_STR];
    int n;
    int ret;
    while(1) {
        n = read(0, str, LEN_STR-1);
        if(n>0) {
            ret = usvmesgc_send("test_client", "test_server", str, n);
            ulogtst("%s\n", 0 == ret?"send OK":"send failed");
        }
    }

    return 0;
}
