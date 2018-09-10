#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ulinklist.h"
#include "ustandard/uarray.h"
#include "ustandard/uthreads.h"




/*
    1 thread used for assign task. 
    n thread used for perform task.
 */

struct uthreads_task {
    void *(*start_routine) (void *);
    void *arg;
    long sntask;
    struct timeval tv_add;
    struct timeval tv_assign;
    struct timeval tv_perform;
    struct timeval tv_finish;
};


struct uthreads;
struct uthread {
    int idx;
    int pipe[2];    /* wait for cmd, send the response. */
    pthread_t pth;
    int status; /* 0:idle, 1,running, 2.commit. 3.error.*/

    struct uthreads_task task;

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
    struct uthread* threads_free;
    
    struct uarray* task_status_records; /* recoord the sntask and status, use uthread_query to query the status.*/

    pthread_mutex_t lock_tasks;
    struct ulinklist* tasks;    /* task list. */

    pthread_t pth_assign;
    int pipe_assign[2];

};




struct uthread_task_record {
    long sn;
    uthread_task_e status;
};



void* func_perform(void* arg)
{
    uloginf("\n");
    struct uthread* th = arg;
    char ch;

    th->status = thread_status_idle;

    while(1) {
        uloginf("read %d\n", th->pipe[0]);
        int n = read(th->pipe[0], &ch, 1);
        uloginf("read %d , return %d\n", th->pipe[0], n);
        if(n == 1) {
            th->status = thread_status_assign;
            ulogdbg("thread task status : assign.\n");

            if(NULL != (th->task.start_routine)) {
                th->status = thread_status_running;
                ulogdbg("thread task status : running.\n");
                th->task.start_routine(th->task.arg);
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


struct uthread* func_assign_find_free_thread(struct uthreads* ths)
{
    uloginf("\n");
    int idx = 0;
    for(idx=0; idx<ths->number; idx++) {
        struct uthread* th = &ths->threads[idx%ths->number];
        if(th->status == thread_status_idle 
                || th->status == thread_status_error) {
            uloginf("\n");
            return th;
        }
    }

    uloginf("\n");
    return NULL;
}


int func_assign_1task(struct uthreads* ths, struct uthreads_task* task)
{
    uloginf("\n");
    int ret = 0;
    struct uthread* th = func_assign_find_free_thread(ths);
    if(th) {
        th->task = *task;

        uloginf("write %d.\n", th->pipe[1]);
        int n = write(th->pipe[1], "1", 1);
        uloginf("write %d return %d.\n", th->pipe[1], n);
        if(n == 1) {
            ulogdbg("assign task %ld\n", task->sntask);
        }
        else {
            memset(&th->task, 0, sizeof(struct uthreads_task));
            ret = -1;
        }
    }
    else {
        ret = -1;
    }

    return ret;
}


int func_assign_tasks(struct uthreads* ths)
{
    int ret = 0;
    uloginf("\n");
    pthread_mutex_lock(&ths->lock_tasks);

    while(1) {
        struct uthreads_task* task = ulinklist_pop(ths->tasks);
        if(!task) {
            break;
        }

        ret = func_assign_1task(ths, task);
        if(0 == ret) {

        }
        else {
            ulinklist_add_header(ths->tasks, task);
            break;
        }
    }

    pthread_mutex_unlock(&ths->lock_tasks);
    return ret;
}


void* func_assign(void* arg)
{
    struct uthreads* ths = arg;

    fd_set set;
    int maxfd = 0;

    while(1) {
        FD_ZERO(&set);
        FD_SET(ths->pipe_assign[0], &set);
        maxfd = um_max(maxfd, ths->pipe_assign[0]);
        struct timeval tv = {1, 0};
        int nready = select(maxfd+1, &set, NULL, NULL, &tv);
        if(nready < 0) {
            ulogerr("select return error.\n");
            continue;
        }

        if(nready == 0) {
            /* repeat work. */
            uloginf("loop timeout.\n");
            continue;
        }

        if(nready == 1) {
            /* receive task notification. */
            uloginf("receive task notification.\n");
            char s[1];
            ssize_t nread = read(ths->pipe_assign[0], s, 1);
            if(nread != 1) {
                ulogerr("\n");
            }

            func_assign_tasks(ths);


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

    ret = pthread_create(&ths->pth_assign, NULL, func_assign, ths);
    ret |= pipe(ths->pipe_assign);

    ths->task_status_records = uarray_create(sizeof(struct uthread_task_record));
    ths->tasks = ulinklist_create();

    ths->threads = um_malloc(sizeof(struct uthread)*ths->number);

    uloginf("\n");

    if(0 != ret || NULL == ths->task_status_records || NULL == ths->tasks) {
        ulogerr("\n");
        ret = -1;
        goto finish;
    }

    uloginf("\n");

    int idx;
    for(idx=0; idx<ths->number; idx++) {
        uloginf("\n");
        struct uthread* th = &ths->threads[idx];
        int ret_pipe = pipe(th->pipe);
        if(0 != ret_pipe) {
            uloginf("\n");
            ulogerr("pipe error.\n");
            break;
        }
        th->status = thread_status_init;

        ret = pthread_create(&th->pth, NULL, func_perform, th);
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

/* ctrl. */
long uthreads_addtask(struct uthreads* ths, void *(*start_routine) (void *), void *arg, int ctrl)
{
    int retsntask = -1;

    struct uthreads_task* task = um_malloc(sizeof(*task));
    task->start_routine = start_routine;
    task->arg = arg;
    gettimeofday(&task->tv_add, NULL);

    /* different thread call, need to lock. 1.sntask; 2.tasks.  */
    pthread_mutex_lock(&ths->lock_tasks);

    ths->sntask_getto ++;
    task->sntask = ths->sntask_getto ;
    retsntask = ths->sntask_getto;
    ulinklist_add_tailer(ths->tasks, task);

    pthread_mutex_unlock(&ths->lock_tasks);

    /* notify the assign thread task there is new task. */
    int nwrite = write(ths->pipe_assign[1], "0", 1);
    if(nwrite != 1) {
        ulogerr("send notify error.\n");
    }

    return retsntask;
}






uthread_task_e uthread_query(struct uthreads* ths, long sn);
