#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ukeyword.h"


#define NUM_MAX_ITEM    10
struct ukeyword_item
{
    void*   item; 
    size_t  size;
    int     sn;
};


struct ukeyword
{
    void*   buf;
    size_t  size;
    size_t  size_total;
    size_t  size_max_item;

    int num_items;
    struct ukeyword_item items[NUM_MAX_ITEM];
};


ukeyword_t* ukeyword_init(void)
{
    size_t size_buf = 1024;
    void* buf = um_malloc(size_buf);
    if(NULL == buf) {
        errno = ENOMEM;
        return NULL;
    }

    ukeyword_t* kw = um_malloc(sizeof(*kw));
    if(NULL == kw) {
        um_free(buf);
        buf = NULL;
        errno = ENOMEM;
        return NULL;
    }

    kw->buf             = buf;
    kw->size            = 0;
    kw->size_total      = size_buf;
    kw->size_max_item   = 0;

    kw->num_items       = 0;
    
    return kw;
}


int ukeyword_register(ukeyword_t* kw, const void* item, size_t size, int sn)
{
    int ret = 0;

    if(kw->num_items >= NUM_MAX_ITEM) {
        ret = -1;
        return ret;
    }

    kw->items[kw->num_items].item = um_malloc(size);
    if(NULL == kw->items[kw->num_items].item) {
        errno = ENOMEM;
        ret = -1;
        return ret;
    }
    memcpy(kw->items[kw->num_items].item, item, size);
    kw->items[kw->num_items].size = size;
    kw->items[kw->num_items].sn   = sn;
    kw->num_items ++;

    kw->size_max_item = size>kw->size_max_item?size:kw->size_max_item;

    return ret;
}


int ukeyword_input(ukeyword_t* kw, unsigned char ch)
{
    int ret = -1;
    int i;
    
    if(kw->size == kw->size_total) {
        memcpy(kw->buf, 
                kw->buf+(kw->size_total-(kw->size_max_item-1)), kw->size_max_item-1);
        kw->size = kw->size_max_item-1; 
    }

    memcpy(kw->buf+kw->size, &ch, 1);
    kw->size ++;
    
    for(i=0; i<kw->num_items; i++) {
        struct ukeyword_item* item_cmp = &kw->items[i];
        if(kw->size >= item_cmp->size 
                && 0 == memcmp(item_cmp->item, kw->buf+kw->size-item_cmp->size, item_cmp->size)) {
            ret = item_cmp->sn;
            break;
        }
    }

    return ret;
}


void ukeyword_deinit(ukeyword_t* kw)
{
    int i;
    for(i=0; i<kw->num_items; i++) {
        um_free(kw->items[i].item);
        kw->items[i].item = NULL;
    }

    um_free(kw);
    kw = NULL;
}
