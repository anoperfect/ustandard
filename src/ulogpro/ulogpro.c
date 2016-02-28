#include "ustandard/ustandard_sys.h"
#include <string.h>
#include "__common.h"
#include "ulog_pattern.c"
#include "ulogconf.c"
#include "ulogpro.h"



FILE* _ulog_get_fp(const char* pathname);
int _ulog_read_config(const char* pathname);
int _ulog_add_level(const char* level, char* output);




#define LEN_LEVEL           20
#define MAX_LEVEL           20
#define LEN_PATTERN         1024
#define LEN_OUTPUT          1024
#define MAX_OUTPUT          20
#define LEN_OUTPUT_PATH     128

struct ulog_level {
    char level[LEN_LEVEL];

    FILE* fps[MAX_OUTPUT];
    int num_fps;

    struct ulog_pattern pattern;
};


const char* kpname;
struct ulog_level klevel[MAX_LEVEL];
int knum_level = 0;

int _ulog_read_config(const char* pathname)
{
    int ret = 0;
    int num_item;
    int i;
    char level_string[LEN_LEVEL];
    char output_string[LEN_OUTPUT];

    CONF* conf = ulogconf_open(pathname);
    if(NULL == conf) {
        fprintf(stderr, "ulog config file parse error.\n");
        ret = -1;
        return ret;
    }

    knum_level = 0;

    /* parse global. */
    num_item = ulogconf_get_item_num(conf, "global");
    for(i=0; i<num_item; i++) {
        ulogconf_get_item_by_index(conf, "global", i, 
                level_string, LEN_LEVEL, 
                output_string, LEN_OUTPUT);
        ret = _ulog_add_level(level_string, output_string);
    }

    /* parse this process. */
    num_item = ulogconf_get_item_num(conf, kpname);
    for(i=0; i<num_item; i++) {
        ulogconf_get_item_by_index(conf, kpname, i, 
                level_string, LEN_LEVEL, 
                output_string, LEN_OUTPUT);
        ret = _ulog_add_level(level_string, output_string);
    }

    __debug_printf("ulogconf_close.\n");
    ulogconf_close(conf);

    return ret;
}


