#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
void test_strcpy(void)
{
    #define LEN     10
    char bm[LEN];
    char str0[LEN];
    memset(bm, 'x', LEN);
    memset(str0, 0, LEN);
    char dest[LEN+2];
    char* tmp = NULL;

    char src[2*LEN];

    memcpy(dest, bm, LEN); dest[LEN] = 100; dest[LEN+1] = 101;
    tmp = ustrcpy(NULL, LEN, src);
    __assert(errno == EINVAL);
    __assert(NULL == tmp);

    tmp = ustrcpy(dest, 0, src);
    __assert(errno == EINVAL);
    __assert(NULL == tmp);
    __assert(0 == memcmp(dest, bm, LEN));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    tmp = ustrcpy(dest, LEN, NULL);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, str0, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    src[0] = '\0';
    src[1] = 10;
    tmp = ustrcpy(dest, LEN, src);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, str0, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-2);
    tmp = ustrcpy(dest, 1, src);
    __assert(errno == E2BIG);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, bm+1, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    src[0] = 'a';
    src[1] = '\0';
    src[2] = 100;
    tmp = ustrcpy(dest, LEN, src);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == 'a');
    __assert(dest[1] == '\0');
    __assert(0 == memcmp(dest+2, str0, LEN-2));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "a234567890123fhsjldjfsjliudgfsi", LEN-2);
    tmp = ustrcpy(dest, 2, src);
    __assert(errno == E2BIG);
    __assert(tmp == dest);
    __assert(dest[0] == 'a');
    __assert(dest[1] == '\0');
    __assert(0 == memcmp(dest+2, bm, LEN-2));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-2);
    __assert(strlen(src) == LEN-2);
    tmp = ustrcpy(dest, LEN, src);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-2));
    __assert('\0' == dest[LEN-2]);
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-1);
    __assert(strlen(src) == LEN-1);
    tmp = ustrcpy(dest, LEN, src);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-1));
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN);
    __assert(strlen(src) == LEN);
    tmp = ustrcpy(dest, LEN, src);
    __assert(errno == E2BIG);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-1));
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN+6);
    __assert(strlen(src) == LEN+6);
    tmp = ustrcpy(dest, LEN, src);
    __assert(errno == E2BIG);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-1));
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);


}


