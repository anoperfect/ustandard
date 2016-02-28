#ifndef __UFANCTION_H__
#define __UFANCTION_H__
/*
    define some function. 
 */
#define um_min(x,y) (((x)<=(y))?(x):(y))
#define um_min3(x,y,z) (((((x)<=(y))?(x):(y)) <= (z))?(((x)<=(y))?(x):(y)):(z))

#define um_max(x,y) (((x)>=(y))?(x):(y))
#define um_max3(x,y,z) (((((x)>=(y))?(x):(y)) >= (z))?(((x)>=(y))?(x):(y)):(z))

#define um_align4(x)    (((x)+3) &~(3))



#define um_strcpy(dest, size, src)                                                  \
do{                                                                                 \
    size_t size_qweasdxrty = um_min((size)-1, strlen((src)));                       \
    strncpy((dest), (src), size_qweasdxrty);                                        \
    (dest)[size_qweasdxrty] = '\0';                                                 \
}while(0)


#define um_strncpy(dest, size, src, n)                                              \
do{                                                                                 \
    size_t size_qweasdxrty = um_min(n, strlen((src)));                              \
    size_qweasdxrty = um_min((size)-1, size_qweasdxrty);                            \
    strncpy((dest), (src), size_qweasdxrty);                                        \
    (dest)[size_qweasdxrty] = '\0';                                                 \
}while(0)


#define um_strcut_crlf(s)                                                           \
do{                                                                                 \
    size_t size_qweasdxrty = strlen((s));                                           \
    if(size_qweasdxrty>=2                                                           \
            && (s)[size_qweasdxrty-2] == '\r' && (s)[size_qweasdxrty-1] == '\n') {  \
        (s)[size_qweasdxrty-2] = '\0';                                              \
    }                                                                               \
    else if(size_qweasdxrty>=1 && (s)[size_qweasdxrty-1] == '\n') {                 \
        (s)[size_qweasdxrty-1] = '\0';                                              \
    }                                                                               \
}while(0)


/*
    copy src to dest with the strlen(dest) == len.
    so need to make sure dest aviliable size >= len+1.
    if strlen(src) < len, add blank prefix.
    if strlen(src) > len, add ... prefix.
 */
#define um_strcpy_prefix(dest, len, src)                                            \
do{                                                                                 \
    if((len) <= 3) {                                                                \
        memset((dest), '.', (len));                                                 \
    }                                                                               \
    else {                                                                          \
        size_t size_src1qwerty = strlen((src));                                     \
        if((len) >= size_src1qwerty) {                                              \
            memset((dest), ' ', (len)-size_src1qwerty);                             \
            strcpy((dest)+(len)-size_src1qwerty, (src));                            \
        }                                                                           \
        else {                                                                      \
            strcpy((dest), "...");                                                  \
            strncpy((dest)+3, (src)+size_src1qwerty-((len)-3), (len)-3);            \
            (dest)[(len)] = '\0';                                                   \
        }                                                                           \
    }                                                                               \
}while(0)

#define um_num_of_array_member(a) (sizeof(a)/sizeof((a)[0]))
#define um_utv_is_zero(x)   ((0 == ((x)->tv_sec)) && (0 == ((x)->tv_usec)))


#endif//__UFANCTION_H__
