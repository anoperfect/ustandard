#include "ustandard/ustandard.h"




int int_printf(char* str, size_t size, void* p)
{
    int n ;
    memcpy(&n, p, sizeof(int));
    snprintf(str, size, "%d", n);
    return 0;
}


int main()
{
    char str[10240];
    struct uarray* array = uarray_create_detail(sizeof(int), -1, int_printf);

    int idx;
    for(idx=0; idx<6; idx++) {
        uarray_add_at(array, -1, &idx);

        uarray_description(array, "", str, 10240);
        printf("%s\n\n\n", str);
    }

    uarray_add_at(array, 0, &idx);
    uarray_description(array, "", str, 10240);
    printf("%s\n\n\n", str);

    idx++;
    uarray_add_at(array, 7, &idx);
    uarray_description(array, "", str, 10240);
    printf("%s\n\n\n", str);

    idx++;
    uarray_add_at(array, 8, &idx);
    uarray_description(array, "", str, 10240);
    printf("%s\n\n\n", str);

    idx++;
    uarray_add_at(array, 8, &idx);
    uarray_description(array, "", str, 10240);
    printf("%s\n\n\n", str);

    idx++;
    uarray_add_at(array, 100, &idx);
    uarray_description(array, "", str, 10240);
    printf("%s\n\n\n", str);



    uarray_destroy(array);
    array = NULL;



    return 0;
}
