#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <stdarg.h>
#include <sys/time.h>
#include <unistd.h>

#include "ustandard/uslog.h"
#define USLOG_LEVEL_NUMBER      100
#define USLOG_OUTSTREAM_NUMBER  6



#define LOG printf("%ld %s %d\n", pthread_self(), __FUNCTION__, __LINE__);

/* config for different level. */
struct uslog_config {
    int enabled;
    FILE* outstream[USLOG_OUTSTREAM_NUMBER];
    int noutstream;
    unsigned int item;
    const char* prefix;
    const char* suffix;
};


/* uslog context. */
struct uslog_data {
    int inited;
    struct uslog_config configs[USLOG_LEVEL_NUMBER];
};

/*
    use thread private to record the thread idx. 
    the index sequence depending on calling uslog_thread_index.
 */
//#define NSLOG_MAX_THREAD 1024
//static pthread_t klog_pths[NSLOG_MAX_THREAD];
static int knlog_pths;
static pthread_mutex_t klog_pthread_idx_lock = PTHREAD_MUTEX_INITIALIZER;

/* tp mean thread private. */
__thread int tp_thread_idx = -1;
__thread int tp_function_level = 0;
__thread char* tp_function[1024];


int uslog_thread_index(void)
{
    if(-1 != tp_thread_idx) {
        return tp_thread_idx;
    }

    pthread_mutex_lock(&klog_pthread_idx_lock);   
    tp_thread_idx = knlog_pths;
    knlog_pths ++;
    pthread_mutex_unlock(&klog_pthread_idx_lock);   

    return tp_thread_idx;
}


int _uslog_config_set_outstream(struct uslog_config* config, const char* s)
{
    int ret = 0;
    config->noutstream = 0;

    const char* start = s;
    const char* tmp;
    size_t len;
    char name[1024];
    int finished = 0;
    while(!finished) {
        tmp = strchr(start, ',');
        if(!tmp) {
            finished = 1;
            tmp = start + strlen(start);
        }

        len = tmp - start;
        if(len > 0 && len < sizeof(name)) {
            strncpy(name, start, len);
            name[len] = '\0';

            if(name[len-1] == '\n') {
                name[len-1] = '\0';
            }

            FILE* out = NULL;
            if(0 == strcmp(name, "stdout")) {
                out = stdout;
            }
            else if(0 == strcmp(name, "stderr")) {
                out = stderr;
            }
            else if(0 == strcmp(name, "/dev/null")) {
                out = NULL;
            }
            else {
                out = fopen(name, "w");
            }

            if(out) {
                setbuf(out, NULL);
                if(config->noutstream < USLOG_OUTSTREAM_NUMBER) {
                    config->outstream[config->noutstream] = out;
                    config->noutstream ++;
                }
            }
        }
        
        start = tmp + 1;
    }

    return ret;
}


int _uslog_config_read(struct uslog_data* logctx, const char* name)
{
    int ret = 0;

    FILE* fp = fopen(name, "r");
    if(!fp) {
        printf("no nslog config file.\n");
        return ret;
    }

    int level = -1;
    char line[1024];
    while(1) {
        char* tmp = fgets(line, sizeof(line), fp);
        if(!tmp) {
            break;
        }
    
        size_t len = strlen(line);
        if(len >=4 && line[0] == '[' && line[len-1] == '\n' && line[len-2] == ']') {
            level = atol(line+1);
            if(level >= 0 && level < USLOG_LEVEL_NUMBER) {

            }
            else {
                fprintf(stderr, "level error %d\n", level);
                break;
            }
        }

        const char* key = NULL;
        size_t size_key = 0;

        key = "item=";
        size_key = strlen(key);
        if(0 == strncmp(line, key, size_key)) {
            logctx->configs[level].item = atol(line+size_key);
        }
        
        key = "outstream=";
        size_key = strlen(key);
        if(0 == strncmp(line, key, size_key)) {
            _uslog_config_set_outstream(&logctx->configs[level], line+size_key);
        }

        key = "enabled=";
        size_key = strlen(key);
        if(0 == strncmp(line, key, size_key)) {
            if(0 == atol(line+size_key)) {
                logctx->configs[level].enabled = atol(line+size_key);
            }
            else {

            }
        }
    }

    fclose(fp);

    return ret;
}


