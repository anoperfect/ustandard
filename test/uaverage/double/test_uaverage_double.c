#include "ustandard/ustandard.h"
int main()
{
    struct uaverage_double* avr = uaverage_double_init(10240, 10);
    assert(NULL != avr);

    #define NUM 12
    int a[NUM] = {1, 2, 7, 10, 16, 100, 60, 45, 45, 60, 8, 19};

    int i;
    for(i=0; i<NUM; i++) {
        printf("%lf\n", uaverage_double_add(avr, a[i]));
    }

    uaverage_double_deinit(avr);
    avr = NULL;

    return 0;
}
