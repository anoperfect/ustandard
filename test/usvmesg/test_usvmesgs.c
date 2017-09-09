#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
int main()
{
    __ptr_t s = usvmesgs_register("test_server", 1024*1024);    
    assert(0 != s);

    struct umesg_recv_data data;
    int ret = 0;
    while(1) {
        ret = usvmesgs_recv(s, &data, NULL);
        if(0 == ret) {
            fprintf(stdout, "recv message from : %s \n", data.sender);
            fwrite(data.ptr, 1, data.size, stdout);
            fprintf(stdout, "\n");
        }
    }

    return 0;
}
