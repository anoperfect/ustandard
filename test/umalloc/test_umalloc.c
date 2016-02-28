#include "ustandard/ustandard.h"
int main()
{
#define LEN 1000
    char str[LEN];

    void* p1 = NULL;
    void* p2 = NULL;
    void* p3 = NULL;

    umalloc_enable(1);


    p1 = um_malloc(1024);
    umalloc_query(str, LEN);
    printf("p1:\n%s\n", str);

    p2 = um_malloc(1024);
    umalloc_query(str, LEN);
    printf("p1+p2:\n%s\n", str);

    p3 = um_malloc(1024);
    umalloc_query(str, LEN);
    printf("p1+p2+p3:\n%s\n", str);




    um_free(p2);
    umalloc_query(str, LEN);
    printf("p1+p2+p3-p2:\n%s\n", str);

    um_free(p3);
    umalloc_query(str, LEN);
    printf("p1+p2+p3-p2-p3:\n%s\n", str);

    um_free(p1);
    umalloc_query(str, LEN);
    printf("p1+p2+p3-p2-p3-p1:\n%s\n", str);

    void* r1 = NULL;
    void* r2 = NULL;
    
    r1 = um_realloc(NULL, 100);
    umalloc_query(str, LEN);
    printf("0 -> r1:\n%s\n", str);

    um_free(r1);
    umalloc_query(str, LEN);
    printf("0 -> r1, free(r1):\n%s\n", str);

    r1 = um_realloc(NULL, 100);
    r2 = um_realloc(r1, 0);
    umalloc_query(str, LEN);
    printf("0 -> r1 -> 0: [r1=%p, r2=%p]\n%s\n", r1, r2, str);

    r1 = um_realloc(NULL, 100);
    r2 = NULL;
    #define LEN_R2 (100*1024*1024)
    r2 = um_realloc(r1, LEN_R2);
    umalloc_query(str, LEN);
    printf("r1 -> r2(%u):\n%s\n", LEN_R2, str);






    umalloc_enable(0);

    return 0;
}
