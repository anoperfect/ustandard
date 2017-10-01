#include "ustandard/ustandard.h"
int main()
{
    umalloc_enable(1);


    struct ulinklist* list = ulinklist_create();
    ulinklist_add_tailer(list, (void*)1);
    ulinklist_add_tailer(list, (void*)2);
    ulinklist_add_tailer(list, (void*)3);
    ulinklist_add_tailer(list, (void*)4);
    ulinklist_add_tailer(list, (void*)5);
    ulinklist_add_tailer(list, (void*)6);
    ulinklist_add_header(list, (void*)7);

    struct uvdata* data;
    while(NULL != (data=ulinklist_next(list))) {
        ulogtst("payload : %ld.\n", (long)(data->p));
    }

    char s[10240];

    umalloc_query(s, 10240);
    printf("umalloc_query : %s\n", s);

    ulinklist_destroy(list);

    umalloc_query(s, 10240);
    printf("umalloc_query after ulinklist_destroy:%s\n", s);

    return 0;
}
