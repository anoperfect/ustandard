#include "ustandard/ustandard.h"
struct ump_simple_node {
    size_t offset;
    size_t size_alloc;

    size_t size_free;
};
#define LEN         (1024*1024)
#define LEN_BORDER  4
#define LEN_UMP     28
#define LEN_NODES   10476
extern int ump_sumple_get_node_number(struct ump_simple* ump);

extern int ump_sumple_get_node(struct ump_simple* ump, 
        int idx, int num, struct ump_simple_node* nodes);

int ktest = 0;
#define NUM     800
void* kptrs[NUM];

#define LEN_STR (1024*1024)
char kstr[LEN_STR];

struct ump_simple* kump= NULL;


struct ump_simple_node knodes[1024];
#define NODE_ASSERT(idx, oset, s_alloc, s_free)                                     \
do{                                                                                 \
    assert( knodes[idx].offset == (oset) );                                         \
    assert( knodes[idx].size_alloc == (s_alloc) );                                  \
    assert( knodes[idx].size_free == (s_free) );                                    \
}while(0);


#define NODES_ASSERT_ON_EMPTY                                                       \
do{                                                                                 \
    int knum = ump_sumple_get_node_number(kump);                                    \
    assert(1 == knum);                                                              \
    int knum_get = ump_sumple_get_node(kump, 0, knum, knodes);                      \
    assert(knum_get == knum);                                                       \
    NODE_ASSERT(0, 0, 0, (LEN - LEN_UMP - LEN_NODES))                               \
    ulogtst("NODES_ASSERT_ON_EMPTY OK[%d].\n", ktest);                            \
}while(0);


int ptrs_num()
{
    int retn = 0;
    int i;
    for(i=0; i<NUM; i++) {
        if(NULL != kptrs[i]) {
            retn ++;
        }
    }
    return retn;
}


int num_rand(int num)
{
    return urandft(0, num);
}

#define NUM_ALLOC(n)    num_rand(n)
#define NUM_FREE(n)     num_rand(n)

#define SIZE_ALLOC(n)  urandft(1, LEN/(n)*2)


int select_free_idx(int num)
{
    int sn = urandft(0, num-1);
    int t = 0;
    int idx;
    for(idx=0; idx<NUM; idx++) {
        if(NULL != kptrs[idx]) {
            if(t == sn) {
                break;
            }
            else {
                t ++;
            }
        }
    }
    assert(t == sn);

    return idx;
}


void test_alloc(void)
{
    ulogtst("\n\n[%d]\n", ktest ++);
    int num = ptrs_num();
    int num_null = NUM - num;
    int num_alloc = NUM_ALLOC(num_null);
    int num_add = 0;
    ulogtst("test_alloc-----[num = %3d, num_alloc=%d]------------------------------------------.\n", num, num_alloc);
    int i;
    int j = 0;
    kstr[0] = '\0';
    int nstr = 0;
    for(i=0; i<num_alloc; i++) {
        for(; j<NUM; j++) {
            if(NULL == kptrs[j]) {
                break;
            }
        }
        assert(j != NUM);
        size_t size_alloc = SIZE_ALLOC(num+num_alloc);
        kptrs[j] = ump_simple_alloc(kump, size_alloc);
        if(NULL != kptrs[j]) {
            num_add ++;
        }
        nstr += snprintf(kstr+nstr, LEN_STR-nstr, 
                "\t\t%3d : size=%6d, kptrs[%d]%p%s\n", 
                i, size_alloc, j, kptrs[j], kptrs[j]?"":".......................");
        j ++;
    }
    ulogtst("num=%d, num_alloc = %d\n%s", num, num_alloc, kstr);
    ulogtst("test_alloc finished------[%d+%d=%d]--------------------------------.\n", num, num_add, num+num_add);
}

void test_free(void)
{
    int num = ptrs_num();
    int num_free = NUM_FREE(num);

    ulogtst("test_free-----[num=%3d, num_free=%3d]-------------------------------------------.\n", num, num_free);
    int empty = 0;
    if(num_free == num) { empty = 1; }
    kstr[0] = '\0';
    int nstr = 0;

    int t = 0;
    while(t < num_free) {
        int idx_free = select_free_idx(num);
        nstr += snprintf(kstr+nstr, LEN_STR-nstr, 
                "\t\t%3d : kptrs[%3d]%p\n", t, idx_free, kptrs[idx_free]);
        if(NULL == kptrs[idx_free]) {
            break;
        }
        assert(0 == ump_simple_free(kump, kptrs[idx_free]));
        kptrs[idx_free] = NULL;

        num --;
        t++;
    }

    ulogtst("num=%d, num_free = %d\n%s", num, num_free, kstr);
    assert(t == num_free);

    /* all freed detect. */
    if(empty) { NODES_ASSERT_ON_EMPTY }

    ulogtst("test_free finished---------------------------------------.\n");
}




int main()
{
    memset(kptrs, 0, sizeof(void*)*NUM);

    kump = ump_simple_create(LEN);
    assert(NULL != kump);

    while(1) {
        /* alloc. */
        test_alloc();

        /* free. */
        test_free();
    }

    ump_simple_destroy(kump);
    kump = NULL;

    return 0;
}
