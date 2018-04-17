#ifndef __USTRING_H__
#define __USTRING_H__
__BEGIN_DECLS


char *ustrncpy(char *dest, size_t size_dest, const char *src, size_t n);
char *ustrcpy(char *dest, size_t size_dest, const char *src);

/* return value should use um_free(p) to free, not free(p). */
char* ustrndup(const char* src, size_t n);

/* return value should use um_free(p) to free, not free(p). */
char* ustrdup(const char* src);

/* return value should use um_free(p) to free, not free(p). */
char* ustrdup_sub(const char* src, struct urange range);


char* ustrcat(char* dest, size_t size_dest, const char* src);
char* ustrncat(char* dest, size_t size_dest, const char* src, size_t n);
char* ustrcat_format(char* dest, size_t size_dest, const char* fmt, ...);



extern const char kchars_blank[];
extern const char kchars_crlf[];


char* ustrtrim_chars(char* s, bool trim_left, bool trim_rignt, 
        const char* chars);

char* ustrtrim(char* s);

/*
    as a input/output parameter, use um_free(str->s) to free the alloced memory. 
 */
struct ustr {
    char* s;
    size_t len;
    size_t capacity;
};
typedef struct ustr ustr_t;


/*
    1. const string. (with '\0').
    2. sub string of a c string. without '\0'.  
    do not need to free s. 
 */
struct ustrc {
    const char* cs;
    size_t      len;
};
typedef struct ustrc ustrc_t;


/*
    writable string with valid capacity size. 
 */
struct ustrw {
    char*       wrs;
    size_t      size;
};
typedef struct ustrw ustrw_t;













#define USTRSPLIT_INCLUDE_0LENGTH    1<<0
#define USTRSPLIT_INCLUDE_SPLIT      1<<1


long ustrsplit(const char* src, const char* split, int opt, 
        long n, ustrw_t strws[]);



int ustrsplit2(const char* src, const char* split, 
        char* s1, size_t size1,
        char* s2, size_t size2);




int ustrcmp_tailer(const char* s1, const char* s2);
int ustrncmp_tailer(const char* s1, const char* s2, size_t n);
int ustrcasecmp_tailer(const char* s1, const char* s2);
int ustrncasecmp_tailer(const char* s1, const char* s2, size_t n);
char ustrtail(const char* s);




/* if string last char equal to ch, cut it. usually used to cut \n if the reading line */
char* ustrcut_char_last(char* output, char ch);


/* find. */
/*
    return : found ranges. 
            -1: input value invalid. 
             0: not found.
           > 0: found number. it would not larger then nmax. so if return value equal to nmax, maybe there're more matched pattern.
 */
long ustr_find(const char* s, 
        const char* needle, 
        struct urange* range,
        long nmax);


char* ustrchrs(char* s, char* chs);


/* return the range in haystack, from needle_start, to needle_end.(include start, end).  */
struct urange ustr_range(const char *haystack, 
        const char *needle_start, const char* needle_end);


long ustr_ranges(const char *haystack, 
        const char *needle_start, const char* needle_end,
        struct urange ranges[], long n);


/* use um_free(p) to free the return str->s. */
int ustr_replace_ranges(const char* s, 
        const char* to, 
        int nranges, 
        struct urange* ranges, 
    	ustr_t* str);


/* use um_free(p) to free the return dest value. */
int ustr_replaces(const char* s, 
        const char* needle, const char* to, 
    	ustr_t* str);


int ustr_replaces_reuse(char* s, size_t size, const char* needle, const char* to);


struct ustr_replace_member {
    struct urange range;
    const char* to;
    size_t to_size;
};



int ustr_replace_members(const char* s, 
        int nmember, 
        struct ustr_replace_member members[], 
    	ustr_t* str);




__END_DECLS
#endif /* usting.h */
