#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <stdarg.h>
#include <sys/time.h>
#include <unistd.h>

#include "ustandard/uslog.h"
#define MAX_SLOG    8




/* config for different level. */
struct uslog_config {
    FILE* fp;
    unsigned int item;
    const char* prefix;
    const char* suffix;
};


/* uslog context. */
struct uslog_data {
    int inited;
    struct uslog_config configs[MAX_SLOG];
};


int _uslog_config_init(struct uslog_data* logctx)
{
    int ret = 0;

    int num_block = (MAX_SLOG/8);
    assert(num_block*8 == MAX_SLOG);
    int index_block;
    int level;

    /* set the default value for all level. */
    int i;
    for(i=0; i<MAX_SLOG; i++) {

        level = i;
        logctx->configs[level].fp = NULL;
        logctx->configs[level].item = 0;
        logctx->configs[level].prefix = "";
        logctx->configs[level].suffix = "";
    }

    /* config v0, test, info, debug, error. */
    for(index_block=0; index_block<num_block; index_block++) {

        level = index_block*8 + 0;
        logctx->configs[level].fp = NULL;
        logctx->configs[level].item = USLOG_ITEM_ENABLE_FILEINFO | USLOG_ITEM_ENABLE_PTHREAD_ID ;
        logctx->configs[level].prefix = " v0   : ";
        logctx->configs[level].suffix = "";

        level = index_block*8 + (USLOG_USTANDARD_TEST);
        logctx->configs[level].fp = stdout;
        logctx->configs[level].item = USLOG_ITEM_ENABLE_FILEINFO | USLOG_ITEM_ENABLE_PTHREAD_ID ;
        logctx->configs[level].prefix = " test : ";
        logctx->configs[level].suffix = "";

        level = index_block*8 + (USLOG_USTANDARD_INFO);
        logctx->configs[level].fp = stdout;
        logctx->configs[level].item = USLOG_ITEM_ENABLE_FILEINFO | USLOG_ITEM_ENABLE_PTHREAD_ID ;
        logctx->configs[level].prefix = " info : ";
        logctx->configs[level].suffix = "";

        level = index_block*8 + (USLOG_USTANDARD_DEBUG);
        logctx->configs[level].fp = NULL;
        logctx->configs[level].item = USLOG_ITEM_ENABLE_FILEINFO | USLOG_ITEM_ENABLE_PTHREAD_ID ;
        logctx->configs[level].prefix = " debug: ";
        logctx->configs[level].suffix = "";

        level = index_block*8 + (USLOG_USTANDARD_ERROR);
        logctx->configs[level].fp = stderr;
        logctx->configs[level].item = USLOG_ITEM_ENABLE_FILEINFO | USLOG_ITEM_ENABLE_PTHREAD_ID ;
        logctx->configs[level].prefix = " error: ";
        logctx->configs[level].suffix = "";
    }

    return ret;
}
/* blue: \e[34;1m , read \e[31;1m . end with "\e[0m" */

/* uslog_init would be called once. */
struct uslog_data* uslog_init(void)
{
    struct uslog_data* ret_logctx = NULL;
    struct uslog_data* logctx = NULL;
    logctx = (struct uslog_data*)malloc(sizeof(struct uslog_data));
    if(NULL != logctx) {
        memset(logctx, 0, sizeof(sizeof(struct uslog_config)*MAX_SLOG));
        _uslog_config_init(logctx);
    }

    ret_logctx = logctx;
    return ret_logctx;
}


int uslog_deinit(struct uslog_data* logctx)
{
    int ret = 0;

    if(NULL != logctx) {
        free(logctx);
        logctx = NULL;
    }

    return ret;
}


int uslog_config_setopt(struct uslog_data** logctx, int level, int tag, ...)
{
    int ret = 0;

    if(NULL == (*logctx)) {
        (*logctx) = uslog_init();
        if(NULL == (*logctx)) {
            fprintf(stderr, "%s\n", "uslog_init");
            ret = -1;
            return ret;
        }
    }

    if(0<=level && level<MAX_SLOG) {
        struct uslog_config* get_config = &(*logctx)->configs[level];    
        va_list arg;
        va_start(arg, tag);

        switch(tag) {
            case USLOG_CONFIG_OUTSTREAM:
            get_config->fp          = va_arg(arg, FILE*); 
            break;

            case USLOG_CONFIG_ITEM:
            get_config->item        = va_arg(arg, unsigned int);
            break;

            case USLOG_CONFIG_PREFIX:
            get_config->prefix      = va_arg(arg, const char*); 
            break;

            case USLOG_CONFIG_SUFFIX:
            get_config->suffix      = va_arg(arg, const char*); 
            break;

            default:
            break;
        }

        va_end(arg);
    }
    else {
        ret = -1;
    }

    return ret;
}


/* detect if this level enabled. 
   if level not in the range , or fp == NULL, it's disabled and do not need to output.*/
int uslog_is_enabled(struct uslog_data** logctx, int level)
{
    int retb = 0;

    if(NULL == (*logctx)) {
        (*logctx) = uslog_init();
        if(NULL == (*logctx)) {
            fprintf(stderr, "%s\n", "uslog_init");
            retb = 0;
            return retb;
        }
    }

    struct uslog_config* config = NULL;
    if(level >= 0 && level < MAX_SLOG) {
        config = &(*logctx)->configs[level];
        if(NULL != config->fp) {
            retb = 1;
        }
    }

    return retb;
}


