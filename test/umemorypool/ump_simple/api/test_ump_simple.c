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

    void* pa;
    void* pb;
    void* pc;
    void* pd1;
    void* pd2;
    void* pe;
    void* pf;size_t size_f = 1036740;
    void* pg;
    size_t size_a = 100;
    size_t size_b = 200;
    size_t size_c = 1000;
    size_t size_d1 = 50;
    size_t size_d2 = 10000;
struct ump_simple_node knodes[1024];
int knum;
int knum_get;

#define LT  10
void test_show_nodes(const char* item)
{
#define LEN_STR 10240
    char str[LEN_STR];
    size_t n = 0; 
    int i;
    snprintf(str+n, LEN_STR-n, "<%10s> knodes: ", item); 
    n = strlen(str);

    for(i=0; i<knum_get; i++) {
        snprintf(str+n, LEN_STR-n, " | %d: %u,%u,%u | ", i, 
                knodes[i].offset, knodes[i].size_alloc, knodes[i].size_free);
        n = strlen(str);
    }
    ulogtst("%s.\n", str);
}


#define NODE_ASSERT(idx, oset, s_alloc, s_free)                                     \
do{                                                                                 \
    assert( knodes[idx].offset == (oset) );                                         \
    assert( knodes[idx].size_alloc == (s_alloc) );                                  \
    assert( knodes[idx].size_free == (s_free) );                                    \
}while(0);


#define NODES_ASSERT_ON_EMPTY                                                       \
do{                                                                                 \
    knum = ump_sumple_get_node_number(ump);                                         \
    assert(1 == knum);                                                              \
    knum_get = ump_sumple_get_node(ump, 0, knum, knodes);                           \
    assert(knum_get == knum);                                                       \
    test_show_nodes("empty");                                                       \
    NODE_ASSERT(0, 0, 0, (LEN - LEN_UMP - LEN_NODES))                               \
}while(0);


#define NODES_GET(n)                                                                \
do{                                                                                 \
    knum = ump_sumple_get_node_number(ump);                                         \
    assert(n == knum);                                                              \
    knum_get = ump_sumple_get_node(ump, 0, knum, knodes);                           \
    assert(knum_get == knum);                                                       \
}while(0)


#define __addr_content  ((void*)ump + LEN_UMP + LEN_NODES)
#define __real(size) (um_align4(size) + 2*LEN_BORDER)
#define LEN_CONTENT (LEN - LEN_UMP - LEN_NODES)

void test1(struct ump_simple* ump)
{
    ulogtst("running %s .\n", __FUNCTION__);
    NODES_ASSERT_ON_EMPTY
    /* +a -a */
    pa = ump_simple_alloc(ump, size_a);
    NODES_GET(1); 
    test_show_nodes("+a");
    assert(pa == (__addr_content + LEN_BORDER));
    NODES_GET(1); 
    NODE_ASSERT(0, 0, __real(size_a), LEN_CONTENT - __real(size_a))
    
    ump_simple_free(ump, pa);
    NODES_ASSERT_ON_EMPTY

    /* +a +b */
    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    NODES_GET(2);
    test_show_nodes("+a+b");
    assert(pb == (__addr_content + __real(size_a) + LEN_BORDER));
    NODE_ASSERT(0, 0, __real(size_a), 0)
    NODE_ASSERT(1, __real(size_a), __real(size_b), LEN_CONTENT - __real(size_a) - __real(size_b))
    /* +a +b -a */
    ump_simple_free(ump, pa);
    NODES_GET(2); 
    test_show_nodes("+a+b-a");
    NODE_ASSERT(0, 0, 0, __real(size_a))
    NODE_ASSERT(1, __real(size_a), __real(size_b), LEN_CONTENT - __real(size_a) - __real(size_b))

#if 1    
    /* +a +b -a +a*/
    pa = ump_simple_alloc(ump, size_a);
    NODES_GET(2); 
    test_show_nodes("+a+b-a+a");
    assert(pb == (__addr_content + __real(size_a) + LEN_BORDER));
    NODE_ASSERT(0, 0, __real(size_a), 0)
    NODE_ASSERT(1, __real(size_a), __real(size_b), LEN_CONTENT - __real(size_a) - __real(size_b))
    /* +a +b -b */
    ump_simple_free(ump, pb);
    NODES_GET(1); 
    test_show_nodes("+a+b-b");
    NODE_ASSERT(0, 0, __real(size_a), (LEN_CONTENT - __real(size_a)))

    /* +a +b -b +b*/
    pb = ump_simple_alloc(ump, size_b);
    NODES_GET(2); 
    test_show_nodes("+a+b-a+a");
    assert(pb == (__addr_content + __real(size_a) + LEN_BORDER));
    NODE_ASSERT(0, 0, __real(size_a), 0)
    NODE_ASSERT(1, __real(size_a), __real(size_b), LEN_CONTENT - __real(size_a) - __real(size_b))
    /* +a +b -a -b */
    ump_simple_free(ump, pa);
    ump_simple_free(ump, pb);
    NODES_ASSERT_ON_EMPTY

    /* +a +b -b -a */
    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    ump_simple_free(ump, pb);
    ump_simple_free(ump, pa);
    NODES_ASSERT_ON_EMPTY

#endif
}


