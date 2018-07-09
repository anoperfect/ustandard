#include "ustandard/ustandard.h"
#include <vector>
#include <set>
#include <map>

struct timeval tv[100];

int main(int argc, char* argv[])
{
    int num = 1024 * 1024;
    int t;
    {
    gettimeofday(&tv[2], NULL);
    t = 0;
    std::map<int, int> map;
    while(t<num ) {
        map[t] = t;
        t ++;
    }
    gettimeofday(&tv[3], NULL);
    uloginf("%lld\n", utv_count(&tv[2], &tv[3]));
    }

    {
    gettimeofday(&tv[0], NULL);
    t = 0;
    std::set<int> set;
    while(t<num ) {
        set.insert(t);
        t++;
    }
    gettimeofday(&tv[1], NULL);
    uloginf("%lld\n", utv_count(&tv[0], &tv[1]));
    }

    {
    gettimeofday(&tv[0], NULL);
    t = 0;
    std::set<int> set;
    while(t<num ) {
        set.insert(t);
        t++;
    }
    gettimeofday(&tv[1], NULL);
    uloginf("%lld\n", utv_count(&tv[0], &tv[1]));
    }


    //assert(set.count(100));
    //assert(!set.count(num));


    {
    gettimeofday(&tv[2], NULL);
    t = 0;
    std::map<int, int> map;
    while(t<num ) {
        map[t] = t;
        t ++;
    }
    gettimeofday(&tv[3], NULL);
    uloginf("%lld\n", utv_count(&tv[2], &tv[3]));
    }


    //assert(map[100] == 100);
    return 0;
}
