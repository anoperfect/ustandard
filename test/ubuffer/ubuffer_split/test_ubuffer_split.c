#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
int test_ubuffer_split_by_char(void)
{
    int ret = 0;

    #define LEN_STR     1024
    #define NUM 1024
    struct ubuffer_data data[NUM];
    int n;
    ret = -1;
    do {
        char str[LEN_STR];
        int i;
        for(i=0; i<LEN_STR; i++) {
            str[i] = i % 128;
        }

        int ch;
        for(ch=0; ch<128; ch++) {
            n = ubuffer_split_by_char(str, LEN_STR, ch, data, NUM);
            //ulogtst("ch%-3d n=%d.\n", ch, n);
            if(ch == 0 || ch == 128-1) {
                assert(n == 8);
            }
            else {
                assert(n == 9);
            }
        }

        um_strcpy(str, LEN_STR, "01234567890123456789");
        for(ch='0'; ch<='9'; ch++) {
            n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
            //ulogtst("ch%c n=%d.\n", ch, n);
            if(ch == '0' || ch == '9') {
                assert(n == 2);
            }
            else {
                assert(n == 3);
            }
        }

        const char* bm[100];
        /*000*/
        um_strcpy(str, LEN_STR, "aabcaaaadefgaaaaaaa");
        ch = 'h';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(1 == n);
        bm[0] = "aabcaaaadefgaaaaaaa";
        for(i=0; i<n; i++) {
            assert(data[i].size == strlen(bm[i]));
            assert(0 == memcmp(data[i].ptr, bm[i], data[i].size));
        }

        /*001*/
        um_strcpy(str, LEN_STR, "aabcaaaadefgg");
        ch = 'g';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(1 == n);
        bm[0] = "aabcaaaadef";
        for(i=0; i<n; i++) {
            assert(data[i].size == strlen(bm[i]));
            assert(0 == memcmp(data[i].ptr, bm[i], data[i].size));
        }

        /*010*/
        um_strcpy(str, LEN_STR, "axbbbbbbcaaaadefg");
        ch = 'b';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(2 == n);
        bm[0] = "ax";
        bm[1] = "caaaadefg";
        for(i=0; i<n; i++) {
            assert(data[i].size == strlen(bm[i]));
            assert(0 == memcmp(data[i].ptr, bm[i], data[i].size));
        }

        /*100*/
        um_strcpy(str, LEN_STR, "xxxxxxaabcaaaadefg");
        ch = 'x';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(1 == n);
        bm[0] = "aabcaaaadefg";
        for(i=0; i<n; i++) {
            assert(data[i].size == strlen(bm[i]));
            assert(0 == memcmp(data[i].ptr, bm[i], data[i].size));
        }

        /*011*/
        um_strcpy(str, LEN_STR, "adbbbbbbcaxx");
        ch = 'x';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(1 == n);
        bm[0] = "adbbbbbbca";
        for(i=0; i<n; i++) {
            assert(data[i].size == strlen(bm[i]));
            assert(0 == memcmp(data[i].ptr, bm[i], data[i].size));
        }

        /*101*/
        um_strcpy(str, LEN_STR, "padbbbbbbcaxxaapp");
        ch = 'p';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(1 == n);
        bm[0] = "adbbbbbbcaxxaa";
        for(i=0; i<n; i++) {
            assert(data[i].size == strlen(bm[i]));
            assert(0 == memcmp(data[i].ptr, bm[i], data[i].size));
        }

        /*110*/
        um_strcpy(str, LEN_STR, "xxxaabcaaaadefgaaaaaaa");
        ch = 'x';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(1 == n);
        bm[0] = "aabcaaaadefgaaaaaaa";
        for(i=0; i<n; i++) {
            assert(data[i].size == strlen(bm[i]));
            assert(0 == memcmp(data[i].ptr, bm[i], data[i].size));
        }

        /*111*/
        um_strcpy(str, LEN_STR, "qqq");
        ch = 'q';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(0 == n);

        /*n*/
        um_strcpy(str, LEN_STR, "axpbbbbbbcaaaadefg");
        ch = 'b';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, 1);
        assert(1 == n);
        bm[0] = "axp";
        for(i=0; i<n; i++) {
            assert(data[i].size == strlen(bm[i]));
            assert(0 == memcmp(data[i].ptr, bm[i], data[i].size));
        }

        um_strcpy(str, LEN_STR, "axbbbbbbcaaaadefg");
        ch = 'b';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, 0);
        assert(0 == n);

        um_strcpy(str, LEN_STR, "");
        ch = 'a';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(0 == n);

        um_strcpy(str, LEN_STR, "a");
        ch = 'a';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(0 == n);

        um_strcpy(str, LEN_STR, "aa");
        ch = 'a';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(0 == n);

        um_strcpy(str, LEN_STR, "ac");
        ch = 'b';
        n = ubuffer_split_by_char(str, strlen(str), ch, data, NUM);
        assert(1 == n);
        assert(data[0].size == strlen("ac"));
        assert(0 == memcmp(data[0].ptr, "ac", strlen("ac")));

        ret = 0;
    }while(0);


    return ret;
}

int main(int argc, const char* argv[])
{
    ustandard_test();
    int ret = 0;
    ret = test_ubuffer_split_by_char();
    return ret;
}
