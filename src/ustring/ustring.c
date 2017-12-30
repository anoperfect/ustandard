#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ubuffer.h"
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





char* ustrcat(char* dest, size_t size_dest, const char* src)
{
    uslog_check_arg(dest, dest);
    uslog_check_arg(src, dest);
    size_t len = strlen(dest);
    uslog_check_arg(size_dest > len, dest);

    char* retp = ustrcpy(dest+len, size_dest-len, src);
    return retp;
}


char* ustrncat(char* dest, size_t size_dest, const char* src, size_t n)
{
    uslog_check_arg(dest, dest);
    uslog_check_arg(src, dest);
    size_t len = strlen(dest);
    uslog_check_arg(size_dest > len, dest);

    char* retp = ustrncpy(dest+len, size_dest-len, src, n);
    return retp;
}


char* ustrcat_format(char* dest, size_t size_dest, const char* fmt, ...)
{
    char* retp = dest;

    char tmp[1024];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(tmp, 1024, fmt, ap);
    va_end(ap);

    if(n > -1 && n < 1024) {
        retp = ustrcat(dest, size_dest, tmp);
        return retp;
    }

    int size = 1024 * 2;
    char *p, *np;
    if ((p = um_malloc(size)) == NULL) {
        return retp;
    }

    while (1) {
        /* Try to print in the allocated space. */
        va_start(ap, fmt);
        n = vsnprintf(p, size, fmt, ap);
        va_end(ap);
        /* If that worked, return the string. */
        if (n > -1 && n < size)
            return p;
        /* Else try again with more space. */
        if (n > -1)    /* glibc 2.1 */
            size = n+1; /* precisely what is needed */
        else           /* glibc 2.0 */
            size *= 2;  /* twice the old size */
        if ((np = um_realloc (p, size)) == NULL) {
            um_free(p);
            break;
        } 
        else {
            p = np;
        }
    }

    return retp;
}

































char    kchars_blank[] = {' ', '\t'};
size_t  kn_chars_blank = 2;


char    kchars_crlf[] = {'\r', '\n'};
size_t  kn_chars_crlf = 2;


bool uchars_include_char(const char s[], size_t n, char ch)
{
    int ret = false;
    errno = 0;

    size_t i;
    for(i=0; i<n; i++)
    {
        if(ch == s[i]) {
            ret = true;
            break;
        }
    }

    return ret;
}


/*strtrim.*/
char* ustrtrim_chars(char* s, bool trim_left, bool trim_right, 
        const char* chars, size_t n)
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
            /* could use bitmap. */
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












































































/* if string last char equal to ch, cut it. usually used to cut \n if the reading line */
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

    long retn = 0;

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


/* use um_free(p) to free the return dest value. */
int ustr_replaces(const char* s, 
        const char* needle, const char* to, 
    	void** ppdest, size_t* len_dest)
{
    int ret = 0;

    long nfind = 0;
    long n = 1024;
    struct urange* ranges = NULL;
        
    while(1) {
        ranges = um_realloc(ranges, sizeof(*ranges) * n);
        nfind = ustr_find(s, 
                needle, 
                ranges, n);

        if(nfind != n) {
            break;
        }

        /* expand ranges. */
        n = n * 2;
        continue;
    }

    if(nfind == -1) {
        ulogerr("find return %ld\n", nfind);
        ret = -1;
    }
    else {
        size_t len_s        = strlen(s);
        size_t len_to       = strlen(to);
        struct ucbuf bufs_replace[1];
        bufs_replace[0].p       = to;
        bufs_replace[0].size    = len_to;

        /* '\0' need to make dest got a \0 terminal. */
        ret = ubuffer_replace(s, len_s+1, 
    		ppdest, len_dest, 
            ranges, nfind,
            bufs_replace, 1);
    }

    return ret;
}


int _urange_add_to_array(struct urange** pprange, long* pnum, long* ptotal, struct urange* range_add)
{
    int ret = 0;

    if(NULL == *pprange) {
        *ptotal = 10;
        *pnum = 0;
        *pprange = um_malloc(sizeof(struct urange) * *ptotal);
        if(NULL == *pprange) {
            *pnum =  0;
            *ptotal = 0;
            ret = -1;
        }
    }
    else {
        if(*pnum >= *ptotal) {
            struct urange* tmp = um_realloc(*pprange, sizeof(struct urange) * 2 * *ptotal);
            if(tmp) {
                *ptotal = *ptotal * 2;
            }
            else {
                ret = -1;
            }
        }
    }

    if( 0 == ret) {
        memcpy(*pprange+*pnum, range_add, sizeof(struct urange));
        (*pnum) ++;
    }

    return ret;
}



/*
    buffer with size, choosing n length range, count the left.
    it's used on the split function. first find the range, then use urange_left to get the splited segment.

    return struct urange* have the number *nleft, use um_free(p) to free.
 */
struct urange* urange_left(size_t size, struct urange* ranges, long n, long* nleft)
{
    uslog_check_arg(ranges,     NULL);
    uslog_check_arg(nleft,      NULL);

    bool invalid = false;

    long idx;
    struct urange range;
    long prev = 0;

    struct urange* range_left = NULL;
    long nrange_left = 0;
    long ntotal_range_left = 0;

    for(idx=0; idx<n; idx++) {
        range.location = prev;
        if(ranges[idx].location >= prev) {
            range.length = ranges[idx].location - prev;
            _urange_add_to_array(&range_left, &nrange_left, &ntotal_range_left, &range);
        }
        else {
            invalid = true;
        }

        prev = ranges[idx].location + ranges[idx].length;
    }

    if(size >= prev) {
        range.location = prev;
        range.length = size - prev;
        _urange_add_to_array(&range_left, &nrange_left, &ntotal_range_left, &range);
    }
    else {
        invalid = true;
    }

    if(invalid) {
        um_free_check(range_left);
        range_left = NULL;
        *nleft = 0;
    }
    else {
        *nleft = nrange_left;
    }

    return range_left;
}












































/********************************************
ustring/ustrcpy
ustring/ustrdup
ustring/ustrtrim
ustring/ustrsplit

  *******************************************/