int _ulog_add_level(const char* level, char* output)
{
    __debug_printf("add with <%s = %s>\n", level, output);

    int ret = 0;

    if(knum_level >= MAX_LEVEL) {
        fprintf(stderr, "ulog config level full.\n");
        ret = -1;
        return ret;
    }

    char chs_split[] = {',', ' ', '\t'};
    int n_chs_split = sizeof(chs_split) / sizeof(char);

    struct ulog_level* level_st = &klevel[knum_level];
    level_st->num_fps = 0;

    /* get and cut the pattern string from output. */
    char str_pattern[LEN_PATTERN];
    char* s;
    char* e;
    int length;
    if(NULL != (s=strchr(output, '[')) && NULL != (e=strchr(s+1, ']'))) {
        length = e-s+1;
        __strncpy(str_pattern, LEN_PATTERN, s+1, length);
        level_st->pattern.num_patterns = 0;
        ulog_parse_pattern(&level_st->pattern, str_pattern);
        *s = '\0';
    }
    else {
        ulog_default_pattern(&level_st->pattern);
    }

    /* level string. */
    __strcpy(level_st->level, LEN_LEVEL, level);

    /* destination FILE*. */
    const char* tmp = output;
    while(1) {
        _ustrpass(&tmp, chs_split, n_chs_split);
        __debug_printf("tmp = %s\n", tmp);
        length = _ustrpart(tmp, chs_split, n_chs_split);
        __debug_printf("length = %d\n", length);
        if(length > 0) {
            if(level_st->num_fps < MAX_OUTPUT) {
                char output_path[LEN_OUTPUT_PATH];
                __strncpy(output_path, LEN_OUTPUT_PATH, tmp, length);
                tmp += length;
                __debug_printf("output_path = %s\n", output_path);
                level_st->fps[level_st->num_fps] = _ulog_get_fp(output_path);
                if(NULL != level_st->fps[level_st->num_fps]) {
                    level_st->num_fps ++;
                }

            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }
    
    if(level_st->num_fps > 0) {
        knum_level ++ ;
    }

    __debug_printf("end add.\n");

    return ret;
}


int knum_fps_global = 0;
struct ulog_output_fp
{
    char output_path[LEN_OUTPUT_PATH];
    FILE* fp;
};
struct ulog_output_fp kfps_global[MAX_OUTPUT] = {{{0}}};
int kfps_global_inited = 0;
void ulog_get_fp_init(void);

FILE* _ulog_get_fp(const char* pathname)
{
    FILE* fp = NULL;
    size_t size;
    if(NULL == pathname || 0 == (size=strlen(pathname))) {
        __debug_printf("%s : invalid pathname.\n", __FUNCTION__);
        return fp;
    }

    __debug_printf("get fp : %s \n", pathname);

    if(!kfps_global_inited)
    {
        kfps_global_inited = 1;

        __strcpy(kfps_global[knum_fps_global].output_path, LEN_OUTPUT_PATH, "stdout");
        kfps_global[knum_fps_global].fp = stdout;
        knum_fps_global ++ ;
        
        __strcpy(kfps_global[knum_fps_global].output_path, LEN_OUTPUT_PATH, "stderr");
        kfps_global[knum_fps_global].fp = stderr;
        knum_fps_global ++ ;
        
        __strcpy(kfps_global[knum_fps_global].output_path, LEN_OUTPUT_PATH, "/dev/null");
        kfps_global[knum_fps_global].fp = NULL;
        knum_fps_global ++ ;
    }

    int i;
    int is_found = 0;
    for(i=0; i<knum_fps_global; i++) {
        if(0 == strcmp(pathname, kfps_global[i].output_path)) {
            fp = kfps_global[i].fp;
            is_found = 1;
            break;
        }
    }

    /* store the pathname and fp information. */
    if(!is_found) {
        fp = fopen(pathname, "w");
        if(knum_fps_global < MAX_OUTPUT) {
            __strcpy(kfps_global[knum_fps_global].output_path, LEN_OUTPUT_PATH, pathname);
            kfps_global[knum_fps_global].fp = fp;
            knum_fps_global ++;
        }
    }

    return fp;    
}


struct ulog_level* _ulog_getlevel_info(const char* level)
{
    struct ulog_level* level_st = NULL;

    int i;
    for(i=0; i<knum_level; i++) {
        if(0 == strcmp(level, klevel[i].level)) {
            level_st = &klevel[i];
            break;
        }
    }

    return level_st;
}
 

const char* kconf_path = "/etc/ustandard/ulogpro.conf";
int kis_inited = 0; 
int ulog_init(int argc, char* argv[])
{
    int ret = 0;

    if(kis_inited) {
        return ret;
    }
    kis_inited = 1;

    kpname = strrchr(argv[0], '/')?strrchr(argv[0], '/')+1:argv[0];

    /* get the ulog.conf path. */
    kconf_path = "ulog.conf";

    /* read the config file and parse. */
    _ulog_read_config(kconf_path);

    __debug_printf("level num = %d.\n", knum_level);

    return ret;
}


int ulog(const char* level, 
        const char* file, const char* function, unsigned long line, 
        const char* format, ...)
{
    int nprintf = 0;
    va_list ap;

    if(!kis_inited) {
        fprintf(stdout, "[ulog not init]");

        va_start(ap, format);
        vfprintf(stdout, format, ap);
        va_end(ap);
    }

    struct ulog_level* level_st = _ulog_getlevel_info(level);
    if(NULL == level_st) {
        return nprintf;
    }

    int i;
    for(i=0; i<level_st->num_fps; i++) {
        va_start(ap, format);
        vfprintf(level_st->fps[i], format, ap);
        va_end(ap);

        fflush(level_st->fps[i]);
    }

    return nprintf;
}

