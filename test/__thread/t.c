#include <stdio.h>
#include <unistd.h>
#include <string.h>



//__thread int a[1024*1024*100];
 int a[1024*1024*100];

int main(int argc, char* argv[])
{
    memset(a, 0, sizeof(a));
    a[1024*1024*100 - 1] = 0;
    while(1) {
        sleep(1);
    }

    return 0;
}
