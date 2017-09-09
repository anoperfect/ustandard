#include <stdio.h>
#include <string.h>
#include "ustandard/ustatic/_ustatic_s.h"
#ifndef ustandard_perror
    #define ustandard_perror(x...) fprintf(stderr, x)
#endif
static char* _ustrncpy(char* dest, size_t size_dest, const char* src, size_t size_src)
{
    size_t size = size_dest-1>=size_src?size_src:size_dest-1;
    strncpy(dest, src, size);
    dest[size] = '\0';

    if(size_dest-1 < size_src) {
        /* Indicate if dest string length not enough. */
        ustandard_perror("string length not enough.\n");
    }

    return dest;
}


static char* _ustrncat(char* dest, size_t size_dest, const char* src, size_t size_src)
{
    size_t len = strlen(dest);
    dest += len;
    size_dest -= len;

    _ustrncpy(dest, size_dest, src, size_src);

    return dest;
}


static char* _ustrcpy(char* dest, size_t size_dest, const char* src)
{
    size_t size_src = strlen(src);
    _ustrncpy(dest, size_dest, src, size_src);
    return dest;
}


static char* _ustrcat(char* dest, size_t size_dest, const char* src)
{
    
    size_t len = strlen(dest);
    dest += len;
    size_dest -= len;

    size_t size_src = strlen(src);
    _ustrncpy(dest, size_dest, src, size_src);

    return dest;
}


struct _usstring {
    const char* s;
    size_t len;
};


int _ustrsplit(const char* src, const char* split, struct _usstring* string, int n)
{
    int retn = 0;
    char* s;
    char* e;
    s = (char*)src;
    size_t size_src = strlen(src);
    size_t size_split = strlen(split);
    while(retn < n && (*s) != '\0') {
        e = strstr(s, split);
        if(NULL == e) {
            break;
        }

        if(e != s) {
            string[retn].s = s;
            string[retn].len = e - s;
            retn ++;
        }

        s = e + size_split;
    }

    if(s < (src+size_src)) {
        string[retn].s = s;
        string[retn].len = (src+size_src+1)-s;
        retn ++;
    }

    return retn;
}


static char kchars_blank[] = {'\r', '\n', ' ', '\t'};


static int _ustrn_include_char(const char* s, size_t n, char ch)
{
    int ret = 0;
    int i;
    for(i=0; i<n; i++)
    {
        if(ch == s[i])
            return 1;
    }

    return ret;
}


static char* _ustrtrim(char* str, const char s[], size_t n, 
        int is_trim_left, int is_trim_right)
{
    int len_trim_left = 0;
    int len_trim_right = 0;
    int len_str = 0;

    len_str = strlen(str);
    if(len_str == 0) {
        return str;
    }

    char* tmp = str;

    if(is_trim_right) {
        tmp = str + (len_str-1);
        while(_ustrn_include_char(s, n, *tmp)) {
            tmp -- ;
            len_trim_right ++ ;
        }

        str[len_str-len_trim_right] = '\0';
        len_str -= len_trim_right;
    }

    if(is_trim_left) {
        tmp = str;
        while(*tmp != '\0' && _ustrn_include_char(s, n, *tmp))
        {
            len_trim_left ++ ;
            tmp ++ ;
        }

        if(len_trim_left != 0) {
            int i;
            for(i=0; i<=len_str-len_trim_left; i++)
            {
                str[i] = str[i+len_trim_left];
            }
        }
    }     

    return str;
}





