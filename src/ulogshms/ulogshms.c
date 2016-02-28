#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ufile.h"
#include "ustandard/ulogshms.h"
#define LOGSHMS_HEADER_LEN      2
#define LOGSHMS_HEADER_PRINT    "@#"
#define LOGSHMS_HEADER_LINE     "@L"
#define LOGSHMS_HEADER_DATA     "@D"
struct ulogshms_info {
    size_t nblock;
    size_t size_block;

    size_t idx_block;
    size_t idx;
    size_t finish_block;

    int reading;
};

struct ulogshms {
    key_t   key;
    size_t  size;
    int     shm_id;
    void*   shm_addr;

    struct ulogshms_info     info;
    struct ulogshms_info*    shm_info;
    void* addr;
};

static void* _shm_create(key_t key, size_t size, int* pshm_id);
static int _shm_destroy(int shm_id, void* addr);
#define SIZE_INFO   1024


struct ulogshms* ulogshms_create(key_t key, size_t size, int nblock)
{
    struct ulogshms* s = um_malloc(sizeof(*s));
    if(NULL != s) {
        void* shm_addr = _shm_create(key, size, &s->shm_id);
        if(NULL != shm_addr) { 
            s->key = key;
            s->size = size;
            s->shm_addr = shm_addr; 
            memset(s->shm_addr, 0, s->size);

            s->info.nblock          = nblock;
            s->info.size_block      = (size-SIZE_INFO) / nblock;
            s->info.idx_block       = 0;
            s->info.idx             = 0;
            s->info.finish_block    = 0;
            s->info.reading         = 0;

            s->shm_info = s->shm_addr;
            memcpy(s->shm_info, &s->info, sizeof(struct ulogshms_info));
            s->addr = (void*)(s->shm_addr) + SIZE_INFO;
            ulogdbg("s->shm_addr=%p, s->addr=%p.\n", s->shm_addr, s->addr);
        }
        else {
            free(s);
            s = NULL;
        }
    }

    return s;
}


int ulogshms_destroy(struct ulogshms* s)
{
    int ret = 0;

    if(NULL != s) {
        _shm_destroy(s->shm_id, s->shm_addr);
        s->shm_id = -1;
        s->shm_addr = NULL;

        free(s);
        s = NULL;
    }

    return ret;
}


int _ulogshms_p(const void* p, size_t size)
{
    int ret = 0;
    fprintf(stdout, "%s", "======");
    fprintf(stdout, "%c", *(const char*)p);
    fprintf(stdout, "%c", *(const char*)(p+1));
    const unsigned char* s = (const unsigned char*)p + LOGSHMS_HEADER_LEN;
    unsigned short int n = ((*s) << 8) | (*(s+1));
    fprintf(stdout, " len%d", n);
    fprintf(stdout, "%c ======\n", *(const char*)(p+LOGSHMS_HEADER_LEN+2));
    
    return ret;
}


static int ulogshms_add(struct ulogshms* s, const void* p, size_t size)
{
    int ret = 0;

    //_ulogshms_p(p, size);

    if(NULL != s && NULL != p && size <= s->info.size_block) {
        /* if client reading. wait at most 15ms. */
        int t = 3;
        while(s->shm_info->reading && t -- ) {
            usleep(5*1000);
        }
        //s->shm_info->reading = 0;

        if(size + s->info.idx > s->info.size_block) {
            memset(s->addr + s->info.idx_block*s->info.size_block + s->info.idx, 0, 
                    s->info.size_block - s->info.idx);

            s->info.finish_block ++;
            s->info.idx_block = (s->info.idx_block + 1) % s->info.nblock;
            s->info.idx = 0;

            s->shm_info->finish_block   = s->info.finish_block ;
            s->shm_info->idx_block      = s->info.idx_block ;
            s->shm_info->idx            = s->info.idx ;

            ulogdbg("write idx_block : %d.\n", s->info.idx_block);
        }

        ulogdbg("add at %p.\n", 
                s->addr + s->info.idx_block*s->info.size_block + s->info.idx);
        memcpy(s->addr + s->info.idx_block*s->info.size_block + s->info.idx, p, 
                size);
        s->info.idx += size;
        s->shm_info->idx = s->info.idx;
    }
    else {
        ulogerr("ulogshms_add error.\n");
    }

    struct ulogshms_info* shm_info = s->shm_info;
    ulogdbg("shm_info : nblock=%u, size_block=%u, idx_block=%u, idx=%u, finish_block=%u.\n", shm_info->nblock, shm_info->size_block, shm_info->idx_block, shm_info->idx, shm_info->finish_block);

    return ret;
}


