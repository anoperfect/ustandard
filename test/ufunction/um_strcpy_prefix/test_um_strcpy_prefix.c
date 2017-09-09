#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ustandard/ufunction.h"


int main()
{
    #define LEN 8
    char dest[LEN];
    dest[LEN-1] = 100;
    um_strcpy_prefix(dest, LEN-1, "1");
    printf("dest = [%s], border=%d.\n", dest, dest[LEN-1]);

    um_strcpy_prefix(dest, LEN-1, "12");
    printf("dest = [%s], border=%d.\n", dest, dest[LEN-1]);

    um_strcpy_prefix(dest, LEN-1, "1234567890");
    printf("dest = [%s], border=%d.\n", dest, dest[LEN-1]);

    return 0;
}
