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





extern char    kchars_blank[];
extern size_t  kn_chars_blank;
extern char    kchars_crlf[];
extern size_t  kn_chars_crlf;

#define UST_CHARS_BLANK         kchars_blank
#define UST_N_CHARS_BLANK       kn_chars_blank
#define UST_CHARS_CRLF          kchars_crlf
#define UST_N_CHARS_CRLF        kn_chars_crlf

char* ustrtrim_chars(char* s, int trim_left, int trim_rignt, 
        char chars[], size_t n);

char* ustrtrim(char* s);






int ustrsplit(const char* src, const char* split, 
        int n, char* dest[], size_t size_dest[]);

struct ustrsub {
    const char* s;
    size_t      size;
};


int ustrsplit_sub(const char* src, const char* split, 
        int n, struct ustrsub subs[]);


int ustrsplit2(const char* src, const char* split, 
        char* s1, size_t size1,
        char* s2, size_t size2);




int ustrcmp_tailer(const char* s1, const char* s2);
int ustrncmp_tailer(const char* s1, const char* s2, size_t n);
int ustrcasecmp_tailer(const char* s1, const char* s2);
int ustrncasecmp_tailer(const char* s1, const char* s2, size_t n);
char ustrtail(const char* s);




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















__END_DECLS
#endif /* usting.h */
