#include "ustandard/ustatic/_ustatic_s.h"
#include <time.h>
static size_t _udatetime_to_string(char* dest, size_t size, const char* format)
{
    size_t ret_size = 0;

    struct tm t;
    time_t sec = time(NULL);
    memcpy(&t, localtime(&sec), sizeof(t));
    //localtime_r(&sec, &t);
   
    if(NULL == format) {
        format = "%T";
    }
    ret_size = strftime(dest, size, format, &t);

    return ret_size;
}
