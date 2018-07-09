#include "ustandard/ustandard.h"
int main(int argc, char* argv[])
{
    char* src = "123";
    size_t n;
    char* dest;

    umalloc_enable_monitor(1);

    n = 0;
    dest = um_strndup(src, n);
    uloginf("[%s]\n", dest);

    n = 1;
    dest = um_strndup(src, n);
    uloginf("[%s]\n", dest);

    n = 2;
    dest = um_strndup(src, n);
    uloginf("[%s]\n", dest);

    n = 3;
    dest = um_strndup(src, n);
    uloginf("[%s]\n", dest);

    n = 4;
    dest = um_strndup(src, n);
    uloginf("[%s]\n", dest);

    n = 5;
    dest = um_strndup(src, n);
    uloginf("[%s]\n", dest);

    n = 100;
    dest = um_strndup(src, n);
    uloginf("[%s]\n", dest);

    n = 100;
    char* str0 = NULL;
    dest = um_strndup(str0, n);
    uloginf("[%s]\n", dest);
    

    dest = um_strdup(src);
    uloginf("[%s]\n", dest);

    dest = um_strdup(str0);
    uloginf("[%s]\n", dest);

    char s[1024];
    umalloc_query(s, sizeof(s));
    uloginf("\n%s\n", s);



    return 0;
}
