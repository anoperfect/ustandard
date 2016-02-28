#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/uverbose.h"


FILE* kv_debug = NULL;
FILE* kv_info = NULL;
FILE* kv_error = NULL;











void uverbose_set(const char* level, FILE* fp)
{
    if(0 == strcmp(level, "debug")) {
        kv_debug = fp;
        return ;
    }

    if(0 == strcmp(level, "info")) {
        kv_info = fp;
        return ;
    }

    if(0 == strcmp(level, "error")) {
        kv_error = fp;
        return ;
    }
}
