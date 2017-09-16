#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ustring.h"
char *ustrncpy(char *dest, size_t size_dest, const char *src, size_t n)
{
    uslog_check_arg(dest != NULL,           NULL);
    uslog_check_arg((int)size_dest>0,       NULL);

    char* rets = dest;
    errno = 0;

    memset(dest, 0, size_dest);
    size_t ncopy = 0;
    if(NULL != src) {
        size_t len_src = strlen(src);
        ncopy = n<=len_src?n:len_src; 
        if(ncopy > (size_dest-1)) {
            ulogerr("%s : %s.\n", __FUNCTION__, "not enough");
            errno = E2BIG;
            ncopy = size_dest-1;
        }
        else {

        } 
        strncpy(dest, src, ncopy);
        dest[ncopy] = '\0';
    }
    else {
        dest[0] = '\0';
    }

    return rets;
}


char *ustrcpy(char *dest, size_t size_dest, const char *src)
{
    uslog_check_arg(dest != NULL, NULL);

    char* rets = dest;
    errno = 0;
    rets = ustrncpy(dest, size_dest, src, NULL!=src?strlen(src):0);
    return rets;
}















/*strdup.*/
/* return value should use um_free(p) to free, not free(p). */
char* ustrndup(const char* src, size_t n)
{
    char* rets = NULL;

    errno = 0;
    src = NULL!=src?src:"";
    size_t len_src = strlen(src);
    n = um_min(n, len_src);
    size_t size_alloc = n + 1;
    rets = (char*)um_malloc(size_alloc);
    if(NULL != rets) {
        memcpy(rets, src, size_alloc-1);
        rets[size_alloc-1] = '\0';
    }
    else {
        usloge_malloc(size_alloc);
    }

    return rets;
}


/* return value should use um_free(p) to free, not free(p). */
char* ustrdup(const char* src)
{
    char* rets = NULL;
    errno = 0;
    rets = ustrndup(src, strlen(src));
    return rets;
}


/* return value should use um_free(p) to free, not free(p). */
char* ustrdup_sub(const char* src, struct urange range)
{
    uslog_check_arg(NULL != src,           NULL);
    uslog_check_arg(range.location >= 0 && range.length > 0 && (range.location + range.length) <= strlen(src), NULL);

    char* substring = ustrndup(src+range.location, range.length);
    return substring;
}


































char    kchars_blank[] = {' ', '\t'};
size_t  kn_chars_blank = 2;


char    kchars_crlf[] = {'\r', '\n'};
size_t  kn_chars_crlf = 2;


int uchars_include_char(const char s[], size_t n, char ch)
{
    int ret = 0;
    errno = 0;

    size_t i;
    for(i=0; i<n; i++)
    {
        if(ch == s[i]) {
            ret = 1;
            break;
        }
    }

    return ret;
}


/*strtrim.*/
char* ustrtrim_chars(char* s, int trim_left, int trim_right, 
        char chars[], size_t n)
{
    uslog_check_arg(s != NULL, NULL);

    char* rets = s;
    errno = 0;

    size_t len_s = strlen(s);
    size_t len_left = 0; 
    size_t len_right = 0; 
    int i;

    if(trim_right) {
        for(i=(int)len_s-1; i>=0; i--) {
            if(uchars_include_char(chars, n, s[i])) {
                len_right ++;
                s[i] = '\0';
            }
            else {
                break;
            }
        }
    }

    if(trim_left) {
        for(i=0; i<(int)len_s && s[i] != '\0'; i++) {
            if(uchars_include_char(chars, n, s[i])) {
                len_left ++;
            }
            else {
                break;
            }
        }

        if(len_left > 0) {
            size_t len_ns = len_s - len_left - len_right;
            for(i=0; i<(int)len_ns; i++) {
                s[i] = s[i+len_left];
            }

            memset(s+len_ns, 0, len_s+1-len_ns);
        }
    }

    return rets;
}


char* ustrtrim(char* s)
{
    uslog_check_arg(s != NULL, NULL);

    char* rets = s;
    errno = 0;
    ustrtrim_chars(s, 1, 1, UST_CHARS_BLANK, UST_N_CHARS_BLANK);
    return rets;
}







































































































/*string split.*/
int ustrsplit(const char* src, const char* split, 
        int n, char* dests[], size_t size_dests[])
{
    uslog_check_arg(src != NULL,        0);
    uslog_check_arg(split != NULL,      0);
    uslog_check_arg(strlen(split) > 0,  0);
    uslog_check_arg(n > 0,              0);
    uslog_check_arg(NULL != dests,      0);
    uslog_check_arg(NULL != size_dests, 0);

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

        if((e-s) > 0) {
            if(retn < n) {
                um_strncpy(dests[retn], size_dests[retn], s, e-s);
                retn ++;
            }
            else {
                errno = E2BIG;
            }
        }

        s = e + size_split;
    }

    if(s < (src+size_src)) {
        if(retn < n) {
            um_strncpy(dests[retn], size_dests[retn], s, (src+size_src+1)-s);
            retn ++;
        }
        else {
            errno = E2BIG;
        }
    }

    return retn;
}


