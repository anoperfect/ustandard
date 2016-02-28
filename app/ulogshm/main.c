#include "ustandard/ustandard.h"
#define __1M    (1024*1024)
#define LEN_FORMAT      (128)
struct ulogshm_data {
    FILE*           fp;
    unsigned long   n;
    key_t           key;
    size_t          size;
    char            format[LEN_FORMAT];
};
static struct ulogshm_data klogshm_data;
static int _ulogshm_getopt(int argc, char* const argv[]);



int main(int argc, char* const argv[])
{
    int ret = 0;
//uslog_sys_set(__vd, stdout);
    ret = _ulogshm_getopt(argc, argv);
    if(0 != ret) {
        exit(EXIT_FAILURE);
    }
    struct ucycm_read* cmr = NULL;
    cmr = ucycmr_create(NULL, klogshm_data.key, klogshm_data.size, UCYCMR_TYPE_LATEST, NULL, NULL);
    if(NULL == cmr) {
        fprintf(stderr, "ucycmr_create error.\n");
        exit(EXIT_FAILURE);
    }

    unsigned long sn;
    struct ucycm_read_segment_info info;
    int t_error = 0;
    while(1) {
        ret = ucycmr_get(cmr, NULL, &sn, &info, NULL);
        if(0 != ret) {
            t_error ++;
            fprintf(stderr, "ucycmr_get error %d.\n", t_error);
            sleep(1);
            if(t_error > 10) {
                break;
            }
        }

        ulogdbg("[%lu - %lu].\n", info.sn_min, info.sn_max);
        fwrite(info.addr, 1, info.size, klogshm_data.fp);
    }


    ucycmr_destroy(cmr);
    cmr = NULL;

    exit(EXIT_SUCCESS);
}


int _ulogshm_getopt(int argc, char* const argv[])
{
    int ret = 0;

    const char* strdisplay = "ulogshm usage : ""\n"
"--help display usage""\n" 
"-o     stdout | stderr | syslog | linux path . default to stdout.""\n"
"-n     number . display last n lines .""\n"
"-k     key. ipc shared meory key.""\n"
"-p     fotk path. use ftok instead of ipc key.""\n"
"-s     size(nk,nm). shared meory size.""\n"
"-f     format. display format.""\n";

    if(1 ==argc || (2 == argc && 0 == strcmp(argv[1], "--help"))) {
        fprintf(stdout, "%s", strdisplay);
        exit(EXIT_SUCCESS);
    }

    klogshm_data.fp = stdout;
    klogshm_data.n = 1024;
    klogshm_data.key = ftok("/bin/streammgrd", 'x');
    klogshm_data.size = __1M;
    ustrcpy(klogshm_data.format, LEN_FORMAT, "%P");

    FILE* fp = NULL;
    int opt;
    const char* optstring = "o:n:k:p:s:f:h:";
    while(-1 != (opt = getopt(argc, argv, optstring))) {
        switch(opt) {
            case 'o':
            /*  ## not implenet.*/
            fp = NULL;
            break;

            case 'n':
            klogshm_data.n = atol(optarg);
            break;

            case 'k':
            klogshm_data.key = atol(optarg);
            break;

            case 'p':
            klogshm_data.key = ftok(optarg, 'x');
            break;

            case 's':
            klogshm_data.size = atol(optarg);
            if('k' == ustrtail(optarg) || 'K' == ustrtail(optarg)) {
                klogshm_data.size *= 1024;
            }
            if('m' == ustrtail(optarg) || 'M' == ustrtail(optarg)) {
                klogshm_data.size *= (1024*1024);
            }
            break;

            case 'f':
            ustrcpy(klogshm_data.format, LEN_FORMAT, optarg);
            break;

            default:
            break;
        }
    }

    return ret;
}
















































#if 0
    unsigned long i;
    struct ucycm_read_segment_info info_tmp;
    for(i=info.sn_min; i<=info.sn_max; i++) {
        ret = ucycmr_info(cmr, i, &info_tmp, NULL);
        if(0 != ret) {
            fprintf(stderr, "ucycmr_create error.\n");
            ucycmr_destroy(cmr);
            cmr = NULL;
            exit(EXIT_FAILURE);
        }
        
        fwrite(info_tmp.addr, 1, info_tmp.size, stdout);

    }
#endif