/* find the pthread_t index. if the array full, return -1. */
struct pthread_id{
    pthread_t pth;
    int id;
};
#define NUM_PTHS    1024
static struct pthread_id kpths[NUM_PTHS];
static int kpths_num = 0;
static pthread_mutex_t kpths_lock = PTHREAD_MUTEX_INITIALIZER; 


int uget_pthread_id(void)
{
    pthread_mutex_lock(&kpths_lock);
    int id_ret = -1;
    pthread_t pth = pthread_self();

    int i;
    for(i=0; i<kpths_num; i++) {
        if(kpths[i].pth == pth) {
            id_ret = i;
            break;
        }
    }

    if(-1 == id_ret && kpths_num < NUM_PTHS) {
        id_ret = kpths_num;
        kpths_num ++;
        kpths[id_ret].pth = pth;
        kpths[id_ret].id = id_ret;
    }

    pthread_mutex_unlock(&kpths_lock);
    return id_ret;
}


/* used to count the cost time. */
static struct timeval ktv_uslog_start = {0, 0};


/* use the level config, output the string. */
int uslog_fprintf(struct uslog_data** logctx, int level, 
        const char* file, const char* function, unsigned int line,const char* s)
{
    int retn = 0;

    if(NULL == (*logctx)) {
        (*logctx) = uslog_init();
        if(NULL == (*logctx)) {
            fprintf(stderr, "%s\n", "uslog_init");
            retn = 0;
            return retn;
        }
    }

    int is_enabled = 0;
    struct uslog_config* config = NULL;
    if(level >= 0 && level < MAX_SLOG) {
        config = &(*logctx)->configs[level];
        if(NULL != config->fp) {
            is_enabled = 1;
        }
    }

    if(!is_enabled) {
        retn = 0;
        return retn;
    }

    #define LEN_STR     (10240)
    char str[LEN_STR];
    size_t len_str = 0;
    size_t n = 0;

    /* add filename, function, line . */
    if(config->item & USLOG_ITEM_ENABLE_FILEINFO) {
        snprintf(str+len_str, LEN_STR-len_str, 
                "[%30s, %36s, %6d]", file, function, line);
        n = strlen(str+len_str);
        len_str += n;
    }

    /* add pthread_t index, not pthread_t directly. */
    if(config->item & USLOG_ITEM_ENABLE_PTHREAD_ID) {
        snprintf(str+len_str, LEN_STR-len_str, "[%3d]", uget_pthread_id());
        n = strlen(str+len_str);
        len_str += n;
    }

    /* pid. */
    if(config->item & USLOG_ITEM_ENABLE_PID) {
        snprintf(str+len_str, LEN_STR-len_str, "[%4d]", getpid());
        n = strlen(str+len_str);
        len_str += n;
    }

    /* running timeval. 
       it's better to call uslog once at the beginning of the process. */
    if(config->item & USLOG_ITEM_ENABLE_TIMEVAL) {
        struct timeval tv;
        time_t      sec = 0;
        suseconds_t usec = 0;

        gettimeofday(&tv, NULL);
        if(ktv_uslog_start.tv_sec == 0 && ktv_uslog_start.tv_usec == 0) {
            ktv_uslog_start = tv; 
        }
        else {
            if(tv.tv_usec < ktv_uslog_start.tv_usec) {
                sec = tv.tv_sec - ktv_uslog_start.tv_sec -1;
                usec = tv.tv_usec + 1e6 - ktv_uslog_start.tv_usec;
            }
            else {
                sec = tv.tv_sec - ktv_uslog_start.tv_sec ;
                usec = tv.tv_usec - ktv_uslog_start.tv_usec;
            }
        }

        snprintf(str+len_str, LEN_STR-len_str, "[%4lu:%06lu]", sec, usec);
        n = strlen(str+len_str);
        len_str += n;
    }

    if(config->item & USLOG_ITEM_ENABLE_DATATIME) {
        time_t sec = time(NULL);
        struct tm t;
        memcpy(&t, localtime(&sec), sizeof(struct tm));
        n = strftime(str+len_str, LEN_STR-len_str, "[%F %T]", &t);
        len_str += n;
    }

    const char* prefix = config->prefix;
    const char* suffix = config->suffix;

    snprintf(str+len_str, LEN_STR-len_str, "%s", prefix);
    n = strlen(str+len_str);
    len_str += n;

    snprintf(str+len_str, LEN_STR-len_str, "%s", s);
    n = strlen(str+len_str);
    len_str += n;

    /* consider the console color system. move the \n after suffix.*/
    if(len_str>0 && str[len_str-1] == '\n') {
        len_str -= 1;
        snprintf(str+len_str, LEN_STR-len_str, "%s\n", suffix);
        n = strlen(str+len_str);
        len_str += n;
    }
    else {
        snprintf(str+len_str, LEN_STR-len_str, "%s", suffix);
        n = strlen(str+len_str);
        len_str += n;
    }
    #undef LEN_STR

    retn = fprintf(config->fp, "%s", str);

    return retn;
}


#if 0
/*
    set config for different level. 
 */
int uslog_config_set(int level, struct uslog_config* config)
{
    int ret = 0;

    uslog_init();
    if(0<=level && level<MAX_SLOG && NULL!=config) {
        struct uslog_config* get_config = &kslog_config[level];    
        get_config->fp      = config->fp;
        get_config->item    = config->item;
        get_config->prefix  = NULL!=config->prefix?config->prefix:get_config->prefix;
        get_config->suffix  = NULL!=config->suffix?config->suffix:get_config->suffix;
    }
    else {
        ret = -1;
    }

    return ret;
}
#endif