int ustrsplit_sub(const char* src, const char* split, 
        int n, struct ustrsub subs[])
{
    uslog_check_arg(src != NULL,        0);
    uslog_check_arg(split != NULL,      0);
    uslog_check_arg(strlen(split) > 0,  0);
    uslog_check_arg(n > 0,              0);
    uslog_check_arg(NULL != subs,       0);

    int retn = 0;
    int num = 0;
    char* s;
    char* e;
    s = (char*)src;
    size_t size_src = strlen(src);
    size_t size_split = strlen(split);
    while(num < n && (*s) != '\0') {
        e = strstr(s, split);
        if(NULL == e) {
            break;
        }

        if((e-s) > 0) {
            if(num < n) {
                subs[num].s = s;
                subs[num].size = e-s;
                num ++;
            }
            else {
                errno = E2BIG;
            }
        }

        s = e + size_split;
    }

    if(s < (src+size_src)) {
        if(num < n) {
            subs[num].s = s;
            subs[num].size = (src+size_src+1)-s;
            num ++;
        }
        else {
            errno = E2BIG;
        }
    }

    retn = num;
    return retn;
}


int ustrsplit2(const char* src, const char* split, 
        char* s1, size_t size1,
        char* s2, size_t size2)
{
    uslog_check_arg(src != NULL,        -1);
    uslog_check_arg(split != NULL,      -1);
    uslog_check_arg(s1 != NULL,         -1);
    uslog_check_arg(s2 != NULL,         -1);
    int ret = 0;
    const char* tmp = strstr(src, split);
    if(NULL != tmp) {
        ustrncpy(s1, size1, src, tmp-src);
        tmp += strlen(split);
        ustrcpy(s2, size2, tmp);
    }
    else {
        ret = -1;
    }

    return ret;
}




















/*strcmp.*/
/*
    return 0, if s2 equal to s1 tailer. 
 */
int ustrcmp_tailer(const char* s1, const char* s2)
{
    uslog_check_arg(s1 != NULL, 0);
    uslog_check_arg(s2 != NULL, 0);

    int ret = 0;

    size_t size1 = strlen(s1);
    size_t size2 = strlen(s2);

    if(size1 < size2) {
        ret = -1;
    }
    else {
        s1 += (size1 - size2);
        ret = strcmp(s1, s2);
    }

    return ret;
}


/*
    return 0, if s2 equal to s1 tailer in n length. 
 */
int ustrncmp_tailer(const char* s1, const char* s2, size_t n)
{
    int ret = 0;

    size_t size1 = strlen(s1);
    size_t size2 = strlen(s2);

    if(n > size1 || n > size2) {
        ret = -1;
    }
    else {
        s1 += (size1 - n);
        s2 += (size2 - n);
        ret = strcmp(s1, s2);
    }

    return ret;
}


/*
    return 0, if s2 case equal to s1 tailer. 
 */
int ustrcasecmp_tailer(const char* s1, const char* s2)
{
    uslog_check_arg(s1 != NULL, 0);
    uslog_check_arg(s2 != NULL, 0);

    int ret = 0;

    size_t size1 = strlen(s1);
    size_t size2 = strlen(s2);

    if(size1 < size2) {
        ret = -1;
    }
    else {
        s1 += (size1 - size2);
        ret = strcasecmp(s1, s2);
    }

    return ret;
}


/*
    return 0, if s2 case equal to s1 tailer in n length. 
 */
int ustrncasecmp_tailer(const char* s1, const char* s2, size_t n)
{
    int ret = 0;

    size_t size1 = strlen(s1);
    size_t size2 = strlen(s2);

    if(n > size1 || n > size2) {
        ret = -1;
    }
    else {
        s1 += (size1 - n);
        s2 += (size2 - n);
        ret = strcasecmp(s1, s2);
    }

    return ret;
}


/*
    return char value of the string. 
 */
char ustrtail(const char* s)
{
    char ret = 0;

    size_t len;

    if(NULL != s && (len=strlen(s))>0) {
        ret = s[len-1];
    }

    return ret;
}













































































char* ustrcut_char_last(char* output, char ch)
{
    size_t len;
    if((NULL != output) 
            && ((len=strlen(output)) > 0)
            && (ch == output[len-1] || 0 == ch)) {
        output[len-1] = '\0';
    }

    return output;
}





























struct sub_string {
    const char* s;
    size_t      n;
};


int ustrreg_sub(const char* s, 
        const char* regex, 
        struct sub_string* sub)
{
    int ret = 0;

    regex_t reg = {0};
    ret = regcomp(&reg, regex, REG_EXTENDED);
    if(0 != ret) {
        ret = -1;
        goto finish;
    }

    regmatch_t match;
    ret = regexec(&reg, s, 1, &match, 0);
    if(0 != ret) {
        ret = -1;
        goto finish;
    }

    if(NULL != sub) {
        sub->s = s+match.rm_so;
        sub->n = match.rm_eo - match.rm_so;
    }

finish:
    regfree(&reg);
    return ret;
}


/*
    return : found ranges. 
            -1: input value invalid. 
             0: not found.
           > 0: found number. it would not larger then nmax. so if return value equal to nmax, maybe there're more matched pattern.
 */
long ustr_find(const char* s, 
        const char* needle, 
        struct urange* range,
        long nmax)
{
    uslog_check_arg(NULL != s,          -1);
    uslog_check_arg(NULL != needle,     -1);
    uslog_check_arg(strlen(needle)>0,   -1);
    uslog_check_arg(NULL != range,      -1);
    uslog_check_arg(nmax > 0,           -1);

    int retn = 0;

    const char* t = s;
    size_t len_needle = strlen(needle);

    while(1) {
        char* tmp = strstr(t, needle);
        if(NULL == tmp) {
            break;
        }

        if(retn < nmax) {
            range[retn].location = tmp - s;
            range[retn].length = len_needle;

            retn ++;
            t = tmp + len_needle;
        }
        else {
            break;
        }
    }

    return retn;
}













































/********************************************
ustring/ustrcpy
ustring/ustrdup
ustring/ustrtrim
ustring/ustrsplit

  *******************************************/
