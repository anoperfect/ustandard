#include "ustandard/ustatic/_ustring_s.h"
char* kteststrs[] = {
    "", 
    "123", 
    ",", 
    ",123", 
    "123,", 
    "123,456", 
    ",123,456", 
    "123,456,", 
};
int main()
{
    struct _usstring string[10];

    #define LEN_TMP 1024
    char str_tmp[LEN_TMP];

    int nt = sizeof(kteststrs)/sizeof(kteststrs[0]);

    int i;
    int j;
    for(i=0; i<nt; i++) {
        const char* str_test = kteststrs[i];

        int n = _ustrsplit(str_test, ",", 10, string);
        printf("parse : %s\n", str_test);

        for(j=0; j<n; j++) {
            _ustrncpy(str_tmp, LEN_TMP, string[j].s, string[j].len);
            printf("\t%d : %s\n", j, str_tmp);
        }
    }

    return 0;
}
