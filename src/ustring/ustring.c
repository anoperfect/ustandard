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



__thread char*  __ustrdup_str;
__thread size_t __ustrdup_size_n;
__thread size_t __ustrdup_len;




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
    uslog_check_arg(dest, dest);
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
        if (n > -1 && n < size) {
            break;
        }

        /* Else try again with more space. */
        if (n > -1)    /* glibc 2.1 */
            size = n+1; /* precisely what is needed */
        else           /* glibc 2.0 */
            size *= 2;  /* twice the old size */
        if ((np = um_realloc (p, size)) == NULL) {
            break;
        } 
        else {
            p = np;
        }
    }

    ustrcat(dest, size_dest, p);
    um_free(p);

    return retp;
}

































const char kchars_blank[] = " \t";
const char kchars_crlf[] = "\r\n";


static bool uchars_include_char(const char s[], size_t n, char ch)
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
        const char* chars)
{
    uslog_check_arg(s != NULL, NULL);

    size_t n = strlen(chars);
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
    ustrtrim_chars(s, 1, 1, kchars_blank);
    return rets;
}






































































































/*string split.*/
static long _ustrsplit(const char* src, const char* split, int opt, 
        long n, struct urange* ranges, struct urange** ranges_expand)
{
    uslog_check_arg(src != NULL,        0);
    uslog_check_arg(split != NULL,      0);
    uslog_check_arg(strlen(split) > 0,  0);
    uslog_check_arg(n > 0,              0);
    uslog_check_arg(NULL != ranges,      0);
    uslog_check_arg(NULL != ranges_expand, 0);

    int retn = 0;
    char* s;
    char* e;
    size_t len_src = strlen(src);
    size_t size_split = strlen(split);

    long nfound = 0;
    s = (char*)src;
    while((*s) != '\0') {
        e = strstr(s, split);
        if(NULL == e) {
            break;
        }

        if((e-s) > 0 || (opt & USTRSPLIT_INCLUDE_0LENGTH)){
            nfound ++;
        }
    
        s = e + size_split;
    }

    if(s < (src+len_src)) {
        nfound ++;
    }

    struct urange* ranges_write = NULL;
    long nranges_write = 0;
    nranges_write = nranges_write;
    if(nfound <= n) {
        ranges_write = ranges;
        nranges_write = n;
    }
    else {
        *ranges_expand = um_malloc(sizeof(struct urange) * nfound);
        ranges_write = *ranges_expand;
        nranges_write = nfound;
    }

    s = (char*)src;
    long idx = 0;
    while(retn < n && (*s) != '\0') {
        e = strstr(s, split);
        if(NULL == e) {
            break;
        }

        if((e-s) > 0 || (opt & USTRSPLIT_INCLUDE_0LENGTH)){
            ranges_write[idx].location = s - src;
            ranges_write[idx].length = (opt & USTRSPLIT_INCLUDE_SPLIT)?(e-s+size_split):(e-s);
            idx ++;
        }
    
        s = e + size_split;
    }

    if(s < (src+len_src)) {
        ranges_write[idx].location = s - src;
        ranges_write[idx].length = len_src - (s - src);
        idx ++;
    }

    return nfound;
}




