#ifndef __UTIMEVAL_H__
#define __UTIMEVAL_H__




#include "ustandard/ustandard_sys.h"
#include "ustandard/utypes.h"
__BEGIN_DECLS

#define SECOND_TO_MICRO     (1000000)
#define SECOND_TO_NANO      (1000000000)
#define MICRO_TO_NANO       (1000)



int64_t utv_count(const struct timeval* tstart, const struct timeval* tend);
int64_t utv_expired(const struct timeval* tstart);
int utv_add(struct timeval* tv, const struct timeval* add);
int utv_sub(struct timeval* tv, const struct timeval* sub);
int utv_add_usec(struct timeval* tv, int64_t usec);
int utv_sub_usec(struct timeval* tv, int64_t usec);

extern int ktv_rsec;
extern int ktv_rusec;
void utv_record_start(void);
void utv_record_stop(void);


/* count for process running.------------------------------------------------------------*/
struct utm
{
    int utm_hour;
    int utm_min;
    int utm_sec;
    int utm_usec;
};
void utv_passage_start(void);
void utv_passage_r(struct utm* tm);
struct utm* utv_passage(void);
/*---------------------------------------------------------------------------------------*/

extern unsigned long kutv_get_monotonic_count;
int utv_get_monotonic(struct timeval* tv);


long utvcmp(const struct timeval* t1, const struct timeval* t2);


/* snprintf struct timeval to string.  */
int utv_tostr_r(char*s, size_t size, const struct timeval* tv);
const char* utv_tostr(const struct timeval* tv);



#define __utv_count(name)   \
    struct timeval utv_count_qwerty_##name##_s, utv_count_qwerty_##name##_e;    \
    gettimeofday(&utv_count_qwerty_##name##_s, NULL)
#define __utv_count_usec(name)  \
    gettimeofday(&utv_count_qwerty_##name##_e, NULL) == 0?(int64_t)(utv_count_qwerty_##name##_e.tv_sec-utv_count_qwerty_##name##_s.tv_sec)*(int64_t)SECOND_TO_MICRO + (int64_t)(utv_count_qwerty_##name##_e.tv_usec-utv_count_qwerty_##name##_s.tv_usec):0


#define __utv_interval(name, usec)  \
do{ \
    static struct timeval ktv_interval_tyuiop_##name = {0, 0};  \
    if(ktv_interval_tyuiop_##name.tv_sec == 0 && ktv_interval_tyuiop_##name.tv_usec == 0) {\
        gettimeofday(&ktv_interval_tyuiop_##name, NULL);    \
        usec = 0;   \
    }   \
    else {  \
        struct timeval tv_cur;  \
        gettimeofday(&tv_cur, NULL);    \
        usec = (int64_t)(tv_cur.tv_sec-ktv_interval_tyuiop_##name.tv_sec)*(int64_t)SECOND_TO_MICRO + (int64_t)(tv_cur.tv_usec-ktv_interval_tyuiop_##name.tv_usec);  \
        ktv_interval_tyuiop_##name = tv_cur;    \
    }   \
}while(0)


__END_DECLS
#endif//__UTIMEVAL_H__
