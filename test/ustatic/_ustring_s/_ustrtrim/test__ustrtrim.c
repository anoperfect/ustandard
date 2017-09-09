#include "ustandard/ustatic/_ustring_s.h"
char* kteststrs[] = {
    "", 
    " ", 
    "  ", 
    " abc", 
    "abc ", 
    "  abc   ", 
    " abc def ", 
    "abcdef", 
    "\n"
};



int main()
{
    int nt = sizeof(kteststrs)/sizeof(kteststrs[0]);    
    #define LEN_TMP 1024
    char str_tmp[LEN_TMP];
    char str_tmpo[LEN_TMP];
    
    char blank[1] = {' '};

    int i;
    printf("none trim.\n");
    for(i=0; i<nt; i++) {
        _ustrcpy(str_tmp, LEN_TMP, kteststrs[i]);
        _ustrcpy(str_tmpo, LEN_TMP, kteststrs[i]);
        _ustrtrim(str_tmp, blank, 1, 0, 0);
        printf("[%s] after trim [%s]\n", str_tmpo, str_tmp);
    }

    printf("left trim.\n");
    for(i=0; i<nt; i++) {
        _ustrcpy(str_tmp, LEN_TMP, kteststrs[i]);
        _ustrcpy(str_tmpo, LEN_TMP, kteststrs[i]);
        _ustrtrim(str_tmp, blank, 1, 1, 0);
        printf("[%s] after trim [%s]\n", str_tmpo, str_tmp);
    }

    printf("right trim.\n");
    for(i=0; i<nt; i++) {
        _ustrcpy(str_tmp, LEN_TMP, kteststrs[i]);
        _ustrcpy(str_tmpo, LEN_TMP, kteststrs[i]);
        _ustrtrim(str_tmp, blank, 1, 0, 1);
        printf("[%s] after trim [%s]\n", str_tmpo, str_tmp);
    }

    printf("both trim.\n");
    for(i=0; i<nt; i++) {
        _ustrcpy(str_tmp, LEN_TMP, kteststrs[i]);
        _ustrcpy(str_tmpo, LEN_TMP, kteststrs[i]);
        _ustrtrim(str_tmp, blank, 1, 1, 1);
        printf("[%s] after trim [%s]\n", str_tmpo, str_tmp);
    }


    return 0;
}
