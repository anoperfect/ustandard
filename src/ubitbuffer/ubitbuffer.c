#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ubitbuffer.h"
int ubit_get(const void* ptr, unsigned int idx)
{
    int retb = 0;

    unsigned int uchar_idx = idx / 8 ;
    unsigned int bit_idx = (8 - 1) - idx % 8 ;
    unsigned char uch = ((unsigned char*)ptr)[uchar_idx];
    retb = uch & (1<<bit_idx) ? 1 : 0;

    return retb;
}


uint64_t ubit_gets(const void* ptr, unsigned int idx, unsigned int len)
{
    uint64_t retv = 0;

    int i;
    for(i=0; i<len; i++) {
        retv = (retv << 1) | ubit_get(ptr, idx + i);
    }

    return retv;
}


int ubit_set(void* ptr, unsigned int idx, int v01)
{
    int ret = 0;

    unsigned int uchar_idx = idx / 8 ;
    unsigned int bit_idx = (8 - 1) - idx % 8 ;
    unsigned char* puch = (void*)ptr + uchar_idx;
    if(v01) {
        *puch = (*puch) | (1<<bit_idx);
    }
    else {
        *puch = (*puch) & (~(1<<bit_idx));
    }
    
    return ret;
}


int ubit_sets(void* ptr, unsigned idx, unsigned int len, uint64_t v)
{
    int ret = 0;

    int i;
    for(i=0; i<len; i++) {
        ubit_set(ptr, idx+i, (v>>(len-i-1)) & 0x1);
    }

    return ret;
}
















int ubit_get_by_length(const void* ptr, size_t size, int n, int lens[])
{
    int ret = 0;

    uint64_t r = 0;
    int i;
    int j;
    unsigned int idx = 0;
    for(i=0; i<n; i++) {
        if(idx + lens[i] > size*8) {
            break;
        }
        r = 0;
        for(j=0; j<lens[i]; j++) {
            r = (r<<1) | ubit_get(ptr, idx + j);           
        }
        idx += lens[i];
    } 

    return ret;
}



struct ubit_parse {
    const void*     ptr;
    size_t          size;
    unsigned int    idx;
};


struct ubit_parse* ubit_parse_set(const void* ptr, size_t size)
{
    struct ubit_parse* retp = NULL;

    struct ubit_parse* parse = NULL;
    size_t size_alloc = sizeof(*parse);
    parse = um_malloc(size_alloc);
    if(NULL == parse) {
        retp = NULL; 
        return retp;
    }
    memset(parse, 0, size_alloc); 

    parse->ptr      = ptr;
    parse->size     = size;
    parse->idx      = 0;
    
    retp = parse;

    return retp;
}


int ubit_parse_reset(struct ubit_parse* parse, const void* ptr, size_t size)
{
    int ret = 0;

    /* ## check NULL != parse. */

    parse->ptr      = ptr;
    parse->size     = size;
    parse->idx      = 0;

    return ret;
}


int64_t ubit_parse_r(struct ubit_parse* parse, const char* name, size_t length, 
        ubit_output_type_e type, 
        char* dest, size_t size)
{
    int64_t retv = 0;

    ulognon("parse->idx : %u\n", parse->idx);
    if(0 == parse->idx % 8) {
        uloginf("------parse <%s>.\n", name);
        uloginf("[%x] \n", ((unsigned char*)parse->ptr)[parse->idx/8]);
        uloginf("[%x] \n", ((unsigned char*)parse->ptr)[parse->idx/8 + 1]);
        uloginf("[%x] \n", ((unsigned char*)parse->ptr)[parse->idx/8 + 2]);
    }
    if(parse->idx + length <= parse->size * 8) {
        int i;
        uint64_t value = 0;
        int n;

        snprintf(dest, size, "%36s : ", name);
        n = strlen(dest);
        dest += n;
        size -= n;

        for(i=0; i<length; i++) {
            value = (value << 1) + ubit_get(parse->ptr, parse->idx + i);
        }

        switch(type) {
            case ubit_output_type_2:
            for(i=0; i<length && i<size; i++) {
                dest[i] = ubit_get(parse->ptr, parse->idx + i)?'1':'0';
            }
            dest[i] = '\0';
            break;

            case ubit_output_type_16:
            snprintf(dest, size, "0x%llx", (unsigned long long int)value);
            break;

            case ubit_output_type_10:
            snprintf(dest, size, "%llu", (unsigned long long int)value);
            break;

            default:
            ulogerr("not defined.\n");
            break;
        }

        parse->idx += length;
        retv = value;
    }
    else {
        ulogerr("not enough.\n");
        retv = -1;
    }
    
    return retv;
}


int64_t ubit_parse(struct ubit_parse* parse, const char* name, size_t length, 
        ubit_output_type_e type)
{
    int64_t retv = 0;

    #define LEN_STR_OUTPUT  128
    char str[LEN_STR_OUTPUT];
    retv = ubit_parse_r(parse, name, length, type, str, LEN_STR_OUTPUT);
    if(-1 != retv) {
        printf("%s.\n", str);
    }

    return retv;
}


int ubit_parse_skip(struct ubit_parse* parse, unsigned int n)
{
    int ret = 0;
    parse->idx += n;
    return ret;
}


int ubit_parse_finish(struct ubit_parse* parse)
{
    int ret = 0;

    if(NULL != parse) {
        um_free(parse);
        parse = NULL;
    }

    return ret;
}


struct ubit_trans01_padding {
    char* border_01     ;

    char* prefix_uchars ;
    char* border_uchars ;
    char* suffix_uchars ;

