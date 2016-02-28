#include "ustandard/ustandard.h"
#include "ustandard_test.inc.c"
void test_strsplit(void)
{
#define LEN 1024
    char src[LEN];
    char split[LEN];

#define LEN_DEST1    10
#define LEN_DEST2    18
#define LEN_DEST3    20
#define NUM_DEST     3
    char dest1[LEN_DEST1];
    char dest2[LEN_DEST2];
    char dest3[LEN_DEST3];
    size_t size_dests[NUM_DEST] = {LEN_DEST1, LEN_DEST2, LEN_DEST3};
    char* dests[NUM_DEST] = {dest1, dest2, dest3};
    int n;

    char bm_dest1[LEN_DEST1];
    char bm_dest2[LEN_DEST2];
    char bm_dest3[LEN_DEST3];
    size_t bm_size_dests[NUM_DEST] = {LEN_DEST1, LEN_DEST2, LEN_DEST3};
    char* bm_dests[NUM_DEST] = {bm_dest1, bm_dest2, bm_dest3};

    int i;
    for(i=0; i<NUM_DEST; i++) {
        memset(dests[i], 101, size_dests[i]);
        memset(bm_dests[i], 101, bm_size_dests[i]);
    }

//EINVAL
    n = ustrsplit(NULL, "1", NUM_DEST, dests, size_dests);
    __assert(0 == n);
    __assert(errno == EINVAL);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "123");
    n = ustrsplit(src, NULL, NUM_DEST, dests, size_dests);
    __assert(0 == n);
    __assert(errno == EINVAL);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "123");
    n = ustrsplit(src, "", NUM_DEST, dests, size_dests);
    __assert(0 == n);
    __assert(errno == EINVAL);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "123");
    n = ustrsplit(src, "1", 0, dests, size_dests);
    __assert(0 == n);
    __assert(errno == EINVAL);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "123");
    n = ustrsplit(src, "1", -1, dests, size_dests);
    __assert(0 == n);
    __assert(errno == EINVAL);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }
    
    ustrcpy(src, LEN, "123");
    n = ustrsplit(src, "1", 0, dests, size_dests); 
    __assert(0 == n);
    __assert(errno == EINVAL);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }
    
    ustrcpy(src, LEN, "123");
    n = ustrsplit(src, "1", NUM_DEST, NULL, size_dests);
    __assert(0 == n);
    __assert(errno == EINVAL);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }
    
    ustrcpy(src, LEN, "123");
    n = ustrsplit(src, "1", NUM_DEST, dests, NULL);
    __assert(0 == n);
    __assert(errno == EINVAL);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

//n=0
    ustrcpy(src, LEN, "");
    ustrcpy(split, LEN, "x");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(0 == n);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "");
    ustrcpy(split, LEN, "");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(0 == n);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "x");
    ustrcpy(split, LEN, "x");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(0 == n);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "xxxxxxxxxxxxxxxxxxxxxxxx");
    ustrcpy(split, LEN, "x");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(0 == n);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "123abc");
    ustrcpy(split, LEN, "123abc");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(0 == n);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "123abc123abc");
    ustrcpy(split, LEN, "123abc");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(0 == n);
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=0; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

