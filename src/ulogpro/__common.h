#define __min(x, y) (((x)<=(y))?(x):(y))


#define __strcpy(dest, size_dest, src)                              \
do{                                                                 \
    memset(dest, 0, size_dest);                                     \
    strncpy(dest, src, size_dest-1);                                \
}while(0)


#define __strncpy(dest, size_dest, src, n)                          \
do{                                                                 \
    memset(dest, 0, size_dest);                                     \
    strncpy(dest, src, __min(size_dest-1, n));                      \
}while(0)


#define __debug_printf(x...)                                        \
do{                                                                 \
    printf("[%30s, %36s]debug : ", __FILE__, __FUNCTION__);         \
    printf(x);                                                      \
}while(0)

#undef __debug_printf
#define __debug_printf(x...)


#define __error_printf(x...)                                        \
do{                                                                 \
    printf("[%30s, %36s]error : ", __FILE__, __FUNCTION__);         \
    printf(x);                                                      \
}while(0)


static int _ucharcmp(char ch, char chs_split[], int n_chs_split)
{
    int ret = -1;

    int i;
    for(i=0; i<n_chs_split; i++) {
        if(ch == chs_split[i]) {
            ret = 0;
            break;
        }
    }

    return ret;
}


static int _ustrpart(const char* str, char chs_split[], int n_chs_split)
{
    int length = 0;
    __debug_printf("str=[%s]\n", str);
    const char* tmp = str;
    while(*tmp != '\0' && _ucharcmp(*tmp, chs_split, n_chs_split)) {
        tmp ++;
        length ++;
    }

    return length;
}


static void _ustrpass(const char** pstr, char chs_split[], int n_chs_split)
{
    __debug_printf("_strpass before [%s]\n", *pstr);
    while(0 == _ucharcmp(**pstr, chs_split, n_chs_split)) {
        (*pstr) ++ ;
    }
    __debug_printf("_strpass after  [%s]\n", *pstr);
}


long _ufile_length(const char* pathname)
{
    return (1024*10);
}


size_t _ufile_copy_to_buffer(const char* path, char* content, size_t size)
{
    FILE* fp = fopen(path, "r");
    if(NULL == fp) {
        return 0;
    }

    fread(content, 1, size, fp);
    fclose(fp);

    return size;
}