int _uslog_config_init(struct uslog_data* logctx)
{
    int ret = 0;
    memset(logctx, 0, sizeof(*logctx));

    int level;
    /* set the default value for all level. */
    int idx;
    for(idx=0; idx<USLOG_LEVEL_NUMBER; idx++) {
        level = idx;
        logctx->configs[level].enabled = 1;
        logctx->configs[level].noutstream = 1;
        logctx->configs[level].outstream[0] = stdout;
        logctx->configs[level].item = USLOG_ITEM_ENABLE_FILEINFO | USLOG_ITEM_ENABLE_PTHREAD_ID | USLOG_ITEM_ENABLE_TIMEVAL;
        logctx->configs[level].prefix = "";
        logctx->configs[level].suffix = "";
    }

    /* config v0, test, info, debug, error. */
    {
        logctx->configs[__v0].outstream[0] = NULL;
        logctx->configs[__v0].prefix = " v0   : ";

        logctx->configs[__vt].outstream[0] = stdout;
        logctx->configs[__vt].prefix = " test : ";

        logctx->configs[__vi].outstream[0] = stdout;
        logctx->configs[__vi].prefix = " info : ";

        logctx->configs[__vd].outstream[0] = NULL;
        logctx->configs[__vd].prefix = " debug: ";

        logctx->configs[__ve].outstream[0] = stderr;
        logctx->configs[__ve].prefix = " error: ";
    }

    _uslog_config_read(logctx, "/etc/uslog.conf");

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

    if(0<=level && level<USLOG_LEVEL_NUMBER) {
        struct uslog_config* get_config = &(*logctx)->configs[level];    
        va_list arg;
        va_start(arg, tag);

        switch(tag) {
            case USLOG_CONFIG_OUTSTREAM:
            //get_config->fp          = va_arg(arg, FILE*); 
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
    if(level >= 0 && level < USLOG_LEVEL_NUMBER) {
        config = &(*logctx)->configs[level];
        if(config->enabled && config->noutstream > 0 && config->outstream[0] != NULL) {
            retb = 1;
        }
    }

    return retb;
}


#if 0
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
#endif


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
    if(level >= 0 && level < USLOG_LEVEL_NUMBER) {
        config = &(*logctx)->configs[level];
        if(NULL != config->outstream[0] && config->noutstream > 0) {
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
                "[%60s, %45s, %6d]", file, function, line);
        n = strlen(str+len_str);
        len_str += n;
    }

    /* add pthread_t index, not pthread_t directly. */
    if(config->item & USLOG_ITEM_ENABLE_PTHREAD_ID) {
        //snprintf(str+len_str, LEN_STR-len_str, "[%3d]", uget_pthread_id());
        snprintf(str+len_str, LEN_STR-len_str, "[%3d]", uslog_thread_index());
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

    int idx;
    for(idx=0; idx<config->noutstream; idx++) {
        if(config->outstream[idx] != NULL) {
            retn = fprintf(config->outstream[idx], "%s", str);
        }
    }

    return retn;
}


int uslog_level_config(int level, unsigned int item, FILE* outstream[], int noutstream)
{
    int ret = 0;

    


    return ret;
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



const char* uslog_level_blank(int level)
{
    static char* kblank[] = {
        "", 
        "  ", 
        "    ", 
        "      ", 
        "        ", 
        "          ", 
        "            ", 
        "              ", 
        "                ", 
        "                  ", 
        "                    ",
        "                      ",
        "                        ",
        "                          ",
        "                            ",
        "                              ",
        "                                ",
        "                                  ",
        "                                    ",
        "                                      ",
        "                                        ",
        "                                          ",
        "                                            ",
        "                                              ",
        "                                                ",
        "                                                  ",
        "                                                    ",
        "                                                      ",
    };

    if(level < sizeof(kblank) / sizeof(kblank[0])) {
        return kblank[level];
    }

    return "xx- ";
}


int uslog_function_enter(const char* file, const char* function, int line)
{
    int ret = 0;

    if(tp_function[tp_function_level] == NULL) {
        tp_function[tp_function_level] = malloc(128);
    }

    size_t size_src = strlen(function);
    size_t size_copy = size_src<128?size_src:128-1;
    strncpy(tp_function[tp_function_level], function, size_copy);
    tp_function[tp_function_level][size_copy] = '\0';

    tp_function_level ++;

    return ret;
}


int uslog_function_return(const char* file, const char* function, int line)
{
    int ret = 0;

    assert(0==strcmp(function, tp_function[tp_function_level-1])); 
    tp_function_level--; 

    return ret;
}
