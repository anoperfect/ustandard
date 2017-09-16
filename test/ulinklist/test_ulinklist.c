#include "ustandard/ustandard.h"
int main()
{
    umalloc_enable(1);


    struct ulinklist* list = ulinklist_create();
    ulinklist_add_tailer(list, 1);
    ulinklist_add_tailer(list, 2);
    ulinklist_add_tailer(list, 3);
    ulinklist_add_tailer(list, 4);
    ulinklist_add_tailer(list, 5);
    ulinklist_add_tailer(list, 6);
    ulinklist_add_header(list, 7);

    int payload = 0;
    while(0 != (payload=ulinklist_next(list))) {
        ulogtst("payload : %d.\n", payload);
    }

    char s[10240];

    umalloc_query(s, 10240);
    printf("%s\n", s);

    ulinklist_destroy(list);

    umalloc_query(s, 10240);
    printf("%s\n", s);

    return 0;
}
