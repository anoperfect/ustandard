#include "ustandard/ustandard.h"
int main()
{
    #define LEN 10 
    unsigned char value[] = "x1234567890";
    char id[LEN];

    memset(id, 'a', LEN);
    um_strcpy(id, LEN, (const char*)value);
    printf("id=%s.\n", id);

    memset(id, 'a', LEN);
    um_strncpy(id, LEN, (const char*)value, 1);
    printf("id=%s.\n", id);

    return 0;
}