int ulogshms_printf(struct ulogshms* s, const char* format, ...)
{
    int retn = 0;

    #define LEN_STR 1024
    unsigned char str[LEN_STR] = LOGSHMS_HEADER_PRINT;

    va_list ap;
    va_start(ap, format);
    char* str_p = (char*)str+(LOGSHMS_HEADER_LEN+2);
    vsnprintf(str_p, LEN_STR-(LOGSHMS_HEADER_LEN+2), 
            format, ap);
    va_end(ap);

    unsigned short int n = strlen(str_p);
    str[LOGSHMS_HEADER_LEN] = ((n>>8)&0xff);
    str[LOGSHMS_HEADER_LEN+1] = (n&0xff);

    size_t wsize = strlen(str_p) + (LOGSHMS_HEADER_LEN+2);
    int ret = ulogshms_add(s, str, wsize);
    if(0 != ret) {
        retn = 0;
    }
    else {
        retn = wsize;
    }

    return retn;
}


int ulogshms_line(struct ulogshms* s, char name, unsigned short int line)
{
    int retn = 0;

    #define LEN_STR 1024
    unsigned char str[LEN_STR] = LOGSHMS_HEADER_LINE;
    unsigned short int n = line;
    str[LOGSHMS_HEADER_LEN] = ((n>>8)&0xff);
    str[LOGSHMS_HEADER_LEN+1] = (n&0xff);
    str[LOGSHMS_HEADER_LEN+2] = name;

    size_t wsize = LOGSHMS_HEADER_LEN+3;
    int ret = ulogshms_add(s, str, wsize);
    if(0 != ret) {
        retn = 0;
    }
    else {
        retn = wsize;
    }

    return retn;
}


int ulogshms_data(struct ulogshms* s, char name, const void* p, size_t size)
{
    int retn = 0;

    #define LEN_STR 1024
    unsigned char str[LEN_STR] = LOGSHMS_HEADER_DATA;
    unsigned short int n = um_min(size, LEN_STR-LOGSHMS_HEADER_LEN-3);
    str[LOGSHMS_HEADER_LEN] = ((n>>8)&0xff);
    str[LOGSHMS_HEADER_LEN+1] = (n&0xff);
    str[LOGSHMS_HEADER_LEN+2] = name;
    memcpy(str+LOGSHMS_HEADER_LEN+3, p, n);

    size_t wsize = LOGSHMS_HEADER_LEN+3+n;
    int ret = ulogshms_add(s, str, wsize);
    if(0 != ret) {
        retn = 0;
    }
    else {
        retn = wsize;
    }

    return retn;
}


void* _shm_create(key_t key, size_t size, int* pshm_id)
{
    void* addr = NULL;
    int shm_id;

    ulogdbg("key=0x%x, size=%d.\n", key, size);
    shm_id=shmget(key, size, 0600 | IPC_CREAT);

    if(shm_id==-1) {
        int eno = errno;
        ulogerr("shmget error. (shmget return shm_id = %d <%s>).\n", 
                shm_id, strerror(eno));
        *pshm_id = -1; 
        return NULL;
    }   

    addr = shmat(shm_id, NULL, 0); 
    if(NULL == addr) {
        int eno = errno;
        ulogerr("shmat error. (shmat return NULL. <%s>).\n", strerror(eno));
        *pshm_id = -1; 
        return NULL;
    }

    *pshm_id = shm_id;
    ulogdbg("shm_id = %d, addr = [%p,%p).\n", shm_id, addr, (void*)(addr+size));

    return addr;
}


int _shm_destroy(int shm_id, void* addr)
{
    int ret = 0;

    if(-1 != shm_id && NULL != addr) {
        shmdt(addr);
        addr = NULL;
        shmctl(shm_id, IPC_RMID, 0);
        shm_id = -1;
    }
    else {
        ret = -1;
    }

    return ret;
}


void* _shm_attach(key_t key, size_t size, int* pshm_id)
{
    void* addr = NULL;
    int shm_id;

    ulogdbg("key=0x%x, size=%d.\n", key, size);
    shm_id=shmget(key, size, 0600);

    if(shm_id==-1) {
        int eno = errno;
        ulogerr("shmget error. (shmget return shm_id = %d <%s>).\n", 
                shm_id, strerror(eno));
        *pshm_id = -1; 
        return NULL;
    }   

    addr = shmat(shm_id, NULL, 0); 
    if(NULL == addr) {
        int eno = errno;
        ulogerr("shmat error. (shmat return NULL. <%s>).\n", strerror(eno));
        *pshm_id = -1; 
        return NULL;
    }

    *pshm_id = shm_id;
    ulogdbg("shm_id = %d, addr = [%p,%p).\n", shm_id, addr, (void*)(addr+size));

    return addr;
}


int _shm_detach(void* addr)
{
    int ret = 0;
    ret = shmdt(addr);
    addr = NULL;
    return ret;
}


int umemcmp0(const void* p, size_t n)
{
    int ret = 0;
    int i;
    for(i=0; i<n; i++) {
        if(0 != *((unsigned char*)(p+i))) {
            ret = *((unsigned char*)(p+i));
        }
    }
    return ret;
}


