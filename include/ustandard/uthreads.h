#ifndef __UTHREADS_H__
#define __UTHREADS_H__
__BEGIN_DECLS


struct uthreads; 
struct uthreads* uthreads_create(int number);
int uthreads_destroy(struct uthreads* ths);



long uthreads_addtask(struct uthreads* ths, void *(*start_routine) (void *), void *arg, int ctrl);



typedef enum {
    uthread_task_init = 0,    
    uthread_task_queue,    
    uthread_task_assign,    
    uthread_task_running,    
    uthread_task_finish,    
}uthread_task_e;

uthread_task_e uthread_query(struct uthreads* ths, long sn);


__END_DECLS
#endif /* uthreads.h */
