#include <stdio.h>
#undef USLOG_MODULE
#define USLOG_MODULE    1

int color_test(void)
{
    int ret = 0;

    

#define COLOUR_DATA(x)      " \e[34;1m"x"\e[0m"
#define COLOUR_FATAL(x)     " \e[31;1m"x"\e[0m"
    printf("123\n"); printf("\e[31;1m""abc""\e[0m"); printf("456\n");
    printf("123\n"); printf("\e[34;1m""abc""\e[0m"); printf("456\n");

    printf("[->vd]");
    printf("\e[34;1m""%s\n""\e[0m", "__vd");


    fprintf(stderr, "[->ve]");
    fprintf(stderr, "\e[31;1m""%s\n""\e[0m", "__ve");


    printf("[->finished]\n");

    
    printf("[->vd]");
    printf(COLOUR_DATA("%s""\n"), "__vd");


    fprintf(stderr, "[->ve]");
    fprintf(stderr, COLOUR_FATAL("%s")"\n", "__ve");


    printf("[->finished]\n");

    fprintf(stderr, "abc""\e[34;1m""efg""\e[0m""123\n");
    fprintf(stdout, "abc""\e[34;1m""efg""\e[0m""123\n");

    fprintf(stdout, "abc""\e[34;1m""efg\n""\e[0m");
    fprintf(stderr, "abc""\e[31;1m""efg\n""\e[0m");

    fprintf(stdout, "abc""\e[34;1m""efg""\e[0m\n");
    fprintf(stderr, "abc""\e[31;1m""efg""\e[0m\n");


    return 0;    




    return ret;
}


int main()
{
    color_test();    

    do {
        char str[1024] ;
        size_t len_str = 0;
        size_t n = 0;
        if(1) {
            n = snprintf(str, 1024, "[%30s, %36s, %6d]",
                    __FILE__, __FUNCTION__, __LINE__);
            len_str += n;
        }
    }while(0);  

    return 0;
}
