#include "ustandard/ustandard_sys.h"
#include "ustandard/umalloc.h"
#include "ustandard/uslog.h"
#include "ustandard/udebug.h"
struct umalloc_node {
    void*           ptr; 
    size_t          size; 
    const char*     file; 
    const char*     function; 
    unsigned int    line;

    struct umalloc_node* next;
};


/* linklist default header. */
static struct umalloc_node klist = {0};

/* set to enable monitor or not. */
static int kmonitor_on = 0;


static pthread_mutex_t klock = PTHREAD_MUTEX_INITIALIZER;
static int umalloc_lock(void)
{
    int ret = 0;
    ret = pthread_mutex_lock(&klock);
    return ret;
}


static int umalloc_unlock(void)
{
    int ret = 0;
    ret = pthread_mutex_unlock(&klock);
    return ret;
}


#define UMALLOC_ENABLE_LOG      0
#if UMALLOC_ENABLE_LOG
static FILE* kfp = NULL;
#define LEN_S   (10*1024)
#define __loglist()                                                                 \
do {                                                                                \
    int retn = 0;                                                                   \
    char s[LEN_S] = {0};                                                            \
    size_t idx = 0;                                                                 \
    size_t size = LEN_S;                                                            \
    struct umalloc_node* node = klist.next;                                         \
    while(NULL != node) {                                                           \
        int n = snprintf(s+idx, size-idx, "\t0x%08x %-10u   at %-36s,%-20s,%-4d\n", \
        (unsigned int)(node->ptr),node->size,node->file,node->function, node->line);\
        if(n >= size-idx-1) {                                                       \
            retn = size-1;                                                          \
            break;                                                                  \
        }                                                                           \
        else {                                                                      \
            idx += n;                                                               \
            retn += n;                                                              \
        }                                                                           \
        node = node->next;                                                          \
    }                                                                               \
    fprintf(kfp, "\t----------\n%s\t----------\n", s);                              \
    fprintf(stdout, "\t----------\n%s\t----------\n", s);                           \
}while(0);


#define __log(name, ptr, file, function, line)              \
do {                                                        \
    if(NULL == kfp) {                                       \
        kfp = fopen(".alloc.txt", "w");                     \
        setbuf(kfp, NULL);                                  \
        setbuf(stdout, NULL);                               \
    }                                                       \
    __loglist();                                            \
    fprintf(kfp, "----------%s : %8p %36s %36s %3u\n",      \
            name, ptr, file, function, line);               \
    fprintf(stdout, "----------%s : %8p %36s %36s %3u\n",   \
            name, ptr, file, function, line);               \
}while(0)


#else

#define __loglist()
#define __log(name, ptr, file, function, line)

#endif


static int umalloc_record_add(void* ptr, size_t size, 
        const char* file, const char* function, unsigned int line)
{
    int ret = 0; 

    umalloc_lock();
    
    __log("add", ptr, file, function, line);
    struct umalloc_node* node_new = malloc(sizeof(*node_new)); 
    if(NULL != node_new) {
        node_new->ptr = ptr; 
        node_new->size      = size; 
        node_new->file      = file; 
        node_new->function  = function; 
        node_new->line      = line;
        node_new->next      = klist.next;

        klist.next          = node_new;
    }
    else {
        ret = -1;
    }
    __loglist();

    umalloc_unlock();

    return ret;
}


static int umalloc_record_del(void* ptr, 
        const char* file, const char* function, unsigned int line)
{
    int ret = 0; 

    umalloc_lock();

    __log("del", ptr, file, function, line);
    struct umalloc_node* node = &klist;
    struct umalloc_node* node_free = NULL;
    while(NULL != node->next) {
        if(node->next->ptr == ptr) {
            node_free = node->next;
            break;
        }

        node = node->next;
    }
    if(NULL != node_free) {
        node->next = node_free->next;
        free(node_free);
        node_free = NULL;
    }
    else {
        ulogerr("ufree ptr not found.\n");
        ret = -1;
    }
    __loglist();

    umalloc_unlock();

    return ret;
}


