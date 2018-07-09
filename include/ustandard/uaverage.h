#ifndef __UAVERAGE_H__
#define __UAVERAGE_H__
__BEGIN_DECLS
struct uaverage_double;
struct uaverage_double* uaverage_double_init(int nmax, int percent_cut);
int uaverage_double_deinit(struct uaverage_double* avr);

double uaverage_double_add(struct uaverage_double* avr, double d);

int uaverage_double_clear(struct uaverage_double* avr);
__END_DECLS
#endif /* uaverage.h */
