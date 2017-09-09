#include "ustandard/ustandard.h"




int main()
{
    int ret;
    ukeyword_t* kw = ukeyword_init();
    assert(NULL != kw);

    ret = ukeyword_register(kw, "####", 4, 1);
    assert(0 == ret);

    ret = ukeyword_register(kw, "###", 3, 2);
    assert(0 == ret);

    const char* str = "111111###22222####jfkfsk";
    size_t size = strlen(str);
    int i;
    for(i=0; i<size; i++) {
        ret = ukeyword_input(kw, (unsigned char)str[i]);
        if(ret >= 0) {
            printf("matched %d(index=%d). \n", ret, i);
        }
    }

    return 0;
}
