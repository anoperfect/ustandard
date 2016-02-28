#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/upcsem.h"


union semun
{
    int val;
    struct semid_ds* buf;
    unsigned short* array;
}arg;


int upcsem_create(key_t key, int val)
{
    int sem_id;
    sem_id = semget(key, 1, IPC_CREAT | 0666);
    
    if(-1 != sem_id) {
        union semun sem;
        sem.val = val;
        semctl(sem_id, 0, SETVAL, sem);
    }

    return sem_id;
}


int upcsem_destroy(int semid)
{
    semctl(semid, 0, IPC_RMID, 0);
    return 0;
}


int upcsem_add(int semid, int val)
{
    struct sembuf sops = {0, +val, SEM_UNDO};
    return (semop(semid, &sops, 1));
}


int upcsem_sub(int semid, int val)
{
    struct sembuf sops = {0, -val, SEM_UNDO};
    return (semop(semid, &sops, 1));
}