static int ulogshmc_mem(const void* p, size_t size, 
        int (*cb_printf)(const void* p, size_t size), 
        int (*cb_line)(char name, unsigned short int line), 
        int (*cb_data)(char name, const void* p, size_t size))
{
    int ret = 0;

    ulogdbg("ulogshmc_mem %p, size%u.\n", p, size);
    const unsigned char* s = p;
    const unsigned char* s_end = p + size;
    unsigned short int n;
    char name;
    while(s<s_end) {
        if(0 == memcmp(s, LOGSHMS_HEADER_PRINT, LOGSHMS_HEADER_LEN)) {
            s+= LOGSHMS_HEADER_LEN;
            n = ((*s) << 8) | (*(s+1));
            s += 2;
            if(s_end - s >= n) {
                cb_printf(s, n);
                s += n;
            }
            else {
                ulogerr("n=%d, left=%d.\n", n, s_end - s);
            }
        }
        else if(0 == memcmp(s, LOGSHMS_HEADER_LINE, LOGSHMS_HEADER_LEN)) {
            s+= LOGSHMS_HEADER_LEN;
            n = ((*s) << 8) | (*(s+1));
            s += 2;
            name = *s; 
            s ++;
            cb_line(name, n);
        }
        else if(0 == memcmp(s, LOGSHMS_HEADER_DATA, LOGSHMS_HEADER_LEN)) {
            s+= LOGSHMS_HEADER_LEN;
            n = ((*s) << 8) | (*(s+1));
            s += 2;
            name = *s; 
            s ++;
            if(s_end - s >= n) {
                cb_data(name, s, n);
                s += n;
            }
            else {
                ulogerr("n=%d, left=%d.\n", n, s_end - s);
            }
        }
        else {
            if(*s == 0 && s < s_end && 0 == umemcmp0(s, s_end-s)) {
                ulogdbg("finish.\n");
                break;
            }
            else {    
                ulogerr("sign header unexpected<%c , %d>.\n", *s, *s);
                s ++;
            }
        }
    }

    return ret;
}


int ulogshmc_func_printf(const void* p, size_t size)
{
    int ret = 0;

    if(NULL != p && size > 0) {
        fwrite(p, 1, size, stdout);
        if(*(const unsigned char*)(p+size-1) != '\n') {
            fprintf(stdout, "%s", "\n");
        }
    }
    
    return ret;
}


int ulogshmc_func_line(char name, unsigned short int line)
{
    int ret = 0;

    fprintf(stdout, "file : %c, line : %6d .\n", name, line);

    return ret;
}


int ulogshmc_func_data(char name, const void* p, size_t size)
{
    int ret = 0;

    fprintf(stdout, "data : %c, size : %6d .\n", name, size);

    return ret;
}


int ulogshmc(key_t key, size_t size, 
        int (*cb_printf)(const void* p, size_t size), 
        int (*cb_line)(char name, unsigned short int line), 
        int (*cb_data)(char name, const void* p, size_t size))
{
    int ret = 0;

    cb_printf   = NULL!=cb_printf?cb_printf:ulogshmc_func_printf;
    cb_line     = NULL!=cb_line?cb_line:ulogshmc_func_line;
    cb_data     = NULL!=cb_data?cb_data:ulogshmc_func_data;

    void* content = um_malloc(size);
    if(NULL == content) {
        ret = -1;
        goto finish;
    }

    int shm_id = -1;
    void* addr_shm = _shm_attach(key, size, &shm_id);
    if(NULL == addr_shm) {
        ulogerr("_shm_attach.\n");
        ret = -1;
        free(content);
        content = NULL;
        goto finish;
    }

    struct ulogshms_info* shm_info;
    shm_info = addr_shm;
    ulogdbg("shm_info : nblock=%u, size_block=%u, idx_block=%u, idx=%u, finish_block=%u.\n", shm_info->nblock, shm_info->size_block, shm_info->idx_block, shm_info->idx, shm_info->finish_block);
    shm_info->reading = 1;
    memcpy(content, addr_shm, size);
    shm_info->reading = 0;
    _shm_detach(addr_shm);
    shm_id = -1;
    addr_shm = NULL;

    shm_info = content;
    ulogdbg("shm_info : nblock=%u, size_block=%u, idx_block=%u, idx=%u, finish_block=%u.\n", shm_info->nblock, shm_info->size_block, shm_info->idx_block, shm_info->idx, shm_info->finish_block);
    ulogdbg("content addr = %p.\n", content);

    int idxs;
    int idxe;
    if(shm_info->finish_block >= shm_info->nblock) {
        idxs = shm_info->finish_block - shm_info->nblock + 1;    
        idxe = shm_info->finish_block;
    }
    else {
        idxs = 0;
        idxe = shm_info->finish_block;
    }

    int i;
    for(i=idxs; i<idxe; i++) {
        ulogdbg("ulogshmc_mem idx %d.\n", i);
        ulogshmc_mem(content+SIZE_INFO+(i%shm_info->nblock)*shm_info->size_block, 
                shm_info->size_block, 
                cb_printf, cb_line, cb_data);
    }

    if(shm_info->idx > 0) {
        i = idxe;
        ulogdbg("ulogshmc_mem last idx %d.\n", i);
        ulogshmc_mem(content+SIZE_INFO+(i%shm_info->nblock)*shm_info->size_block, 
                shm_info->idx, 
                cb_printf, cb_line, cb_data);
    }

    free(content);
    content = NULL;

finish:
    return ret;
}