static int umalloc_record_modify(void* ptr_new, size_t size_new, void* ptr, 
        const char* file, const char* function, unsigned int line)
{
    int ret = 0; 

    umalloc_lock();

    __log("del", ptr, file, function, line);
    __log("add", ptr_new, file, function, line);
    struct umalloc_node* node = klist.next;
    while(NULL != node) {
        if(node->ptr == ptr) {
            break;
        }
        node = node->next;
    }

    if(NULL != node) {
        node->ptr       = ptr_new; 
        node->size      = size_new; 
        node->file      = file; 
        node->function  = function; 
        node->line      = line;
    }
    else {
        ulogerr("urealloc ptr not found.\n");
        ret = -1;
    }
    __loglist();

    umalloc_unlock();

    return ret;
}


void* umalloc(size_t size, 
        const char* file, const char* function, unsigned int line)
{
    void* ptr = NULL;
    ptr = malloc(size);
    
    if(NULL != ptr) {
        if(kmonitor_on) {
            int ret = umalloc_record_add(ptr, size, file, function, line);
            if(0 != ret) {
                ulogerr("umalloc record add on %s: %s %d.\n", 
                        file, function, line);
            }
        }
    }
    else {
        ulogerr("umalloc error on %s: %s %d.\n", file, function, line);
    }

    return ptr;
}


int ufree(void* ptr, 
        const char* file, const char* function, unsigned int line)
{
    int ret = 0;

    if(NULL != ptr) {
        if(kmonitor_on) {
            ret = umalloc_record_del(ptr, file, function, line);
            if(0 != ret) {
                ulogerr("ufree %p error on %s: %s %d.\n", 
                        ptr,file, function, line);
            }
        }

        free(ptr);
        ptr = NULL;
    }
    else {
        ulogerr("ufree NULL pointer on %s: %s %d.\n", file, function, line);
        ret = -1;
    }

    return ret;
}


void* urealloc(void* ptr_ori, size_t size, 
        const char* file, const char* function, unsigned int line)
{
    void* ptr = NULL;
    ptr = realloc(ptr_ori, size);

    int ret = 0;
    if(NULL == ptr_ori) {
        if(NULL == ptr) {
            ulogerr("urealloc error on %s: %s %d.\n", file, function, line);
        }
        else {
            if(kmonitor_on) {
                ret = umalloc_record_add(ptr, size, file, function, line);
                if(0 != ret) {
                    ulogerr("umalloc record add on %s: %s %d.\n", 
                            file, function, line);
                }
            }
        }
    }
    else if(0 == size) {
        if(kmonitor_on) {
            ret = umalloc_record_del(ptr_ori, file, function, line);
            if(0 != ret) {
                ulogerr("ufree %p error on %s: %s %d.\n", 
                        ptr,file, function, line);
            }
        }
    }
    else {
        if(NULL == ptr) {
            ulogerr("urealloc error on %s: %s %d.\n", file, function, line);
        }
        else if(ptr == ptr_ori) {
            if(kmonitor_on) {
                umalloc_record_modify(ptr, size, ptr_ori, file, function, line);
            }
        }
        else {
            ulogdbg("urealloc addr changed.\n");
            if(kmonitor_on) {
                umalloc_record_modify(ptr, size, ptr_ori, file, function, line);
            }
        }
    }

    return ptr;
}


int umalloc_query(char* s, size_t size)
{
    int retn = 0;

    umalloc_lock();
    s[0] = '\0';
    size_t idx = 0;

    struct umalloc_node* node = klist.next;
    while(NULL != node) {
        int n = snprintf(s+idx, size-idx, "\t%p %-10u   at %-36s,%-20s,%-4d\n", 
                node->ptr, node->size, node->file, node->function, node->line);
        if(n >= size-idx-1) {
            retn = size-1;
            break;
        }
        else {
            idx += n;
            retn += n;
        }
        node = node->next;
    }
    umalloc_unlock();

    return retn;
}


int umalloc_query_clear(void)
{
    int ret = 0; 

    umalloc_lock();
    struct umalloc_node* node_free = NULL;
    while(NULL != klist.next) {
        node_free = klist.next;
        klist.next = klist.next->next;

        free(node_free);
        node_free = NULL;
    }
    umalloc_unlock();

    return ret;
}


int umalloc_enable(int set)
{
    int ret = 0;
    kmonitor_on = set;
    return ret;
}

