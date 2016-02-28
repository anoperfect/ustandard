#include "ustandard/ustandard.h"




int main(int argc, char* argv[])
{
    int ret;

    unsigned char value[16];
    char str[40];
    str[32] = '\0';

    const char* path = argv[1];

    ret = umd5_path(value, path);
    assert(0 == ret);

    umd5_value_to_checksum(str, value);
    printf("%s   %s\n", str, path);

    return 0;
}