    char* prefix_uint  ;
    char* border_uint   ;
    char* suffix_uint   ;
}kpadding = {"", "[ ", " ", "]", "[ ", " ", "]"};

#define UBIT_TRANS01_UCHAR_LENGTH (8 + (8-1)*strlen(kpadding.border_01))

int ubit_trans01_uchar(char* dest, size_t size, unsigned char uv)
{
    int retn = 0;

    ulogdbg("uv = %d\n", uv);
    size_t sizeb = 8;
    size_t size_border = strlen(kpadding.border_01);
    if(size>UBIT_TRANS01_UCHAR_LENGTH) {
        int i;
        int idx_dest = 0;
        for(i=0; i<sizeb; i++) {
            if((sizeb-1)!=i) {
                dest[idx_dest] = ((uv>>(sizeb-1-i))&0x1) + '0';
                memcpy(dest + (idx_dest + 1), kpadding.border_01, size_border);
                idx_dest += (size_border + 1) ;
            }
            else {
                dest[idx_dest] = ((uv>>(sizeb-1-i))&0x1) + '0';
                dest[idx_dest+1] = '\0';
            }
        }
        retn = (sizeb + (sizeb-1)*size_border);
    }
    else {
        usloge_perror("dest not enough");
        retn = 0;
    }

    return retn;
}


int ubit_trans01(char* dest, size_t size, const void* ptr, size_t n)
{
    int ret = 0;

    char*s = dest;
    size_t size_s = size;
    int nprintf;
    int i;
    
    snprintf(s, size_s, "%s", kpadding.prefix_uchars); 
    nprintf = strlen(s);
    s += nprintf; 
    size_s -= nprintf;

    for(i=0; i<n; i++) {
        unsigned char uch = ((const unsigned char*)ptr)[i];

        int n01 = ubit_trans01_uchar(s, size_s, uch);
        if(0 != n01) {
            s += n01;
            size_s -= n01;
        }
        else {
            break;
        }

        if(i != (n-1)) {
            snprintf(s, size_s, "%s", kpadding.border_uchars);
            nprintf = strlen(s);
            s += nprintf;
            size_s -= nprintf;
        }

        ret = 0;
    }

    snprintf(s, size_s, "%s", kpadding.suffix_uchars); 
    nprintf = strlen(s);
    s += nprintf; 
    size_s -= nprintf;

    return ret;
}


int ubit_trans01_uint(char* dest, size_t size, unsigned int uv)
{
    int ret = 0;

    size_t sizev = sizeof(uv);
    int i;

    int nprintf;
    char* s = dest;
    size_t size_s = size;

    snprintf(s, size_s, "%s", kpadding.prefix_uint);
    nprintf = strlen(s);
    s += nprintf;
    size_s -= nprintf;

    for(i=0; i<sizev; i++) {
        if(size_s > UBIT_TRANS01_UCHAR_LENGTH) {
            int n01 = ubit_trans01_uchar(s, size_s, (uv>> ((sizev-1)*8))&0xff);
            s += n01;
            size_s -= n01;
            
            if(i != (sizev-1)) {
                snprintf(s, size_s, "%s", kpadding.border_uint);
                nprintf = strlen(s);
                s += nprintf;
                size_s -= nprintf;
            }
        }
        else {
            break;
        }
    }

    snprintf(s, size_s, "%s", kpadding.suffix_uint);
    nprintf = strlen(s);
    s += nprintf;
    size_s -= nprintf;
    
    return ret;
}


int ubit_trans01_uint64(char* dest, size_t size, uint64_t uv)
{
    int ret = 0;

    size_t sizev = sizeof(uv);
    int i;

    int nprintf;
    char* s = dest;
    size_t size_s = size;

    snprintf(s, size_s, "%s", kpadding.prefix_uint);
    nprintf = strlen(s);
    s += nprintf;
    size_s -= nprintf;

    for(i=0; i<sizev; i++) {
        if(size_s > UBIT_TRANS01_UCHAR_LENGTH) {
            int n01 = ubit_trans01_uchar(s, size_s, (uv>> ((sizev-1-i)*8))&0xff);
            s += n01;
            size_s -= n01;
            
            if(i != (sizev-1)) {
                snprintf(s, size_s, "%s", kpadding.border_uint);
                nprintf = strlen(s);
                s += nprintf;
                size_s -= nprintf;
            }
        }
        else {
            break;
        }
    }

    snprintf(s, size_s, "%s", kpadding.suffix_uint);
    nprintf = strlen(s);
    s += nprintf;
    size_s -= nprintf;

    return ret;
}









int ubit_trans01_with_alloc(char** ppdest, const void* ptr, size_t n)
{
    int ret = 0;

    size_t size_alloc = n * 8 *2 + 8;
    *ppdest = um_malloc(size_alloc);
    if(NULL == *ppdest) {
        ret = -1;
        return ret;
    }
    memset(*ppdest, 0, size_alloc);

    ret = ubit_trans01(*ppdest, size_alloc, ptr, n);
    return ret;
}


int ubit_get_by_length1(void* ptr, size_t size_ptr, int n, int lens[])
{
    int ret = 0;

    char* strbit = NULL;
    ubit_trans01_with_alloc(&strbit, ptr, size_ptr);

    char* s = strbit;
    uint64_t r = 0;
    int i;
    int j;
    for(i=0; i<n; i++) {
        r = 0;
        for(j=0; j<lens[i]; j++) {
            r = r*2 + s[j] ;           
        }
        s += lens[i];
        printf("[%d] 0x%llx\n", i, (unsigned long long int)r);
    }

    return ret;
}



