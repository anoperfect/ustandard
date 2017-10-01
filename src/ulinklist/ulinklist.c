#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ulinklist.h"


struct node {
    struct node* prev;
    struct node* next;

    struct uvdata data;
};


struct ulinklist {

    struct node* header; /* default header. */
    struct node* tailer;

    struct node* curr;

    int num;

    uvdata_free_func data_free;
};


static int _ulinklist_remove_node(struct ulinklist* list, struct node* node);

struct ulinklist* ulinklist_create(void)
{
    struct ulinklist* list = um_malloc(sizeof(*list));
    struct node* node = um_malloc(sizeof(*node));
    if(NULL != list && NULL != node) {
        node->prev = NULL;
        node->next = NULL;
        node->data.p = NULL;
        node->data.use_payload = false;
        node->data.sign_remove = false;
        
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


struct ulinklist* ulinklist_create_detail(uvdata_free_func data_free)
{
    struct ulinklist* list = ulinklist_create();
    if(list) {
        list->data_free = data_free;
    }
    return list;
}


int ulinklist_destroy(struct ulinklist* list, bool clear)
{
    int ret = 0;

    struct node* node;
    while(NULL != (node = list->header->next)) {
        _ulinklist_remove_node(list, node);
        if(clear && list->data_free) {
            list->data_free(&node->data);
            memset(&node->data, 0, sizeof(node->data));
        }
        um_free_check(node);
    }

    um_free(list->header);
    um_free(list);

    return ret;
}


int ulinklist_length(struct ulinklist* list)
{
    return list->num;
}


struct uvdata* ulinklist_header(struct ulinklist* list)
{
    return &list->header->data;
}


struct uvdata* ulinklist_tailer(struct ulinklist* list)
{
    return &list->tailer->data;
}


int _ulinklist_add_node_after(struct ulinklist* list, struct node* node, struct node* add)
{
    uslog_check_arg(list != NULL, -1);
    uslog_check_arg(node != NULL, -1);
    uslog_check_arg(add  != NULL, -1);
    
    int ret = 0;

    add->next = node->next;
    add->prev = node;

    node->next = add;

    list->num ++;
    if(1 == list->num || node == list->tailer) {
        list->tailer = add;
    }

    return ret;
}


struct node* _ulinklist_new_node(void* p)
{
    struct node* add = um_malloc(sizeof(*add));
    add->data.p = p;
    add->data.use_payload = false;
    add->data.sign_remove = false;
    add->data.size = 0;

    return add;
}


struct node* _ulinklist_new_node_by_copy(void* p, size_t size)
{
    struct node* add = um_malloc(sizeof(*add) + size);
    add->data.p = NULL;
    add->data.use_payload = true;
    add->data.sign_remove = false;
    add->data.size = size;
    memcpy(add->data.payload, p, size);

    return add;
}


#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

struct node* _ulinklist_find_node_by_data(struct ulinklist* list, struct uvdata* data)
{
    struct node* node = (void*)data - ((size_t)&((struct node*)0)->data);
    /* it's just a simple verification. not strict. */
    if(&node->data == data) {
        return node;
    }
    else {
        ulogerr("ulinklist node from data check failed.\n");
        return NULL;
    }
}


int ulinklist_add_header(struct ulinklist* list, void* p)
{
    int ret = 0;

    struct node* add = _ulinklist_new_node(p);
    _ulinklist_add_node_after(list, list->header, add);
    
    return ret;
}


int ulinklist_add_header_by_copy(struct ulinklist* list, void* p, size_t size)
{
    int ret = 0;

    struct node* add = _ulinklist_new_node_by_copy(p, size);

    _ulinklist_add_node_after(list, list->header, add);
    
    return ret;
}


int ulinklist_add_after(struct ulinklist* list, struct uvdata* data, void* p)
{
    int ret = 0;

    struct node* add = _ulinklist_new_node(p);
    struct node* node = _ulinklist_find_node_by_data(list, data);

    _ulinklist_add_node_after(list, node, add);
    
    return ret;
}


int ulinklist_add_after_by_copy(struct ulinklist* list, struct uvdata* data, void* p, size_t size)
{
    int ret = 0;

    struct node* add = _ulinklist_new_node_by_copy(p, size);
    struct node* node = _ulinklist_find_node_by_data(list, data);

    _ulinklist_add_node_after(list, node, add);
    
    return ret;
}


int ulinklist_add_before(struct ulinklist* list, struct uvdata* data, void* p)
{
    int ret = 0;

    struct node* add = _ulinklist_new_node(p);
    struct node* node = _ulinklist_find_node_by_data(list, data);
    node = node->prev;

    _ulinklist_add_node_after(list, node, add);
    
    return ret;
}


int ulinklist_add_before_by_copy(struct ulinklist* list, struct uvdata* data, void* p, size_t size)
{
    int ret = 0;

    struct node* add = _ulinklist_new_node_by_copy(p, size);
    struct node* node = _ulinklist_find_node_by_data(list, data);
    node = node->prev;

    _ulinklist_add_node_after(list, node, add);
    
    return ret;
}


int ulinklist_remove_data(struct ulinklist* list, struct uvdata* data, bool clear)
{
    int ret = 0;

    struct node* node = _ulinklist_find_node_by_data(list, data);
    if(NULL != node) {
        _ulinklist_remove_node(list, node);

        if(clear && NULL != list->data_free) {
            list->data_free(data);
            memset(data, 0, sizeof(*data));
        }
    }
    
    return ret;
}


int ulinklist_add_tailer(struct ulinklist* list, void* p)
{
    int ret = 0;

    struct node* add = _ulinklist_new_node(p);

    if(NULL != list->tailer) {
        _ulinklist_add_node_after(list, list->tailer, add);
    }
    else {
        _ulinklist_add_node_after(list, list->header, add);
    }

    list->tailer = add;
    
    return ret;
}


int ulinklist_add_tailer_by_copy(struct ulinklist* list, void* p, size_t size)
{
    int ret = 0;

    struct node* add = _ulinklist_new_node_by_copy(p, size);

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


struct uvdata* ulinklist_next(struct ulinklist* list)
{
    if(NULL != list->curr->next) {
        list->curr = list->curr->next;
        return &list->curr->data;
    }
    else {
        return NULL;
    }
}


struct uvdata* ulinklist_prev(struct ulinklist* list)
{
    if(list->curr == list->header || list->curr->prev == list->header) {
        return NULL;
    }
    else {
        list->curr = list->curr->prev;
        return &list->curr->data;
    }
}


/*
    it's list. not good performance.
    use under restrain.
 */
struct uvdata* ulinklist_at(struct ulinklist* list, int idx)
{
    struct uvdata* data = NULL;

    int t = 0;
    struct node* node = list->header;
    while(node && t<idx) {
        node = node->next;
        t ++;
    }

    if(node) {
        data = &node->data;
    }

    return data;
}


