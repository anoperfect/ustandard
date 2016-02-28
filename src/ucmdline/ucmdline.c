#include "ustandard/ustandard.h"
#include "ustandard/udevelop.h"
#include "ustandard/ufile.h"
#include "ustandard/ubuffer.h"
#include "ustandard/ucmdline.h"
static int kargc = 0;
#define NUM_ARG     128
static const char* kargv[NUM_ARG] = {NULL};
int ucmdline_set(int argc, const char* argv[])
{
    int ret = 0;

    kargc = 0;
    if(argc > 0 && NULL != argv) {
        int i = 0;
        for(i=0; i<argc && i<NUM_ARG; i++) {
            if(NULL != argv[i]) {
                kargv[i] = argv[i];
                kargc ++;
            }
            else {
                break;
            }
        }
    }
    else {
        ret = -1;
    }

    return ret;
}


#define LEN_CMDLINE 4096
static char kcmdline[LEN_CMDLINE];
int _ucmdline_parse(void)
{
    int ret = 0;

    pid_t pid = getpid();
#define LEN_PATH    1024
    char path[LEN_PATH];

    snprintf(path, LEN_PATH, "/proc/%d/cmdline", pid);
    size_t size = ufile_copy_to_buffer(path, kcmdline, LEN_CMDLINE-1);
    struct ubuffer_data data[NUM_ARG];
    kargc = ubuffer_split_by_char(kcmdline, size, '\0', data, NUM_ARG);
    if(kargc > 0) {
        int i;
        for(i=0; i<kargc; i++) {
            kargv[i] = data[i].ptr;
        }
    }
    else {
        ret = -1;
    }

    return ret;
}


const char* ucmdline_get_name(void)
{
    const char* retname = NULL;
    if(0 == kargc) {
        _ucmdline_parse();
    }

    if(kargc > 0) {
        const char* tmp = strrchr(kargv[0], '/');
        retname = NULL != tmp ? tmp+1 : kargv[0];
    }

    return retname;
}


int ucmdline_get_argc(void)
{
    int retn = 0;

    if(0 == kargc) {
        _ucmdline_parse();
    }

    retn = kargc;

    return retn;
}


const char* ucmdline_get_argv(int idx)
{
    const char* retv = 0;

    if(idx > 0 && idx < kargc) {
        retv = kargv[idx];
    }

    return retv;
}
