#include "ustandard/ustandard.h"
int main()
{
    char str1[1024] = {'1', '2', '3', '\r', '\n', '\0'};
    um_strcut_crlf(str1);
    printf("str1=[%s].\n", str1);

    char str2[1024] = {'\r', '\n', '\0'};
    um_strcut_crlf(str2);
    printf("str2=[%s].\n", str2);

    char str3[1024] = {'1', '2', '3', '\n', '\0'};
    um_strcut_crlf(str3);
    printf("str3=[%s].\n", str3);

    char str4[1024] = {'\n', '\0'};
    um_strcut_crlf(str4);
    printf("str4=[%s].\n", str4);

    return 0;
};
