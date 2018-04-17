//#include "ustandard/ustandard.h"
#include <cstdio>
#include <cstring>
#include <map>
struct Test
{
    int x;
    int y;

    bool operator < (const Test &o) const
    {
        return x < o.x || y < o.y;
    }
};


struct countusecs
{
    const char* function;
    int line;

    bool operator < (const countusecs &o) const
    {
        int retcmp = strcmp(function, o.function);
        if(retcmp < 0 || (retcmp == 0 && line <= o.line)) {
            return false;
        }

        return true;
    }
};


int main()
{
    std::map<Test, long> t;

    Test test = {1, 2};
    t[test] = 1;


    Test test1 = {1, 2};
    printf("%ld\n", t[test1]);



    std::map<countusecs, long> t1;
    countusecs c = {"t.cpp", 1};
    countusecs c1 = {"t.cpp", 1};

    t1[c] = 1;
    printf("%ld\n", t1[c]);

    std::map<countusecs, long>::iterator it;
    it = t1.find(c1);
    if(it != t1.end()) {
        printf("%ld\n", it->second);
    }

    printf("%ld\n", t1[c]);
    printf("%ld\n", t1[c1]);
    


    return 0;
}
