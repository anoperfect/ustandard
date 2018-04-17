#include "ustandard/ustandard.h"
int main(int argc, char* argv[])
{
    int idx = 0;
    int sn = 0;
    for(idx=0; idx<256; idx++) {
        char ch = idx;
        if(isprint(ch)) {
            uloginf("[%2d]%3d %02x %c\n", sn, ch, ch, ch);
            sn ++;
        }
    }


    long long int num = 0;
    long long int prev = 0;
    for(num=36; num<96; num++) {
        long long int count = num * num * num * num * num;
        long long int add ;
        if(prev == 0) {
            add = 0;
        }
        else {
            add = (count - prev ) * 1000 / prev;
        }

        prev = count;

        uloginf("%lld %20lld %lld\n", num, count, add);
    }



    return 0;

}
