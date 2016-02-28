#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
void test_ustrtrim(void)
{
    #define LEN 1023
    char bm[LEN+1];
    char str[LEN+1];
    char str0[LEN+1];
    memset(bm, 1, LEN+1);
    memset(str, 10, LEN+1);
    memset(str0, 0, LEN+1);

    const char* s = NULL;
    const char* st = NULL;
    size_t offset_bm = 0;
    bm[LEN] = 'x';
    str[LEN] = 'd';
    char* tmp ;

    tmp = ustrtrim(NULL);
    __assert(NULL == tmp);
    __assert(errno == EINVAL);

    memcpy(str, bm, LEN);
    s = "";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert('\0' == str[10]);
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim(str+10);
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert('\0' == str[10]);
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "abc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim(str+10);
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');
    
    memcpy(str, bm, LEN);
    s = "   abc109";
    st = "abc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim(str+10);
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, 3));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "abc109  \t";
    st = "abc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim(str+10);
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, 3));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "    abc109\t  ";
    st = "abc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim(str+10);
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, 7));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = " \t   \t";
    st = "";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim(str+10);
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, 6));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

}


void test_ustrtrim_chars(void)
{
    #define LEN 1023
    char bm[LEN+1];
    char str[LEN+1];
    char str0[LEN+1];
    memset(bm, 1, LEN+1);
    memset(str, 10, LEN+1);
    memset(str0, 0, LEN+1);

    const char* s = NULL;
    const char* st = NULL;
    size_t offset_bm = 0;
    size_t size0 = 0;
    bm[LEN] = 'x';
    str[LEN] = 'd';
    char* tmp ;
    char chars[] = {' ', '\t', 'x'};
    tmp = ustrtrim_chars(NULL, 1, 1, chars, sizeof(chars));
    __assert(NULL == tmp);
    __assert(errno == EINVAL);

    memcpy(str, bm, LEN);
    s = "";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert('\0' == str[10]);
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 1, 1, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert('\0' == str[10]);
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "abc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 1, 1, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "xbc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 1, 1, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s+1, strlen(s)));
    __assert('\0' == str[16]);
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "   abc109";
    st = "abc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 1, 1, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, 3));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "abc109  \t";
    st = "abc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 1, 1, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, 3));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    //
    memcpy(str, bm, LEN);
    s = "    abc109\t  ";
    st = "    abc109\t  ";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    size0 = strlen(s)-strlen(st);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 0, 0, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, size0));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "    abc109\t  ";
    st = "    abc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    size0 = strlen(s)-strlen(st);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 0, 1, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, size0));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "    abc109\t  ";
    st = "abc109\t  ";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    size0 = strlen(s)-strlen(st);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 1, 0, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, size0));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = "    abc109\t  ";
    st = "abc109";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    size0 = strlen(s)-strlen(st);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 1, 1, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, size0));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = " \t   \t";
    st = "";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 0, 1, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, 6));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = " \t   \t";
    st = "";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 1, 0, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, 6));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    memcpy(str, bm, LEN);
    s = " \t   \t";
    st = "";
    strcpy(str+10, s);
    offset_bm = 10 + strlen(s) + 1;
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, s, strlen(s)+1));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    ustrtrim_chars(str+10, 1, 1, chars, sizeof(chars));
    __assert(errno == 0);
    __assert(0 == memcmp(str, bm, 10));
    __assert(0 == memcmp(str+10, st, strlen(st)+1));
    __assert(0 == memcmp(str+10+strlen(st)+1, str0, 6));
    __assert(0 == memcmp(str+offset_bm, bm+offset_bm, LEN-offset_bm));
    __assert(bm[LEN] == 'x' && str[LEN] == 'd');

    return ;
}


int main()
{
    ustandard_test();

    test_ustrtrim();
    test_ustrtrim_chars();

    return 0;
}
