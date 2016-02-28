#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
void test_umemmove_left(void)
{
    #define LEN     10
    char s[LEN] = "012345678";
    char* tmp = umemmove_left(s, LEN, 6);
    __assert(s == tmp);
    __assert(s[0] == '4');
    __assert(s[1] == '5');
    __assert(s[2] == '6');
    __assert(s[3] == '7');
    __assert(s[4] == '8');
    __assert(s[5] == '\0');
    __assert(s[6] == '6');
    __assert(s[7] == '7');
    __assert(s[8] == '8');
    __assert(s[9] == '\0');
}


void test_umemmove_right(void)
{
    #define LEN     10
    char s0[LEN] = "01";
    char s[LEN] = "01";
    char* tmp = NULL;

    memcpy(s, s0, 3);
    tmp = umemmove_right(s, 0, 1);
    __assert(s == tmp);
    __assert(s[0] == '0');
    __assert(s[1] == '1');
    __assert(s[2] == '\0');

    memcpy(s, s0, 3);
    tmp = umemmove_right(s, LEN, 0);
    __assert(s == tmp);
    __assert(s[0] == '0');
    __assert(s[1] == '1');
    __assert(s[2] == '\0');

    memcpy(s, s0, 3);
    tmp = umemmove_right(s, 3, 1);
    __assert(s == tmp);
    __assert(s[0] == '0');
    __assert(s[1] == '0');
    __assert(s[2] == '1');
    __assert(s[3] == '\0');

    memcpy(s, s0, 3);
    tmp = umemmove_right(s, 3, 2);
    __assert(s == tmp);
    __assert(s[0] == '0');
    __assert(s[1] == '1');
    __assert(s[2] == '0');
    __assert(s[3] == '1');
    __assert(s[4] == '\0');
    
    memcpy(s, s0, 3);
    tmp = umemmove_right(s, 3, 3);
    __assert(s == tmp);
    __assert(s[0] == '0');
    __assert(s[1] == '1');
    __assert(s[2] == '\0');
    __assert(s[3] == '0');
    __assert(s[4] == '1');
    __assert(s[5] == '\0');
    
    memcpy(s, s0, 3);
    tmp = umemmove_right(s, 3, 4);
    __assert(s == tmp);
    __assert(s[0] == '0');
    __assert(s[1] == '1');
    __assert(s[2] == '\0');
    __assert(s[4] == '0');
    __assert(s[5] == '1');
    __assert(s[6] == '\0');
    
    memcpy(s, s0, 3);
    tmp = umemmove_right(s, 3, 7);
    __assert(s == tmp);
    __assert(s[0] == '0');
    __assert(s[1] == '1');
    __assert(s[2] == '\0');
    __assert(s[7] == '0');
    __assert(s[8] == '1');
    __assert(s[9] == '\0');
}



int main()
{
    ustandard_test();

    test_umemmove_left();
    test_umemmove_right();

    return 0;
}
