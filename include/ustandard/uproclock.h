#ifndef __UPROCLOCK_H__
#define __UPROCLOCK_H__



#include <stdio.h>
__BEGIN_DECLS


int uproclock_ipcsem_create(key_t key);

int uproclock_ipcsem_lock(int semid);

int uproclock_ipcsem_unlock(int semid);

int uproclock_ipcsem_destroy(int semid);


__END_DECLS

#endif//__UPROCLOCK_H__