void test2(struct ump_simple* ump)
{
    NODES_ASSERT_ON_EMPTY;
    const char* test_item = NULL;
    
    test_item = "+a+b+c";
    ulogtst("test on : %s\n", test_item);
    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    NODES_GET(3);
    test_show_nodes(test_item);
    assert(pc == (__addr_content + __real(size_a) + __real(size_b) + LEN_BORDER));
    NODE_ASSERT(0, 0, __real(size_a), 0)
    NODE_ASSERT(1, __real(size_a), __real(size_b), 0)
    NODE_ASSERT(2, __real(size_a) + __real(size_b), __real(size_c), LEN_CONTENT - __real(size_a) - __real(size_b) - __real(size_c))
    ump_simple_free(ump, pa);
    ump_simple_free(ump, pb);
    ump_simple_free(ump, pc);
    NODES_ASSERT_ON_EMPTY;

    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    ump_simple_free(ump, pa);
    ump_simple_free(ump, pc);
    ump_simple_free(ump, pb);
    NODES_ASSERT_ON_EMPTY;

    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    ump_simple_free(ump, pb);
    ump_simple_free(ump, pa);
    ump_simple_free(ump, pc);
    NODES_ASSERT_ON_EMPTY;

    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    ump_simple_free(ump, pb);
    ump_simple_free(ump, pc);
    ump_simple_free(ump, pa);
    NODES_ASSERT_ON_EMPTY;

    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    ump_simple_free(ump, pc);
    ump_simple_free(ump, pa);
    ump_simple_free(ump, pb);
    NODES_ASSERT_ON_EMPTY;

    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    ump_simple_free(ump, pc);
    ump_simple_free(ump, pb);
    ump_simple_free(ump, pa);
    NODES_ASSERT_ON_EMPTY;

    test_item = "+a+b+c+last";
    ulogtst("test on : %s\n", test_item);
    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    pf = ump_simple_alloc(ump, 1036740);
    NODES_GET(4);
    test_show_nodes(test_item);
    assert(pf == (__addr_content + __real(size_a) + __real(size_b) + __real(size_c)+ LEN_BORDER));
    NODE_ASSERT(3, __real(size_a) + __real(size_b) + __real(size_c), __real(size_f), 0)
    ump_simple_free(ump, pc);
    ump_simple_free(ump, pb);
    ump_simple_free(ump, pf);
    ump_simple_free(ump, pa);
    /* more free sequence. */ 

    test_item = "+a+b+c+last-last";
    ulogtst("test on : %s\n", test_item);
    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    pf = ump_simple_alloc(ump, 1036740);
    ump_simple_free(ump, pf);
    NODES_GET(3);
    test_show_nodes(test_item);
    assert(pc == (__addr_content + __real(size_a) + __real(size_b) + LEN_BORDER));
    NODE_ASSERT(0, 0, __real(size_a), 0)
    NODE_ASSERT(1, __real(size_a), __real(size_b), 0)
    NODE_ASSERT(2, __real(size_a) + __real(size_b), __real(size_c), LEN_CONTENT - __real(size_a) - __real(size_b) - __real(size_c))
    ump_simple_free(ump, pc);
    ump_simple_free(ump, pb);
    ump_simple_free(ump, pa);

    test_item = "+a+b+c-a";
    ulogtst("test on : %s\n", test_item);
    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    ump_simple_free(ump, pa);
    NODES_GET(3);
    test_show_nodes(test_item);
    assert(pc == (__addr_content + __real(size_a) + __real(size_b) + LEN_BORDER));
    NODE_ASSERT(0, 0, 0, __real(size_a))
    NODE_ASSERT(1, __real(size_a), __real(size_b), 0)
    NODE_ASSERT(2, __real(size_a) + __real(size_b), __real(size_c), LEN_CONTENT - __real(size_a) - __real(size_b) - __real(size_c))

    test_item = "+a+b+c-a+d1";
    ulogtst("test on : %s\n", test_item);
    pd1 = ump_simple_alloc(ump, size_d1);
    NODES_GET(3);
    test_show_nodes(test_item);
    assert(pd1 == (__addr_content + LEN_BORDER));
    NODE_ASSERT(0, 0, __real(size_d1), __real(size_a) - __real(size_d1))
    NODE_ASSERT(1, __real(size_a), __real(size_b), 0)
    NODE_ASSERT(2, __real(size_a) + __real(size_b), __real(size_c), LEN_CONTENT - __real(size_a) - __real(size_b) - __real(size_c))
    ump_simple_free(ump, pd1);
    ump_simple_free(ump, pb);
    ump_simple_free(ump, pc);
    NODES_ASSERT_ON_EMPTY;
    
    test_item = "+a+b+c-a+d2";
    pa = ump_simple_alloc(ump, size_a);
    pb = ump_simple_alloc(ump, size_b);
    pc = ump_simple_alloc(ump, size_c);
    ump_simple_free(ump, pa);
    pd2 = ump_simple_alloc(ump, size_d2);
    NODES_GET(4);
    test_show_nodes(test_item);
    assert(pd2 == (__addr_content + __real(size_a) + __real(size_b) + __real(size_c) + LEN_BORDER));
    NODE_ASSERT(0, 0, 0, __real(size_a))
    NODE_ASSERT(1, __real(size_a), __real(size_b), 0)
    NODE_ASSERT(2, __real(size_a) + __real(size_b), __real(size_c), 0)
    NODE_ASSERT(3, __real(size_a) + __real(size_b) + __real(size_c), __real(size_d2), LEN_CONTENT - (__real(size_a) + __real(size_b) + __real(size_c) + __real(size_d2)))
    ump_simple_free(ump, pb);
    ump_simple_free(ump, pd2);
    ump_simple_free(ump, pc);
    NODES_ASSERT_ON_EMPTY;
}


int main() {

    struct ump_simple* ump= NULL;
    ump = ump_simple_create(1024*1024);
    ump_simple_destroy(ump);
    ump = NULL; 

    ump = ump_simple_create(1024*1024);
    test1(ump); 
    test2(ump); 
    ump_simple_destroy(ump);
    ump = NULL; 

    return 0;
}
