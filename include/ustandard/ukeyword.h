#ifndef __UKEYWORD_H__
#define __UKEYWORD_H__
#include <stdio.h>
__BEGIN_DECLS

struct ukeyword;
typedef struct ukeyword ukeyword_t;


ukeyword_t* ukeyword_init(void);


/*
    register item should not be more then 10.

    param
        size should not be less then 1024.
        sn is the register index, should be >=0 and unique.

    return
        0 OK, -1 Error.
*/
int ukeyword_register(ukeyword_t* kw, const void* item, size_t size, int sn);


/*
    return
        >=0 matched index ; -1 not matched.
*/
int ukeyword_input(ukeyword_t* kw, unsigned char ch);


void ukeyword_deinit(ukeyword_t* kw);

__END_DECLS
#endif /* ukeyword.h */
