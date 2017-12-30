#include "ustandard/ustandard.h"
int main(int argc, const char* argv[])
{
    char s[100] = "1234567890";

    ubuffer_move_left(s, 10, 
            s + 3, 1, 
            2);
    printf("s=%s\n", s);

    strcpy(s, "1234567890");
    ubuffer_move_left(s, 10, 
            s + 1, 9, 
            1);
    printf("s=%s\n", s);

    strcpy(s, "1234567890");
    ubuffer_move_left(s, 10, 
            s + 1, 9, 
            2);
    printf("s=%s\n", s);

    strcpy(s, "1234567890");
    ubuffer_move_left(s, 11, 
            s + 6, 5, 
            6);
    printf("s=%s\n", s);




    strcpy(s, "1234567890");
    ubuffer_move_right(s, 10, 
            s, 9, 
            1);
    printf("s=%s\n", s);

    strcpy(s, "1234567890");
    ubuffer_move_right(s, 10, 
            s+1, 2, 
            3);
    printf("s=%s\n", s);

    




    

    return 0;
}
