#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ustdvalue.h"
#include "ustandard/ubacktrace.h"

double ubacktrace_timeval_sec(struct timeval* tv)
{
    struct timeval cur;
    gettimeofday(&cur, NULL);

    double sec = (double)(cur.tv_sec-tv->tv_sec) 
            + (double)(cur.tv_usec-tv->tv_usec)/1000000.0 ;
    return sec;
}


static struct timeval ubacktrace_timeval(struct timeval* tv)
{
    struct timeval cur;
    struct timeval ret;
    gettimeofday(&cur, NULL);

    if(cur.tv_usec < tv->tv_usec) {
        cur.tv_usec += 1000000;
        cur.tv_sec -= 1;
    }

    ret.tv_sec = cur.tv_sec - tv->tv_sec; 
    ret.tv_usec = cur.tv_usec - tv->tv_usec; 

    return ret;
}


#define __strcpy(dest, size, src)       \
do{                                     \
    memset(dest, 0, size);              \
    strncpy(dest, src, size-1);         \
}while(0)


#define LEN_PATH        1024
static char kdest[LEN_PATH] = {0};
static int kinited = 0;
static struct timeval ktv_start = {0, 0};

#define MAX_THREADS     1024

struct ubacktarce_threads
{
    int num;
    pthread_t   pth[MAX_THREADS];   
    int         level[MAX_THREADS];
};
static struct ubacktarce_threads kthreads = {0};


static void ubacktrace_init(void)
{
    if(!kinited) {
        gettimeofday(&ktv_start, NULL);
        if(0 == strlen(kdest)) {
            snprintf(kdest, LEN_PATH, "/tmp/ee.%u", getpid());
        }
        kinited = 1;
    }
}


/* get the thread number and function level. */
void ubacktrace_get_thread_info(char* dest, size_t size, int** pplevel)
{
    pthread_t pth = pthread_self();
    int i;
    int found = 0;
    for(i=0; i<kthreads.num; i++) {
        if(kthreads.pth[i] == pth) {
            found = 1;
            break;
        }
    }

    /* not found. */
    if(1 == found) {
        snprintf(dest, size, "%s.%d", kdest, i);
        *pplevel = &kthreads.level[i];
    }
    else {
        if(kthreads.num < MAX_THREADS -1) {
            kthreads.pth[kthreads.num]      = pth;
            kthreads.level[kthreads.num]    = 0;
            
            snprintf(dest, size, "%s.%d", kdest, kthreads.num);
            *pplevel = &kthreads.level[kthreads.num];

            kthreads.num ++;

        }
        else {
            snprintf(dest, size, "%s.%d", kdest, -1);
            *pplevel = &kthreads.level[kthreads.num];
        }
    }
}


/* redefine the backtrace dest info. */
void ubacktrace_set(const char* pathname)
{
    if(NULL != pathname) {
        __strcpy(kdest, LEN_PATH, pathname);
    }
    else {
        kdest[0] = '\0';
    }
}


int ubacktrace_enter(const char* name, const char* file, int line)
{
    ubacktrace_init();
if(kstd_pid != getpid()) return 0;
    struct timeval tv = ubacktrace_timeval(&ktv_start);

    char dest[LEN_PATH] = {0};
    int* level;
    ubacktrace_get_thread_info(dest, LEN_PATH, &level);
    FILE* fp = fopen(dest, "a");
    if(NULL == fp) {
        fprintf(stderr, "open failed.\n");
        return 0;
    }

    int i;
    for(i=0; i<*level; i++) {
        fwrite("\t", 1, 1, fp);
    }
    fprintf(fp, "%s##%s##%d##%ld:%ld[->]\n", name, file, line, tv.tv_sec, tv.tv_usec);
    fclose(fp);

    (*level) ++ ;

    return 0;
}


int ubacktrace_exit(const char* name, const char* file, int line)
{
    ubacktrace_init();
if(kstd_pid != getpid()) return 0;
    struct timeval tv = ubacktrace_timeval(&ktv_start);

    char dest[LEN_PATH] = {0};
    int* level;
    ubacktrace_get_thread_info(dest, LEN_PATH, &level);
    FILE* fp = fopen(dest, "a");
    if(NULL == fp) {
        fprintf(stderr, "open failed.\n");
        return 0;
    }

    (*level) -- ;
    int i;
    for(i=0; i<*level; i++) {
        fwrite("\t", 1, 1, fp);
    }
    fprintf(fp, "%s##%s##%d##%ld:%ld[<-]\n", name, file, line, tv.tv_sec, tv.tv_usec);
    fclose(fp);

    return 0;
}
