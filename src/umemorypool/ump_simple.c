#include "ustandard/ustandard.h"
#include "ustandard/udevelop.h"
#include "ustandard/ump_simple.h"
#define MIN_TOTAL                   (10*1024)
#define DETAILS_USE_PERCENT         (1)
#define LEN_BORDER                  (4)
struct ump_simple_node {
    size_t offset;
    size_t size_alloc;

    size_t size_free;
};


struct ump_simple {
    size_t  total;
    void*   addr;

    int                     total_nodes;
    int                     num_nodes;
    struct ump_simple_node* nodes; size_t      offset_content;
    size_t      size_content;
};

static void _ump_simple_free_node(struct ump_simple* ump, int idx);


struct ump_simple* ump_simple_create(size_t total)
{
    struct ump_simple* ump = NULL;
    if(total <= MIN_TOTAL) {
        ulogerr("total less then MIN_TOTAL.\n");
        ump = NULL;    
        return NULL;
    }

    ulogdbg("sizeof(struct ump_simple)=%u, sizeof(struct ump_simple_node)=%u.\n", 
            sizeof(struct ump_simple), sizeof(struct ump_simple_node)); 

    void* p = um_malloc(total);
    if(NULL == p) {
        return ump;
    }

    ump = p;
    ump->total = total;
    ump->addr = p;

    ump->total_nodes = 
            ((total-sizeof(struct ump_simple)) * DETAILS_USE_PERCENT / 100) 
            / sizeof(struct ump_simple_node);
    ulogdbg("total_nodes = %d.\n", ump->total_nodes);
    ump->num_nodes = 0;
    ump->nodes = (void*)ump + sizeof(struct ump_simple);
    
    ump->offset_content = sizeof(struct ump_simple) + 
            ump->total_nodes * sizeof(struct ump_simple_node);
    ump->size_content = ump->total - ump->offset_content;
    ulogdbg("offset_content = %u, size_content = %u.\n", 
            ump->offset_content, ump->size_content);

    /* set the first node. */
    struct ump_simple_node* node = &ump->nodes[0];
    node->offset        = 0;
    node->size_alloc    = 0;
    node->size_free     = ump->size_content;
    ump->num_nodes ++;

    return ump;
}


int ump_simple_destroy(struct ump_simple* ump)
{
    int ret = 0;

    if(NULL != ump) {
        um_free(ump);
        ump = NULL;
    }

    return ret;
}


void* ump_simple_alloc(struct ump_simple* ump, size_t size)
{
    void* ptr = NULL;
    size_t size_alloc = um_align4(size) + LEN_BORDER + LEN_BORDER;
    
    struct ump_simple_node* node = NULL;
    struct ump_simple_node* node_new = NULL;
    int i;
    for(i=0; i<ump->num_nodes; i++) {
        if(ump->nodes[i].size_free >= size_alloc) {
            node = &ump->nodes[i];
            break;
        }
    }

    if(NULL != node) {
        if(0 == node->size_alloc) {
            node->size_alloc = size_alloc;
            node->size_free -= size_alloc;
            ptr = (void*) ump->addr + ump->offset_content 
                + node->offset + LEN_BORDER;
        }
        else {
            /* need to add nodes. */
            if(ump->num_nodes >= ump->total_nodes) {
                ulogerr("nodes full.\n");
            }
            else {
                size_t size_free = node->size_free - size_alloc;

                int j;
                for(j=ump->num_nodes-1; j>=(i+1); j--) {
                    ump->nodes[j+1] = ump->nodes[j];
                }
                ump->num_nodes ++;

                node->size_free = 0;

                node_new = node + 1;
                node_new->offset = node->offset + node->size_alloc;
                node_new->size_alloc = size_alloc;
                node_new->size_free = size_free;

                ptr = (void*) ump->addr + ump->offset_content 
                    + node_new->offset + LEN_BORDER;
            }
        }
    }

    return ptr;
}


int ump_simple_free(struct ump_simple* ump, void* ptr)
{
    int ret = 0;
    if(ump == NULL 
            || ptr == NULL 
            || ptr < (ump->addr + ump->offset_content + LEN_BORDER)
            || ptr >= (ump->addr + ump->total)) {
        ulogerr("ump_simple_free. ptr(%p) not free.\n", ptr);
        ret = -1;
        return ret;
    }

    size_t offset_ptr = ptr - ((void*)ump->addr + ump->offset_content) - LEN_BORDER;
    int freed = 0;
    int i;
    for(i=0; i<ump->num_nodes; i++) {
        struct ump_simple_node* tmp = &ump->nodes[i];
        if(tmp->offset > offset_ptr) {
            ulogerr("node not found.\n");
            break;
        }

        if(tmp->offset == offset_ptr) {
            _ump_simple_free_node(ump, i);
            freed = 1;
            break;
        }

        /* continue search. */
    }

    if(!freed) {
        ulogerr("ump_simple_free. ptr(%p) not free.\n", ptr);
        ret = -1;
        return ret;
    }

    return ret;
}


void _ump_simple_free_node(struct ump_simple* ump, int idx)
{
    struct ump_simple_node* node_free = ump->nodes + idx;
    struct ump_simple_node* node_prev = NULL;
    struct ump_simple_node* node_next = NULL;
    node_next = (idx==(ump->num_nodes-1))?NULL:(node_free + 1);

    int idx_move;
    int interval_move = 0;

    if(0 == idx) {
        node_free->size_free += node_free->size_alloc;
        node_free->size_alloc = 0;

        if(NULL != node_next && 0 == node_next->size_alloc) {
            node_free->size_free += node_next->size_free;
            idx_move = 2;
            interval_move = 1;
        }
        else {
            interval_move = 0;
        }
    }
    else {
        node_prev = node_free - 1;
        node_prev->size_free += (node_free->size_alloc + node_free->size_free);

        if(NULL != node_next && 0 == node_next->size_alloc) {
            node_prev->size_free += node_next->size_free;
            interval_move = 2; 
        }
        else {
            interval_move = 1;
        }
        idx_move = idx + interval_move;
    }

    if(interval_move > 0) {
        int i;
        for(i=idx_move;i<ump->num_nodes;i++) {
            ump->nodes[i-interval_move] = ump->nodes[i];
        }
        ump->num_nodes -= interval_move;
    }
}


int ump_sumple_get_node_number(struct ump_simple* ump)
{
    int num = 0;
    num = ump->num_nodes;
    return num;
}


int ump_sumple_get_node(struct ump_simple* ump, 
        int idx, int num, struct ump_simple_node* nodes)
{
    int retn = 0;
    memset(nodes, 0, sizeof(*nodes)*num);
    if(idx>=0 && idx<ump->num_nodes) {
        retn = um_min(ump->num_nodes - idx, num);
        int i;
        for(i=0; i<retn; i++) {
            nodes[i] = ump->nodes[idx+i];
        }
    }

    return retn;
}
