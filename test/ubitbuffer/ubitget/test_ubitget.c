#include "ustandard/ustandard.h"
#define __P01                                               \
do{                                                         \
    memset(str, 0, 1024);                                   \
    snprintf(str, 1024, "==========%2d : ", __LINE__);      \
    ubit_trans01(str+strlen(str), 1024-strlen(str), p, 12); \
    printf("%s\n", str);                                    \
}while(0);


#define __PUINT64(uv)                                       \
do {                                                        \
    ubit_trans01_uint64(str, 1024, uv);                     \
    printf("uint64 %10llu : %s\n", uv, str);                \
}while(0);


int main()
{
    unsigned char a[12] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}; 
    void* p = a;
    char str[1024];
    __P01
    a[3] = 0xab;
    __P01

    uint64_t v_reado, v_read;
    uint64_t v_write;

    v_reado = ubit_gets(p, 31, 33);
    uloginf("v_reado = %llu\n", v_reado);

    struct timeval tv = {24*3600, 123456};
    v_write = ((uint64_t)(tv.tv_sec)*1000000 + (uint64_t)tv.tv_usec)*9/100;
    ubit_sets(p, 31, 33, v_write);
    __P01

    v_read = ubit_gets(p, 31, 33);
    uloginf("v_write = %llu\n", v_write);
    uloginf("v_read = %llu\n", v_read);

    uint64_t v3230, v2915, v1400;
    v3230 = ubit_gets(p, 31, 3); 
    v2915 = ubit_gets(p, 34, 15);
    v1400 = ubit_gets(p, 49, 15);
    v_read = (v3230<<30) | (v2915<<15) | v1400;
    uloginf("v_read = %llu.\n", v_read);

    ubit_sets(p, 31, 33, v_reado);
    v_read = ubit_gets(p, 31, 33);
    uloginf("v_read = %llu\n", v_read);

    
    __PUINT64(v_write)
    __PUINT64((v_write>>30)&0x7)
    __PUINT64((v_write>>15)&0x7fff)
    __PUINT64((v_write>>0)&0x7fff)
    
    ubit_sets(p, 31, 3 , (v_write>>30)&0x7);
    __P01
    ubit_sets(p, 34, 15 , (v_write>>15)&0x7fff);
    __P01
    ubit_sets(p, 49, 15 , (v_write>>0)&0x7fff);
    __P01
    v_read = ubit_gets(p, 31, 33);
    __P01
    uloginf("v_read = %llu\n", v_read);


    return 0;
}





