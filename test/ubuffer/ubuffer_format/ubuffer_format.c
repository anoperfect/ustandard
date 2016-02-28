#include "ustandard/ustandard.h"
int main(int argc, const char* argv[])
{
    unsigned char ptr[1024];
    int i;
    for(i=0; i<1024; i++) {
        ptr[i] = i%256;
    }

    #define LEN_STR (1024*10)
    char str[LEN_STR];


    /*-----------------------------------------------------------------------*/
    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 1, e_ubuffer_char_type_default, ' ', 16);
    printf("%s\n%s\n", "default1", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 64, e_ubuffer_char_type_default, ' ', 16);
    printf("%s\n%s\n", "default64", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 600, e_ubuffer_char_type_default, ' ', 16);
    printf("%s\n%s\n", "default600", str);

    /*-----------------------------------------------------------------------*/
    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 1, e_ubuffer_char_type_0x, ' ', 16);
    printf("%s\n%s\n", "0x1", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 64, e_ubuffer_char_type_0x, ' ', 16);
    printf("%s\n%s\n", "0x64", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 600, e_ubuffer_char_type_0x, ' ', 16);
    printf("%s\n%s\n", "0x600", str);

    /*-----------------------------------------------------------------------*/
    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 1, e_ubuffer_char_type_0X, ' ', 16);
    printf("%s\n%s\n", "0X1", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 64, e_ubuffer_char_type_0X, ' ', 16);
    printf("%s\n%s\n", "0X64", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 600, e_ubuffer_char_type_0X, ' ', 16);
    printf("%s\n%s\n", "0X600", str);
    
    /*-----------------------------------------------------------------------*/
    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 1, e_ubuffer_char_type_0xprint, ' ', 16);
    printf("%s\n%s\n", "0xprint1", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 64, e_ubuffer_char_type_0xprint, ' ', 16);
    printf("%s\n%s\n", "0xprint64", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 600, e_ubuffer_char_type_0xprint, ' ', 16);
    printf("%s\n%s\n", "0xprint600", str);

    /*-----------------------------------------------------------------------*/
    for(i=0; i<1024; i++) {
        if(!isprint(ptr[i])) {
            ptr[i] = 'x';
        }
    }

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 1, e_ubuffer_char_type_print, ' ', 16);
    printf("%s\n%s\n", "print1", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 64, e_ubuffer_char_type_print, ' ', 16);
    printf("%s\n%s\n", "print64", str);

    memset(str, 100, LEN_STR);
    ubuffer_format(str, LEN_STR, ptr, 600, e_ubuffer_char_type_print, ' ', 16);
    printf("%s\n%s\n", "print600", str);

    return 0;
}
