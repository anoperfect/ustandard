#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/uqueue.h"
struct uqueue {
    int                     total;
    int                     num;
    int                     index_item;
    int                     index_add;
    void*                   data;
    size_t                  size;
    udata_copy              copy;
};


#define __uqueue_addr_data(q, idx)  ((void*)q->data + (idx)*((q)->size))


struct uqueue* uqueue_create(int total, size_t size, udata_copy copy)
{
    struct uqueue* queue = NULL;
    queue = um_malloc(sizeof(*queue) + total*size);
    if(NULL != queue) {
        queue->total        = total;
        queue->num          = 0;
        queue->index_item   = 0;
        queue->index_add    = 0;
        queue->data         = (void*)queue + sizeof(*queue);
        queue->size         = size;
        queue->copy         = copy;
    }
    else {
        ulogerr("%s um_malloc(%u) error.\n",__FUNCTION__,sizeof(*queue) + total*size);
    }

    return queue;
}


int uqueue_destroy(struct uqueue* queue)
{
    int ret = 0;

    if(NULL != queue) {
        um_free(queue);
        queue = NULL;
    }

    return ret;
}


int uqueue_num(struct uqueue* queue)
{
    int retn = 0;
    retn = queue->num;
    return retn;
}


int uqueue_push(struct uqueue* queue, void* item)
{
    int ret = 0;

    if(queue->total == queue->num){
        ret = -1;
    }
    else {
        if(NULL != queue->copy) {
            queue->copy(__uqueue_addr_data(queue, queue->index_add), 
                    item, queue->size);
        }
        else {
            memcpy(__uqueue_addr_data(queue, queue->index_add), 
                    item, queue->size);
        }

        queue->index_add = (queue->index_add + 1) % (queue->total);
        queue->num ++;
    }

    return ret;
}


void* uqueue_pop(struct uqueue* queue)
{
    void* item = NULL;
    if(queue->num > 0) {
        item = __uqueue_addr_data(queue, queue->index_item);
        queue->index_item = (queue->index_item+1) % (queue->total);
        queue->num -- ;

        if(0 == queue->num) {
            queue->index_item = 0;
            queue->index_add = 0;
        }
    }

    return item;
}


int uqueue_clear(struct uqueue* queue)
{
    int ret = 0;
    
    if(NULL != queue) {
        queue->num          = 0;
        queue->index_item   = 0;
        queue->index_add    = 0;
    }

    return ret;
}


void* uqueue_pop_with_compare(struct uqueue* queue, udata_cmp cmp, void* arg)
{
    void* item = NULL;
    if(queue->num > 0) {
        item = __uqueue_addr_data(queue, queue->index_item);
        if(0 == cmp(item, arg, queue->size)) {
            queue->index_item = (queue->index_item+1) % (queue->total);
            queue->num -- ;
            if(0 == queue->num) {
                queue->index_item = 0;
                queue->index_add = 0;
            }
        }
        else {
            item = NULL;
        }
    }

    return item;
}


void* uqueue_get(struct uqueue* queue, int index)
{
    void* item = NULL;

    if(index >= 0 && index < queue->num) {
        int index_get = ((queue->index_item) + index) % (queue->total);
        item = __uqueue_addr_data(queue, index_get);
    }

    return item;
}


