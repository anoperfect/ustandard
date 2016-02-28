#include "ustandard/ustandard.h"




int main()
{
    printf("1 : %s\n", udatetime_to_string(udatetime_format_yyyymmdd_e));
    printf("2 : %s\n", udatetime_to_string(udatetime_format_hhmmss_e));
    printf("3 : %s\n", udatetime_to_string(udatetime_format_yyyymmdd_hhmmss_e));

    char str1[100];
    size_t size1 = 100;
    udatetime_to_string_r(udatetime_format_yyyymmdd_e, str1, size1);
    printf("1r: %s\n", str1);
    udatetime_to_string_r(udatetime_format_hhmmss_e, str1, size1);
    printf("2r: %s\n", str1);
    udatetime_to_string_r(udatetime_format_yyyymmdd_hhmmss_e, str1, size1);
    printf("3r: %s\n", str1);
    
    char str2[6];
    size_t size2 = 6;
    udatetime_to_string_r(udatetime_format_yyyymmdd_e, str2, size2);
    printf("1r: %s\n", str2);
    udatetime_to_string_r(udatetime_format_hhmmss_e, str2, size2);
    printf("2r: %s\n", str2);
    udatetime_to_string_r(udatetime_format_yyyymmdd_hhmmss_e, str2, size2);
    printf("3r: %s\n", str2);

    return 0;
}
