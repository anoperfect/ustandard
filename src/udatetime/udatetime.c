#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/udatetime.h"
#define LEN_STRING_TIME     100
char kstring_datetime[LEN_STRING_TIME];

const char* udatetime_to_string(udatetime_format_e format)
{ udatetime_to_string_r(format, kstring_datetime, LEN_STRING_TIME);
    return kstring_datetime;
}


void udatetime_to_string_r(udatetime_format_e format, char* str, size_t size)
{
    if(NULL == str) {
        return;
    }

    time_t sec;
    struct tm* p;
    time(&sec);
    p = localtime(&sec);
    
    switch(format)
    {
        case udatetime_format_yyyymmdd_e:
            snprintf(str, size, "%4d-%02d-%02d", 
                    1900+p->tm_year, 1+p->tm_mon, p->tm_mday);
        break;
            
        case udatetime_format_hhmmss_e:
            snprintf(str, size, "%02d:%02d:%02d", 
                    p->tm_hour, p->tm_min, p->tm_sec);
        break;

        case udatetime_format_yyyymmdd_hhmmss_e:
            snprintf(str, size, "%4d-%02d-%02d %02d:%02d:%02d", 
                    1900+p->tm_year, 1+p->tm_mon, p->tm_mday,
                    p->tm_hour, p->tm_min, p->tm_sec);
        break;

        default:
            snprintf(str, size, "---not-define---");
        break;
    }
}


int ustrtime_tm(char* s, size_t max, const char* format, struct tm* t)
{
    int ret = 0;
    format = (NULL!=format)?format:"%F %T";
    ret = strftime(s, max, format, t);
    return ret;
}


int ustrtime_now(char* s, size_t max, const char* format)
{
    int ret = 0;

    time_t sec = time(NULL);
    struct tm t;
    memcpy(&t, localtime(&sec), sizeof(struct tm));

#if 0
    localtime_r(&sec, &t);

    this is another function to get tm, but if the timezone changed, 
    the value t is different from the methed
        [memcpy(&t, localtime(&sec), sizeof(t));],
    and the value is wrong.
#endif

    ret = ustrtime_tm(s, max, format, &t);

    return ret;
}

