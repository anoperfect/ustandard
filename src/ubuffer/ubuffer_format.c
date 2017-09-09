#include "ustandard/ustandard.h"
#include "ustandard/udevelop.h"
#include "ustandard/ubuffer_format.h"
int ubuffer_format(char* dest, size_t size_dest, 
        const void* ptr, size_t size, ubuffer_char_type_e type, char space, int num_in_line)
{
    int ret = 0;
    dest[0] = '\0';
    if(NULL == ptr) {
        ulogerr("%s invalid arg.\n", __FUNCTION__);
        ret = -1;
        return ret;
    }

    if(type == e_ubuffer_char_type_default) {
        type = e_ubuffer_char_type_0x;
    }

    int n = 0;
    int idx = 0;
    int i;
    for(i=0; i<size; i++) {
        switch(type) {
            case e_ubuffer_char_type_0x:
            n = snprintf(dest+idx, size_dest-idx, "0x%02x%c", ((unsigned char*)ptr)[i], space);
            break;

            case e_ubuffer_char_type_0X:
            n = snprintf(dest+idx, size_dest-idx, "0X%02X%c", ((unsigned char*)ptr)[i], space);
            break;

            case e_ubuffer_char_type_0xprint:
            if(!isprint(((char*)ptr)[i])) {
                n = snprintf(dest+idx, size_dest-idx, "0x%02x%c", ((unsigned char*)ptr)[i], space);
            }
            else {
                n = snprintf(dest+idx, size_dest-idx, "%c   %c", ((unsigned char*)ptr)[i], space);
            }
            break;

            case e_ubuffer_char_type_print:
            n = snprintf(dest+idx, size_dest-idx, "%c%c", ((unsigned char*)ptr)[i], space);
            break;

            default:
            n = snprintf(dest+idx, size_dest-idx, "0x%02x%c", ((unsigned char*)ptr)[i], space);
            break;
        }

        if(n >= (size_dest-idx)) {
            break;
        }
        idx += n;

        if(i != (size-1) && 0 == (i+1)%num_in_line) {
            n = snprintf(dest+idx, size_dest-idx, "\n");
            if(n >= (size_dest-idx)) {
                break;
            }
            idx += n;
        }
    }

    return ret;
}


#define LEN_STR 1024
static char* ktostr = NULL;


/* ustandard_init inc. */
int ubuffer_tostr_init(void)
{
    int ret = 0;
    if(NULL == ktostr) {
        if(NULL == ktostr) {    
            ktostr = um_malloc(LEN_STR);
        }
    }
    
    return ret;
}


int ubuffer_tostr_deinit(void)
{
    int ret = 0;

    if(NULL != ktostr) {
        um_free(ktostr);
        ktostr = NULL;
    }
    
    return ret;
}


const char* ubuffer_tostr(const void* ptr, size_t size, 
        ubuffer_char_type_e type, 
        char space, int num_in_line)
{
    const char* retstr = NULL;

    ubuffer_tostr_init();
    if(NULL != ktostr) {
        ubuffer_format(ktostr, LEN_STR, ptr, size, type, space, num_in_line);
    }
    
    retstr = ktostr;
    return retstr;
}


const char* ubuffer_tostr_easy(const void* ptr, size_t size)
{
    const char* retstr = NULL;
    retstr = ubuffer_tostr(ptr, size, e_ubuffer_char_type_default, ' ', 16);
    return retstr;
}




