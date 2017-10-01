#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ulinklist.h"
#include "ustandard/uarray.h"
#include "ustandard/uthreads.h"





struct u1 {
    void *(*start_routine) (void *);
    void *arg;
    long sntask;
    struct timeval tv;
};


struct uthreads;
struct uthread {
    int pipe[2];    /* wait for cmd, send the response. */
    pthread_t pth;
    int status; /* 0:idle, 1,running, 2.commit. 3.error.*/

    struct u1* task;

    struct uthreads* ths;
};


enum thread_status {
    thread_status_init = 0  ,
    thread_status_idle      , 
    thread_status_assign    , 
    thread_status_running   , 
    thread_status_commit    , 
    thread_status_error     , 
};


struct uthreads {
    int number;
    long sntask_getto;
    
    struct uthread* threads;

    struct uarray* task_status_records; /* recoord the sntask and status, use uthread_query to query the status.*/
    struct ulinklist* tasks;    /* task list. */

    pthread_t pth_assign;
    int pipe_assign[2];
};




struct uthread_task_record {
    long sn;
    uthread_task_e status;
};



void* func(void* arg)
{
    struct uthread* th = arg;
    char ch;

    th->status = thread_status_idle;

    while(1) {
        int n = read(th->pipe[0], &ch, 1);
        if(n == 1) {
            th->status = thread_status_assign;
            ulogdbg("thread task status : assign.\n");

            if(th->task && th->task->start_routine) {
                th->status = thread_status_running;
                ulogdbg("thread task status : commit.\n");
                th->task->start_routine(th->task->arg);
            }

            th->status = thread_status_commit;
            ulogdbg("thread task status : commit.\n");

            int nwrite = write(th->pipe[1], &th, 1);
            if(0 != nwrite) {

            }

            continue;
        }

        th->status = thread_status_error;
        int nwrite = write(th->pipe[1], &th, 1);
        if(0 != nwrite) {

        }
    }

    return NULL;
}








void* func_assign(void* arg)
{
    struct uthreads* ths = arg;

    fd_set set;
    int maxfd = 0;

    FD_ZERO(&set);
    FD_SET(ths->pipe_assign[0], &set);

    maxfd = um_max(maxfd, ths->pipe_assign[0]);

    while(1) {
        struct timeval tv = {1, 0};
        int nready = select(maxfd+1, &set, NULL, NULL, &tv);
        if(nready < 0) {
            ulogerr("select return error.\n");
            continue;
        }

        if(nready == 0) {
            /* repeat work. */
            
            continue;
        }

        if(nready == 1) {
            /* receive task notification. */

            continue;
        }
    }

    return NULL;
}


struct uthreads* uthreads_create(int number)
{
    int ret = 0;

    struct uthreads* ths = um_malloc(sizeof(*ths));
    memset(ths, 0, sizeof(*ths));
    ths->number = number;
    ths->sntask_getto = 0;

    ret = pthread_create(&ths->pth_assign, NULL, func_assign, ths);
    ret |= pipe(ths->pipe_assign);

    ths->task_status_records = uarray_create(sizeof(struct uthread_task_record), -1);
    ths->tasks = ulinklist_create();

    ths->threads = um_malloc(sizeof(struct uthread)*ths->number);

    if(0 != ret || NULL == ths->task_status_records || NULL == ths->tasks) {
        ret = -1;
        goto finish;
    }

    int idx;
    for(idx=0; idx<ths->number; idx++) {
        struct uthread* th = &ths->threads[idx];
        int ret_pipe = pipe(th->pipe);
        if(0 != ret_pipe) {
            ulogerr("pipe error.\n");
            break;
        }
        th->status = thread_status_init;

        ret = pthread_create(&th->pth, NULL, func, th);
        if(0 != ret) {
            break;
        }
    }

finish:
    if(0 != ret) {
        uthreads_destroy(ths);
        ths = NULL;
    }

    return ths;
}


int uthreads_destroy(struct uthreads* ths)
{
    int ret = 0;
    return ret;
}


long uthreads_addtask(struct uthreads* ths, void *(*start_routine) (void *), void *arg, int ctrl)
{
    int retsntask = -1;

    ths->sntask_getto ++;

    struct u1* task = um_malloc(sizeof(*task));
    task->start_routine = start_routine;
    task->arg = arg;
    task->sntask = ths->sntask_getto ;
    gettimeofday(&task->tv, NULL);

    ulinklist_add_tailer(ths->tasks, task);
    
/*
    int ret = 0;
    int idx = 0;
    for(idx=0; idx<ths->number; idx++) {
        struct uthread* th = &ths->threads[idx];
        if(th->status == thread_status_idle 
                || th->status == thread_status_error) {
            th->start_routine = start_routine;
            th->arg = arg;
            th->status = thread_status_assign;

            int n = write(th->pipe[1], "1", 1);
            if(n == 1) {
                ulogdbg("assign task %ld\n", ths->sntask);
            }
            else {
                th->start_routine = NULL;
                th->arg = NULL;
                th->status = thread_status_idle;
                ret = -1;
            }

            break;
        }
    }

    if(0 == ret) {
        retsntask = ths->sntask;
    }
*/

    return retsntask;
}






uthread_task_e uthread_query(struct uthreads* ths, long sn);
