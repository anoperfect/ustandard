#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
int upid_set(const char* path)
{
    int ret = 0;

    int fd;
    int lock_result;
    fd = open(path, O_RDWR | O_CREAT, 0644);
    if(fd < 0) {
        fprintf(stderr, "Open file failed.\n");
        return -1;
    }

    lock_result = lockf(fd, F_TEST, 0);
    if(lock_result < 0) {
        fprintf(stderr, "lockf error.\n");
        return -1;
    }

    lock_result = lockf(fd, F_LOCK, 0);
    if(lock_result < 0) {
        fprintf(stderr, "lockf error.\n");
        return -1;
    }

    pid_t pid = getpid();
    #define LEN_BUF 60
    char buf[LEN_BUF];    
    snprintf(buf, LEN_BUF, "%d\n", (int)pid);

    size_t size = strlen(buf);
    int n = write(fd, buf, size);
    if(n != size) {
        fprintf(stderr, "write pid error.\n");
        return -1;
    }

    return ret;
}
