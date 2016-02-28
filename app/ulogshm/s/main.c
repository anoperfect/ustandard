#include "ustandard/ustandard.h"
#define __log(fmt...)                                                       \
do {                                                                        \
    struct ulogshms_info info;                                              \
    memset(&info, 0, sizeof(info));                                         \
    info.file = __FILE__;                                                   \
    info.function = __FUNCTION__;                                           \
    info.line = __LINE__;                                                   \
    ulogshms_add_vprintf(ks, &info, fmt);                                   \
} while(0)
struct ulogshms* ks = NULL;
int main(int argc, char* const argv[])
{
    key_t key = ftok("/tmp", 'x');
    size_t size = 1024*1024;
    ks = ulogshms_create(key, size, 1);
    assert(NULL != ks);

    int idx = 0;
    while(1) {
        __log("logmsg %d 0\n", idx);
        __log("logmsg %d 1\n", idx);
        idx ++;
        usleep(urandft(10, 1000000));
    }

    return 0;
}
//static int _ulogshms_getopt(int argc, char* const argv[]);
