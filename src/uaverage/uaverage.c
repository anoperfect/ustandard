#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/uaverage.h"
struct uaverage_double {
    int nmax;
    int percent_cut;
    int num;
    double array[0];
};


struct uaverage_double* uaverage_double_init(int nmax, int percent_cut)
{
    struct uaverage_double* avr = NULL;

    avr = um_malloc(sizeof(*avr) + sizeof(double)*nmax);
    if(NULL != avr) {
        avr->nmax = nmax; 
        avr->percent_cut = percent_cut;
        avr->num = 0;
    }

    return avr;
}


int uaverage_double_deinit(struct uaverage_double* avr)
{
    int ret = 0;
    if(NULL != avr) {
        um_free(avr);
    }
    return ret;
}


double uaverage_double_add(struct uaverage_double* avr, double d)
{
    double ret = 0;
    if(avr->num == avr->nmax) {
        return ret;
    }

    /* insert the value by insert-sort. binary-insert-sort is better. */
    int i;
    int j;
    for(i=0; i<avr->num && d>avr->array[i]; i++);
    for(j=avr->num-1; j>=i; j--) avr->array[j+1] = avr->array[j];
    avr->array[i] = d;
    avr->num ++;

    int ncut = avr->num * avr->percent_cut / 100;
    double sum = 0;
    for(i=ncut; i<avr->num-ncut; i++) {
        sum += avr->array[i];
    }

    ret = sum / (avr->num-2*ncut);
    return ret;
}


int uaverage_double_clear(struct uaverage_double* avr)
{
    int ret = 0;

    avr->num  = 0;

    return ret;
} 
