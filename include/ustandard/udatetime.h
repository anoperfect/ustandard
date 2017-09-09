#ifndef __UDATETIME_H__
#define __UDATETIME_H__
__BEGIN_DECLS


extern char kstring_datetime[];

typedef enum{
    udatetime_format_none                   = 0,
    udatetime_format_yyyymmdd_e,
    udatetime_format_hhmmss_e,
    udatetime_format_yyyymmdd_hhmmss_e,
}udatetime_format_e;

const char* udatetime_to_string(udatetime_format_e format);
void udatetime_to_string_r(udatetime_format_e format, char* str, size_t size);

int ustrtime_tm(char* s, size_t max, const char* format, struct tm* t);
int ustrtime_now(char* s, size_t max, const char* format);

__END_DECLS
#endif//udatetime.h
