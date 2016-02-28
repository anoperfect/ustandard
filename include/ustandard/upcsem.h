#ifndef __UPCSEM_H__
#define __UPCSEM_H__



#include <stdio.h>
__BEGIN_DECLS


int upcsem_create(key_t key, int val);

int upcsem_destroy(int semid);

int upcsem_add(int semid, int val);

int upcsem_sub(int semid, int val);


__END_DECLS

#endif//__UPCSEM_H__

