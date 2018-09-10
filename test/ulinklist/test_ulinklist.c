#include "ustandard/ustandard.h"
int main(int argc, char* argv[])
{
    umalloc_enable_monitor(1);

    char str[1024];

    struct ulinklist* list = ulinklist_create();
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "empty", ulinklist_length(list), str);

    ulinklist_add_tailer(list, (void*)1);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "1", ulinklist_length(list), str);

    ulinklist_add_tailer(list, (void*)2);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "12", ulinklist_length(list), str);

    ulinklist_add_tailer(list, (void*)3);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "123", ulinklist_length(list), str);

    ulinklist_add_tailer(list, (void*)4);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "1234", ulinklist_length(list), str);

    ulinklist_add_tailer(list, (void*)5);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "12345", ulinklist_length(list), str);

    ulinklist_add_tailer(list, (void*)6);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "123456", ulinklist_length(list), str);

    ulinklist_add_header(list, (void*)7);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "7123456", ulinklist_length(list), str);

    while(ulinklist_length(list)) {
        ulinklist_pop(list);
        ulinklist_description(list, str, sizeof(str));
        uloginf("%20s => %d [%s]\n", "pop", ulinklist_length(list), str);
    }


#if 0
    void* data;
    while(NULL != (data=ulinklist_next(list))) {
        ulogtst("payload : %ld.\n", (long)data);
    }
#endif

    char s[10240];
    umalloc_query(s, 10240);
    printf("umalloc_query : %s\n", s);


    ulinklist_add_header(list, (void*)1);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "1", ulinklist_length(list), str);

    ulinklist_add_header(list, (void*)2);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "21", ulinklist_length(list), str);

    ulinklist_add_header(list, (void*)3);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "321", ulinklist_length(list), str);

    ulinklist_add_header(list, (void*)4);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "4321", ulinklist_length(list), str);

    ulinklist_add_header(list, (void*)5);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "54321", ulinklist_length(list), str);

    ulinklist_add_header(list, (void*)6);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "654321", ulinklist_length(list), str);

    ulinklist_add_tailer(list, (void*)7);
    ulinklist_description(list, str, sizeof(str));
    uloginf("%20s => %d [%s]\n", "6543217", ulinklist_length(list), str);

    while(ulinklist_length(list)) {
        ulinklist_pop(list);
        ulinklist_description(list, str, sizeof(str));
        uloginf("%20s => %d [%s]\n", "empty", ulinklist_length(list), str);
    }







    ulinklist_destroy(list);

    umalloc_query(s, 10240);
    printf("umalloc_query after ulinklist_destroy:%s\n", s);



    return 0;
}