/*string split.*/
long ustrsplit(const char* src, const char* split, int opt, 
        long n, ustrw_t strws[])
{
    uslog_check_arg(src != NULL,        0);
    uslog_check_arg(split != NULL,      0);
    uslog_check_arg(strlen(split) > 0,  0);
    uslog_check_arg(n > 0,              0);
    uslog_check_arg(NULL != strws,      0);

    long retn = 0;

    struct urange ranges[1024];
    struct urange* ranges_expand = NULL;
    long nfound = _ustrsplit(src, split, opt, 
        1024, ranges, &ranges_expand);
    retn = nfound;
    struct urange* ranges_use = ranges_expand?ranges_expand:ranges;

    long idx = 0;
    for(idx=0; idx<n&&idx<nfound; idx++) {
        um_strncpy(strws[idx].wrs, strws[idx].size, src+ranges_use[idx].location, ranges_use[idx].length);
    }

    if(ranges_expand) {
        um_free(ranges_expand);
        ranges_expand = NULL;
    }

    retn = nfound;
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


char* ustrchrs(char* s, char* chs)
{
    uslog_check_arg(s != NULL, NULL);
    uslog_check_arg(chs != NULL, NULL);

    char* t = s;
    int len = strlen(chs);
    while(t && *t) {
        int idx;
        for(idx=0; idx<len; idx++) {
            if(*t == chs[idx]) {
                return t;
            }
        }

        t ++;
    }
        
    return NULL;
}


/* return the range in haystack, from needle_start, to needle_end.(include start, end).  */
struct urange ustr_range(const char *haystack, 
        const char *needle_start, const char* needle_end)
{
    struct urange range = {-1, 0};

    uslog_check_arg(haystack != NULL, range);
    uslog_check_arg(needle_start != NULL, range);
    uslog_check_arg(needle_end != NULL, range);

    const char* tmp0 = strstr(haystack, needle_start);
    if(NULL != tmp0) {
        const char* tmp1 = strstr(tmp0 + strlen(needle_start), needle_end);
        if(NULL != tmp1) {
            range.location = tmp0 - haystack;
            range.length = tmp1 - tmp0 + strlen(needle_end);
        }
    }
    
    return range;
}


long ustr_ranges(const char *haystack, 
        const char *needle_start, const char* needle_end,
        struct urange ranges[], long n)
{
    uslog_check_arg(haystack != NULL        , 0);
    uslog_check_arg(needle_start != NULL    , 0);
    uslog_check_arg(needle_end != NULL      , 0);
    uslog_check_arg(ranges != NULL          , 0);
    uslog_check_arg(n > 0                   , 0);

    long nret = 0;
    long idx = 0;
    const char* t = haystack;
    long offset;
    while(1) {
        struct urange range = ustr_range(t, needle_start, needle_end);
        offset = t - haystack;
        if(range.location != -1 && range.length != 0) {
            t += (range.location + range.length);

            range.location += offset;
            nret ++;

            if(idx < n) {
                ranges[idx] = range;
                idx ++;
            }
        }
        else {
            break;
        }
    }

    return nret;
}








/* use um_free(p) to free the return str->s. */
int ustr_replace_ranges(const char* s, 
        const char* to, 
        int nranges, 
        struct urange* ranges, 
    	ustr_t* str)
{
    int ret = 0;

    str->s = NULL;
    str->len = 0;
    str->capacity = 0;

    size_t len_s        = strlen(s);
    size_t len_to       = strlen(to);
    struct ucbuf bufs_replace[1];
    bufs_replace[0].p       = to;
    bufs_replace[0].size    = len_to;

    /* '\0' need to make dest got a \0 terminal. */
    void* dest = NULL;
    size_t len_dest = 0;
    ret = ubuffer_replace(s, len_s+1, 
                &dest, &len_dest, 
                ranges, nranges,
                bufs_replace, 1);
    if(0 == ret) {
        str->s = (char*)dest;
        str->len = len_dest-1;
        str->capacity = len_dest;
    }

    return ret;
}


int ustr_replace_members(const char* s, 
        int nmember, 
        struct ustr_replace_member members[], 
    	ustr_t* str)
{
    int ret = 0;

    if(nmember <= 0) {
        str->s = ustrdup(s);
        if(str->s) {
            str->len= strlen(str->s);
            str->capacity = str->len+ 1;
        }
        else {
            str->len= 0;
            str->capacity = 0;
        }

        return ret;
    }

    long lens = strlen(s);
    long new_length = 0;
    int idx;
    bool valid = true;
    for(idx=0; idx<nmember; idx++) {
        if(!(members[idx].range.location < lens
            && (members[idx].range.location + members[idx].range.length) <= lens)) {
            ulogerr("range invalid. total len: %ld. range:[%ld,%ld].\n", lens, members[idx].range.location, members[idx].range.length);
            valid = false;
        }

        if(idx>0) {
            if(!(members[idx].range.location >= (members[idx-1].range.location + members[idx-1].range.length))) {
                ulogerr("range invalid. total len: %ld. range:[%ld,%ld], prev[%ld,%ld].\n", lens, 
                        members[idx].range.location, members[idx].range.length, 
                        members[idx-1].range.location, members[idx-1].range.length);
                valid = false;
                break;
            }
        }

        if(idx == 0) {
            new_length += members[idx].range.location;
            new_length += members[idx].to_size;
        }
        else {
            new_length += (members[idx].range.location - (members[idx-1].range.location + members[idx-1].range.length));
            new_length += members[idx].to_size;
        }

        if(idx == (nmember-1) && lens > (members[idx].range.location + members[idx].range.length)) {
            new_length += (lens - (members[idx].range.location + members[idx].range.length));
        }
    }

    if(!valid) {
        ret = -1;
        return ret;
    }

    str->s = um_malloc(new_length+1);
    if(!str->s) {
        ret = -1;
        return ret;
    }

    str->s[new_length] = '\0';
    new_length = 0;
    for(idx=0; idx<nmember; idx++) {
        if(idx == 0) {
            memcpy(str->s+new_length, s, members[idx].range.location);
            new_length += members[idx].range.location;
        }
        else {
            memcpy(str->s+new_length, s + (members[idx-1].range.location + members[idx-1].range.length), 
                    (members[idx].range.location - (members[idx-1].range.location + members[idx-1].range.length)));
            new_length += (members[idx].range.location - (members[idx-1].range.location + members[idx-1].range.length));
        }

        memcpy(str->s+new_length, members[idx].to, members[idx].to_size);
        new_length += members[idx].to_size;

        if(idx == (nmember-1) && lens > (members[idx].range.location + members[idx].range.length)) {
            memcpy(str->s+new_length, s+(members[idx].range.location + members[idx].range.length), (lens - (members[idx].range.location + members[idx].range.length)));
            new_length += (lens - (members[idx].range.location + members[idx].range.length));
        }
    }

    str->len = strlen(str->s);
    str->capacity = str->len + 1;

    return ret;
}


















/* use um_free(p) to free the return str->s. */
int ustr_replaces(const char* s, 
        const char* needle, const char* to, 
    	ustr_t* str)
{
    int ret = 0;

    str->s = NULL;
    str->len = 0;
    str->capacity = 0;

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
    else if(nfind == 0) {
        str->s = ustrdup(s);
        str->len = strlen(str->s);
        str->capacity = str->len+1;
    }
    else {
        ret = ustr_replace_ranges(s, to, nfind, ranges, str);
    }
    um_free_check(ranges);

    return ret;
}


/*
    string s, move valid range to left with distance n. 
 */
char* ustr_move_left(char* s, struct urange range_move, long n)
{
    assert(s);

    if(n > 0 && (long)(range_move.location) >= n) {
        int idx;
        for(idx=0; idx<range_move.length; idx++) {
            s[range_move.location-n+idx] = s[range_move.location+idx];
        }
    }

    return s;
}


/*
    reuse : use previous str. 
    alloc : return alloced string in ustr_t;
    write : write to dest c string.
 */
int ustr_replaces_reuse(char* s, size_t size, const char* needle, const char* to)
{
    int ret = 0;

    size_t len_needle = strlen(needle);
    size_t len_to = strlen(to);
    size_t len_s = strlen(s);
    long len_diff = len_needle - len_to;

    if(len_needle == len_to) {
        char* t = s;
        while(1) {
            char* tmp = strstr(t, needle);
            if(!tmp) {
                break;
            }

            memcpy(tmp, to, len_to);

            t = tmp + len_to;
        }
    }
    else if(len_needle > len_to) {
        int nfound = 0;
        struct urange range = {0, 0};
        struct urange range_move = {0, 0};
        char* tread = s;
        while(1) {
            char* tmp = strstr(tread, needle);
            if(!tmp) {
                break;
            }

            nfound ++;

            if(nfound == 1) {
                range.location = tmp - s;
                range.length = len_needle;
                memcpy(tmp, to, len_to);
            }
            else {
                range_move.location = range.location + range.length;
                range_move.length = tmp - s - (range.location + range.length);
                ustr_move_left(s, range_move, (nfound-1)*len_diff);

                memcpy(tmp-(nfound-1)*len_diff, to, len_to);
                range.location = tmp - s;
                range.length = len_needle;
            }

            tread = tmp + len_needle;
        }

        if(nfound > 0) {
            if(len_s > (range.location + range.length)) {
                range_move.location = range.location + range.length;
                range_move.length = len_s - (range.location + range.length);
                ustr_move_left(s, range_move, nfound*len_diff);
            }
        }

        s[len_s - nfound*len_diff] = '\0';
    }
    else {
        ustr_t str;
        ret = ustr_replaces(s, needle, to, &str);
        if(0 == ret) {
            ustrcpy(s, size, str.s);
            um_free(str.s);
        }
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







































































































































































































































































































































































































































































































































































#ifdef TEST_USTR_REPLACES_REUSE
int test_ustr_replaces_reuse(void)
{
    int ret = 0;
    char* s = "0123abc456abcabcab789abcab";
    uloginf("s : [%s]\n", s);

    const char* needle;
    const char* to;
    char s0[1024];
    const char* s0_expected;

    /* found 0. */
    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "abcd";
    to = "xxx";
    s0_expected = "0123abc456abcabcab789abcab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    /* found 1 - left. */
    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "0123";
    to = "xxx";
    s0_expected = "xxxabc456abcabcab789abcab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "0123";
    to = "xyzo";
    s0_expected = "xyzoabc456abcabcab789abcab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "0123";
    to = "xyzoplkjhgfdsa";
    s0_expected = "xyzoplkjhgfdsaabc456abcabcab789abcab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    /* found 1 - middle. */
    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "456";
    to = "xxx";
    s0_expected = "0123abcxxxabcabcab789abcab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "456";
    to = "xy";
    s0_expected = "0123abcxyabcabcab789abcab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "456";
    to = "xyzoplkjhgfdsa";
    s0_expected = "0123abcxyzoplkjhgfdsaabcabcab789abcab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    /* found 1 - right. */
    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "9abcab";
    to = "123456";
    s0_expected = "0123abc456abcabcab78123456";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "9abcab";
    to = "xy";
    s0_expected = "0123abc456abcabcab78xy1";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "9abcab";
    to = "9xyzoplkjhgfdsa";
    s0_expected = "0123abc456abcabcab789xyzoplkjhgfdsa";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));

    /* found n. */
    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "abc";
    to = "xxx";
    s0_expected = "0123xxx456xxxxxxab789xxxab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    assert(0 == strcmp(s0, s0_expected));
    
    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "abc";
    to = "xy";
    s0_expected = "0123xy456xyxyab789xyab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    uloginf("s0 then : [%s]\n", s0);
    assert(0 == strcmp(s0, s0_expected));

    memset(s0, 'u', 1024);
    strcpy(s0, s);
    needle = "abc";
    to = "";
    s0_expected = "0123456ab789ab";
    ustr_replaces_reuse(s0, 1024, needle, to);
    uloginf("s0 then : [%s]\n", s0);
    assert(0 == strcmp(s0, s0_expected));

    return ret;
}



#endif




#ifdef TEST_SPLIT 
void test_split()
{
    const char* s = "a12aa456a7890";

#define NUM 100
#define LEN 1024
    char output[NUM][LEN];
    ustrw_t strs[NUM];

    int idx;
    for(idx=0; idx<NUM; idx++) {
        strs[idx].wrs = output[idx];
        strs[idx].size = LEN;
    }
    
    long n = NUM;
    //int opt = 0;
    //int opt = USTRSPLIT_INCLUDE_0LENGTH;
    //int opt = USTRSPLIT_INCLUDE_SPLIT;
    int opt = USTRSPLIT_INCLUDE_0LENGTH | USTRSPLIT_INCLUDE_SPLIT;
    int nfound = ustrsplit(s, "a", opt, 
            n, strs);

    for(idx=0; idx<nfound; idx++) {
        uloginf("%d : [%s]\n", idx, output[idx]);
    }
}


#endif
