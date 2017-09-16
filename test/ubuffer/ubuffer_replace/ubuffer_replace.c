#include "ustandard/ustandard.h"
int main(int argc, const char* argv[])
{
    const char* s = "121212";

    struct urange ranges[3];

    ranges[0].location = 0;
    ranges[0].length = 1;

    ranges[1].location = 2;
    ranges[1].length = 1;

    ranges[2].location = 4;
    ranges[2].length = 1;

    struct ucbuf buf[2];

    buf[0].p = "2";
    buf[0].size = 1;

    buf[1].p = "32";
    buf[1].size = 2;


    void* t = NULL;
    size_t sizet = 0;

    int ret = ubuffer_replace(s, strlen(s) + 1, 
            &t, &sizet, 
            ranges, 3, 
            buf, 2);

    assert(0 == ret);
    printf("%s\n", (char*)t);








    return 0;
}
