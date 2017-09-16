#include "ustandard/ustandard_sys.h"
#include "ustandard/umalloc.h"
#include "ulinklist.h"




struct node {
    struct node* prev;
    struct node* next;

    int payload;
};


struct ulinklist {

    struct node* header; /* default header. */
    struct node* tailer;

    struct node* curr;

    int num;
};


struct ulinklist* ulinklist_create(void)
{
    struct ulinklist* list = um_malloc(sizeof(*list));
    struct node* node = um_malloc(sizeof(*node));
    if(NULL != list && NULL != node) {
        node->prev = NULL;
        node->next = NULL;
        node->payload = 0;
        
        list->header = node;
        list->curr = node;
        list->tailer = NULL;
        list->num = 0;
    }
    else {
        um_free_check(list);
        um_free_check(node);
    }

    return list;
}


int _ulinklist_add_node_after(struct ulinklist* list, struct node* node, struct node* add)
{
    int ret = 0;

    add->next = node->next;
    add->prev = node;

    node->next = add;

    list->num ++;

    return ret;
}


int ulinklist_add_header(struct ulinklist* list, int payload)
{
    int ret = 0;

    struct node* add = um_malloc(sizeof(*add));
    add->payload = payload;

    _ulinklist_add_node_after(list, list->header, add);
    if(1 == list->num) {
        list->tailer = add;
    }
    
    return ret;
}


int ulinklist_add_tailer(struct ulinklist* list, int payload)
{
    int ret = 0;

    struct node* add = um_malloc(sizeof(*add));
    add->payload = payload;

    if(NULL != list->tailer) {
        _ulinklist_add_node_after(list, list->tailer, add);
    }
    else {
        _ulinklist_add_node_after(list, list->header, add);
    }

    list->tailer = add;
    
    return ret;
}


int _ulinklist_remove_node(struct ulinklist* list, struct node* node)
{
    int ret = 0;

    node->prev->next = node->next;
    if(NULL != node->next) {
        node->next->prev = node->prev;
    }
    else {
        if(node->prev == list->header) {
            list->tailer = NULL;
        }
        else {
            list->tailer = node->prev;
        }
    }

    list->num --;

    return ret;
}


int ulinklist_destroy(struct ulinklist* list)
{
    int ret = 0;

    struct node* node;
    while(NULL != (node = list->header->next)) {
        _ulinklist_remove_node(list, node);
        um_free_check(node);
    }

    um_free(list->header);
    um_free(list);

    return ret;
}


int ulinklist_next(struct ulinklist* list)
{
    if(NULL != list->curr->next) {
        list->curr = list->curr->next;
        return list->curr->payload;
    }
    else {
        return 0;
    }
}


int ulinklist_prev(struct ulinklist* list)
{
    if(list->curr == list->header || list->curr->prev == list->header) {
        return 0;
    }
    else {
        list->curr = list->curr->prev;
        return list->curr->payload;
    }
}






























