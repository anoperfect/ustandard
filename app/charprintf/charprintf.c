#include "ustandard/ustandard.h"




int main()
{
    int idx;
    for(idx=0; idx<256; idx++) {
        char ch = idx;
        if(isspace(ch))
        printf("%3d [%d] : \n[[[%c]]]\n\n\n", idx, isspace(ch), ch);
    }

    char ch;
    ch = '\a';
    printf("ch=%d a\n", ch);

    ch = '\b';
    printf("ch=%d b\n", ch);

    ch = '\f';
    printf("ch=%d f\n", ch);

    ch = '\n';
    printf("ch=%d n\n", ch);

    ch = '\r';
    printf("ch=%d r\n", ch);

    ch = '\t';
    printf("ch=%d t\n", ch);

    ch = '\v';
    printf("ch=%d v\n", ch);



    return 0;
}