//n=1
    ustrcpy(src, LEN, "123abc");
    ustrcpy(split, LEN, "123");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(1 == n);
    __assert(0 == strcmp(dests[0], "abc"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }
    
    ustrcpy(src, LEN, "123abc");
    ustrcpy(split, LEN, "c");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(1 == n);
    __assert(0 == strcmp(dests[0], "123ab"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "123abcdefghijklmnopqrstuvw");
    ustrcpy(split, LEN, "123");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(1 == n);
    __assert(0 == strcmp(dests[0], "abcdefghi"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

//n=2
    ustrcpy(src, LEN, "123abcdefghijklmnopqrstuvw");
    ustrcpy(split, LEN, "v");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(2 == n);
    __assert(0 == strcmp(dests[0], "123abcdef"));
    __assert(0 == strcmp(dests[1], "w"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "123abcdefghijklmnopqrstuvwh1");
    ustrcpy(split, LEN, "g");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(2 == n);
    __assert(0 == strcmp(dests[0], "123abcdef"));
    __assert(0 == strcmp(dests[1], "hijklmnopqrstuvwh"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "xxxxxxxxxxxxxxxxxxxxxxxxx123xxxxxxxxxxxxxxxxxxxxxxxxxdef");
    ustrcpy(split, LEN, "xxxxxxxxxxxxxxxxxxxxxxxx");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(2 == n);
    __assert(0 == strcmp(dests[0], "x123"));
    __assert(0 == strcmp(dests[1], "xdef"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "x123xxxdefxx");
    ustrcpy(split, LEN, "xx");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(2 == n);
    __assert(0 == strcmp(dests[0], "x123"));
    __assert(0 == strcmp(dests[1], "xdef"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "xxx123xxxdefxx");
    ustrcpy(split, LEN, "xx");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(2 == n);
    __assert(0 == strcmp(dests[0], "x123"));
    __assert(0 == strcmp(dests[1], "xdef"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

//n=3
    ustrcpy(src, LEN, "121113411156");
    ustrcpy(split, LEN, "111");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(3 == n);
    __assert(0 == strcmp(dests[0], "12"));
    __assert(0 == strcmp(dests[1], "34"));
    __assert(0 == strcmp(dests[2], "56"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "111121113411156");
    ustrcpy(split, LEN, "111");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(3 == n);
    __assert(0 == strcmp(dests[0], "12"));
    __assert(0 == strcmp(dests[1], "34"));
    __assert(0 == strcmp(dests[2], "56"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "121113411156111");
    ustrcpy(split, LEN, "111");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(3 == n);
    __assert(0 == strcmp(dests[0], "12"));
    __assert(0 == strcmp(dests[1], "34"));
    __assert(0 == strcmp(dests[2], "56"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "111121113411156111");
    ustrcpy(split, LEN, "111");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(3 == n);
    __assert(0 == strcmp(dests[0], "12"));
    __assert(0 == strcmp(dests[1], "34"));
    __assert(0 == strcmp(dests[2], "56"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

//n=4
    __assert(1);
    ustrcpy(src, LEN, "aacbcacdefacghij");
    ustrcpy(split, LEN, "ac");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(3 == n);
    __assert(0 == strcmp(dests[0], "a"));
    __assert(0 == strcmp(dests[1], "bc"));
    __assert(0 == strcmp(dests[2], "def"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "acaacbcacdefacghij");
    ustrcpy(split, LEN, "ac");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(3 == n);
    __assert(0 == strcmp(dests[0], "a"));
    __assert(0 == strcmp(dests[1], "bc"));
    __assert(0 == strcmp(dests[2], "def"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "aacbcacdefacghijac");
    ustrcpy(split, LEN, "ac");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(3 == n);
    __assert(0 == strcmp(dests[0], "a"));
    __assert(0 == strcmp(dests[1], "bc"));
    __assert(0 == strcmp(dests[2], "def"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

    ustrcpy(src, LEN, "acaacbcacdefacghijac");
    ustrcpy(split, LEN, "ac");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(3 == n);
    __assert(0 == strcmp(dests[0], "a"));
    __assert(0 == strcmp(dests[1], "bc"));
    __assert(0 == strcmp(dests[2], "def"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }

//n>4
    ustrcpy(src, LEN, "stdout,stderr,/dev/null,fp,shared memory,...");
    ustrcpy(split, LEN, ",");
    n = ustrsplit(src, split, NUM_DEST, dests, size_dests);
    __assert(3 == n);
    __assert(0 == strcmp(dests[0], "stdout"));
    __assert(0 == strcmp(dests[1], "stderr"));
    __assert(0 == strcmp(dests[2], "/dev/null"));
    __assert(0 == memcmp(bm_size_dests, size_dests, sizeof(size_dests)));
    for(i=n; i<NUM_DEST; i++) {
        __assert(0 == memcmp(bm_dests[i], dests[i], size_dests[i]));
    }




}


int main()
{
    ustandard_test();

    test_strsplit();
    return 0;
}
