#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/uproclock.h"


union semun
{
    int val;
    struct semid_ds* buf;
    unsigned short* array;
}arg;


int uproclock_ipcsem_create(key_t key)
{
    int sem_id;
    sem_id = semget(key, 1, IPC_CREAT | 0666);
    
    if(-1 != sem_id) {
        union semun sem;
        sem.val = 1;
        semctl(sem_id, 0, SETVAL, sem);
    }

    return sem_id;
}


int uproclock_ipcsem_lock(int semid)
{
    struct sembuf sops = {0, -1, SEM_UNDO};
    return (semop(semid, &sops, 1));
}


int uproclock_ipcsem_unlock(int semid)
{
    struct sembuf sops = {0, +1, SEM_UNDO};
    return (semop(semid, &sops, 1));
}


int uproclock_ipcsem_destroy(int semid)
{
    semctl(semid, 0, IPC_RMID, 0);
    return 0;
}
