#include "ustandard/utv.h"
#include "ustandard/udevelop.h"
static struct timeval ktv_record = {0, 0};


int64_t utv_count(const struct timeval* tstart, const struct timeval* tend)
{
    int64_t nusec;
    nusec = (int64_t)(tend->tv_sec-tstart->tv_sec)*(int64_t)SECOND_TO_MICRO + 
            (int64_t)(tend->tv_usec-tstart->tv_usec);

    return nusec;
}


int64_t utv_expired(const struct timeval* tstart)
{
    int64_t nusec;
    struct timeval tv;
    gettimeofday(&tv, NULL);

    nusec = utv_count(tstart, &tv);
    return nusec;
}


int utv_add(struct timeval* tv, const struct timeval* add)
{
    int ret;
    ret = 0;

    tv->tv_sec += add->tv_sec;
    tv->tv_usec += add->tv_usec;

    if(tv->tv_usec >= SECOND_TO_MICRO) {
        tv->tv_sec += 1;
        tv->tv_usec -= SECOND_TO_MICRO;
    }

    return ret;
}


int utv_sub(struct timeval* tv, const struct timeval* sub)
{
    int ret;
    ret = 0;

    if(tv->tv_usec < sub->tv_usec) {
        tv->tv_sec -= 1;
        tv->tv_usec += SECOND_TO_MICRO;
    }

    tv->tv_sec -= sub->tv_sec;
    tv->tv_usec -= sub->tv_usec;

    return ret;
}


int utv_add_usec(struct timeval* tv, int64_t usec)
{
    int ret = 0;

    int64_t n = (int64_t)tv->tv_sec*(int64_t)SECOND_TO_MICRO + (int64_t)tv->tv_usec;
    n += usec;
    if(n >= 0) {
        tv->tv_sec  = n / (int64_t) SECOND_TO_MICRO;
        tv->tv_usec = n % (int64_t) SECOND_TO_MICRO;
    }
    else {
        ret = -1;
    }

    return ret;
}


int utv_sub_usec(struct timeval* tv, int64_t usec)
{
    int ret = 0;

    int64_t n = (int64_t)tv->tv_sec*(int64_t)SECOND_TO_MICRO + (int64_t)tv->tv_usec;
    n -= usec;
    if(n >= 0) {
        tv->tv_sec  = n / (int64_t) SECOND_TO_MICRO;
        tv->tv_usec = n % (int64_t) SECOND_TO_MICRO;
    }
    else {
        ret = -1;
    }

    return ret;
}




/*-----------------------------------------------------------------------------------*/
int ktv_rsec     = 0;
int ktv_rusec    = 0;


void utv_record_start(void)
{
    gettimeofday(&ktv_record, NULL);
}


void utv_record_stop(void)
{
    struct timeval tv_cur;
    gettimeofday(&tv_cur, NULL);

    ktv_rsec  = tv_cur.tv_sec  - ktv_record.tv_sec ;
    ktv_rusec = tv_cur.tv_usec - ktv_record.tv_usec;
    if(ktv_rusec < 0) {
        ktv_rusec += 1000*1000;
        ktv_rsec  -= 1;
    }
}


/*-----------------------------------------------------------------------------------*/





/* count for process running.------------------------------------------------------------*/
static struct timeval ktv_passage_start = {0};
static struct utm kutm = {0};

void utv_passage_start(void)
{
    gettimeofday(&ktv_passage_start, NULL);
}


void utv_passage_r(struct utm* tm)
{
    if(0 == ktv_passage_start.tv_sec && 0 == ktv_passage_start.tv_usec) {
        gettimeofday(&ktv_passage_start, NULL);
        tm->utm_hour    = 0;    
        tm->utm_min     = 0;    
        tm->utm_sec     = 0;    
        tm->utm_usec    = 0;    
    }
    else {
        int sec;
        int usec;
        struct timeval cur;
        gettimeofday(&cur, NULL);
        sec = cur.tv_sec - ktv_passage_start.tv_sec;
        usec = cur.tv_usec - ktv_passage_start.tv_usec;
        if(usec < 0) {
            sec -= 1;
            usec += (1000*1000);
        }

        tm->utm_hour = sec / 3600 ;
        sec = sec % 3600 ;
        tm->utm_min = sec / 60 ;
        sec = sec % 60;
        tm->utm_sec = sec;
        tm->utm_usec = usec;
    }
}


struct utm* utv_passage(void)
{
    utv_passage_r(&kutm);
    return &kutm;
}
/*---------------------------------------------------------------------------------------*/


unsigned long kutv_get_monotonic_count = 0;
int utv_get_monotonic(struct timeval* tv)
{
    int ret = 0;

    struct timespec ts;
    ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    tv->tv_sec  = ts.tv_sec;
    tv->tv_usec = ts.tv_nsec/(long)MICRO_TO_NANO;
    kutv_get_monotonic_count ++;

    return ret;
}


long utvcmp(const struct timeval* t1, const struct timeval* t2)
{
    int ret = 0;
    if(t1->tv_sec > t2->tv_sec) {
        ret = t1->tv_sec > t2->tv_sec;
    }
    else if(t1->tv_sec == t2->tv_sec) {
        if(t1->tv_usec == t2->tv_usec) {
            ret = 0;
        }
        else {
            ret = (t1->tv_usec>t2->tv_usec)?1:-1;
        }
    }
    else {
        ret = 0 - (long)(t2->tv_sec - t1->tv_sec);
    }

    return ret;
}


/* snprintf struct timeval to string.  */
int utv_tostr_r(char*s, size_t size, const struct timeval* tv)
{
    int ret = 0;

    if(NULL != tv) {
        snprintf(s, size, "%lu:%06lu", tv->tv_sec, tv->tv_usec);
    }
    else {
        snprintf(s, size, "nil:000000");
    }
    
    return ret;
}


const char* utv_tostr(const struct timeval* tv)
{
    char* rets = "";

    #define NUM    6 
    #define LEN_STR_TV_PRINTF     64
    static char kstr_tv_printf[NUM][LEN_STR_TV_PRINTF] = {{0}};
    static int kidx = 0;

    rets = kstr_tv_printf[kidx];
    kidx = (kidx+1)%NUM;
    utv_tostr_r(rets, LEN_STR_TV_PRINTF, tv);
    #undef NUM
    
    return rets;
}