void test_strncpy(void)
{
    #define LEN     10
    char bm[LEN];
    char str0[LEN];
    memset(bm, 'x', LEN);
    memset(str0, 0, LEN);
    char dest[LEN+2];
    char* tmp = NULL;

    char src[2*LEN];

    memcpy(dest, bm, LEN); dest[LEN] = 100; dest[LEN+1] = 101;
    tmp = ustrncpy(NULL, LEN, src, LEN);
    __assert(errno == EINVAL);
    __assert(NULL == tmp);
    __assert(0 == memcmp(dest, bm, LEN));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    tmp = ustrncpy(dest, 0, src, LEN);
    __assert(errno == EINVAL);
    __assert(NULL == tmp);
    __assert(0 == memcmp(dest, bm, LEN));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN); dest[LEN] = 100; dest[LEN+1] = 101;
    tmp = ustrncpy(NULL, 0, src, LEN);
    __assert(errno == EINVAL);
    __assert(NULL == tmp);
    __assert(0 == memcmp(dest, bm, LEN));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    tmp = ustrncpy(dest, LEN, NULL, LEN);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, str0, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    tmp = ustrncpy(dest, LEN, NULL, 0);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, str0, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    tmp = ustrncpy(dest, LEN, NULL, 10);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, str0, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    src[0] = '\0';
    src[1] = 10;
    tmp = ustrncpy(dest, LEN, src, 0);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, str0, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    src[0] = '\0';
    src[1] = 10;
    tmp = ustrncpy(dest, LEN, src, 1);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, str0, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    src[0] = '\0';
    src[1] = 10;
    tmp = ustrncpy(dest, LEN, src, 10);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, str0, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-2);
    tmp = ustrncpy(dest, 1, src, 0);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, bm+1, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-2);
    tmp = ustrncpy(dest, 1, src, 1);
    __assert(errno == E2BIG);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, bm+1, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-2);
    tmp = ustrncpy(dest, 1, src, LEN);
    __assert(errno == E2BIG);
    __assert(tmp == dest);
    __assert(dest[0] == '\0');
    __assert(0 == memcmp(dest+1, bm+1, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    src[0] = 'a';
    src[1] = '\0';
    src[2] = 100;
    tmp = ustrncpy(dest, LEN, src, 0);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, str0, LEN));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    src[0] = 'a';
    src[1] = '\0';
    src[2] = 100;
    tmp = ustrncpy(dest, LEN, src, 1);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == 'a');
    __assert(dest[1] == '\0');
    __assert(0 == memcmp(dest+2, str0, LEN-2));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    src[0] = 'a';
    src[1] = '\0';
    src[2] = 100;
    tmp = ustrncpy(dest, LEN, src, 2);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == 'a');
    __assert(dest[1] == '\0');
    __assert(0 == memcmp(dest+2, str0, LEN-2));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "a234567890123fhsjldjfsjliudgfsi", LEN-2);
    tmp = ustrncpy(dest, 2, src, 0);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, str0, 2));
    __assert(0 == memcmp(dest+2, bm+2, LEN-2));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "a234567890123fhsjldjfsjliudgfsi", LEN-2);
    tmp = ustrncpy(dest, 2, src, 1);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(dest[0] == 'a');
    __assert(dest[1] == '\0');
    __assert(0 == memcmp(dest+2, bm, LEN-2));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "a234567890123fhsjldjfsjliudgfsi", LEN-2);
    tmp = ustrncpy(dest, 2, src, 2);
    __assert(errno == E2BIG);
    __assert(tmp == dest);
    __assert(dest[0] == 'a');
    __assert(dest[1] == '\0');
    __assert(0 == memcmp(dest+2, bm, LEN-2));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-2);
    __assert(strlen(src) == LEN-2);
    tmp = ustrncpy(dest, LEN, src, 2);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, 2));
    __assert(0 == memcmp(dest+2, str0, LEN-2));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-2);
    __assert(strlen(src) == LEN-2);
    tmp = ustrncpy(dest, LEN, src, LEN-2);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-2));
    __assert('\0' == dest[LEN-2]);
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-2);
    __assert(strlen(src) == LEN-2);
    tmp = ustrncpy(dest, LEN, src, LEN-1);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-2));
    __assert('\0' == dest[LEN-2]);
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-2);
    __assert(strlen(src) == LEN-2);
    tmp = ustrncpy(dest, LEN, src, LEN);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-2));
    __assert('\0' == dest[LEN-2]);
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-1);
    __assert(strlen(src) == LEN-1);
    tmp = ustrncpy(dest, LEN, src, 3);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, 3));
    __assert(0 == memcmp(dest+3, str0, LEN-3));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-1);
    __assert(strlen(src) == LEN-1);
    tmp = ustrncpy(dest, LEN, src, LEN-1);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-1));
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN-1);
    __assert(strlen(src) == LEN-1);
    tmp = ustrncpy(dest, LEN, src, LEN+10);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-1));
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);


    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN);
    __assert(strlen(src) == LEN);
    tmp = ustrncpy(dest, LEN, src, 6);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, 6));
    __assert(0 == memcmp(dest+6, str0, LEN-6));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN);
    __assert(strlen(src) == LEN);
    tmp = ustrncpy(dest, LEN, src, LEN-1);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-1));
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN);
    __assert(strlen(src) == LEN);
    tmp = ustrncpy(dest, LEN, src, LEN);
    __assert(errno == E2BIG);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-1));
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);


    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN+6);
    __assert(strlen(src) == LEN+6);
    tmp = ustrncpy(dest, LEN, src, 1);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, 1));
    __assert(0 == memcmp(dest+1, str0, LEN-1));
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN+6);
    __assert(strlen(src) == LEN+6);
    tmp = ustrncpy(dest, LEN, src, LEN-1);
    __assert(errno == 0);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-1));
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    memcpy(dest, bm, LEN);
    memset(src, 0, sizeof(src));
    strncpy(src, "1234567890123fhsjldjfsjliudgfsi", LEN+6);
    __assert(strlen(src) == LEN+6);
    tmp = ustrncpy(dest, LEN, src, LEN);
    __assert(errno == E2BIG);
    __assert(tmp == dest);
    __assert(0 == memcmp(dest, src, LEN-1));
    __assert('\0' == dest[LEN-1]);
    __assert(100 == dest[LEN]);
    __assert(101 == dest[LEN+1]);

    return;
}


int main()
{
    ustandard_test();

    test_strcpy();
    test_strncpy();

    return 0;
}
