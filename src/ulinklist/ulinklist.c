#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ulinklist.h"


struct ulinklist_node {
    struct ulinklist_node* prev;
    struct ulinklist_node* next;

    void* p;
};


struct ulinklist {

    struct ulinklist_node* header; /* default header. */
    struct ulinklist_node* tailer;

    struct ulinklist_node* curr;

    int num;

    uf_pointer_free         pointer_free;
    uf_pointer_description  pointer_description;
};


struct ulinklist* ulinklist_create(void)
{
    struct ulinklist* list = um_malloc(sizeof(*list) + 2 * sizeof(struct ulinklist_node));
    
    if(NULL != list) {
        list->header = (struct ulinklist_node*)((unsigned char*)list + sizeof(*list));
        list->tailer = (struct ulinklist_node*)((unsigned char*)list + sizeof(*list) + sizeof(struct ulinklist_node));

        list->header->prev = NULL;
        list->header->next = list->tailer;
        list->header->p = NULL;

        list->tailer->prev = list->header;
        list->tailer->next = NULL;
        list->tailer->p = NULL;

        list->curr = list->header;
        list->num = 0;

        list->pointer_free          = NULL;
        list->pointer_description   = NULL;
    }

    return list;
}


struct ulinklist* ulinklist_create_detail(uf_pointer_free pointer_free, uf_pointer_description  pointer_description)
{
    struct ulinklist* list = ulinklist_create();
    if(list) {
        list->pointer_free          = pointer_free;
        list->pointer_description   = pointer_description;
    }
    return list;
}

static int _ulinklist_remove_node(struct ulinklist* list, struct ulinklist_node* node);

int ulinklist_destroy(struct ulinklist* list)
{
    int ret = 0;

    struct ulinklist_node* node;
    while(list->tailer != (node = list->header->next)) {
        _ulinklist_remove_node(list, node);
            if(list->pointer_free) {
                list->pointer_free(node->p);
            }
            um_free_check(node);
    }

    um_free(list);

    return ret;
}


int ulinklist_length(struct ulinklist* list)
{
    return list->num ;
}


void* ulinklist_header_value(struct ulinklist* list, struct ulinklist_node** ppnode)
{
    if(list->header->next != list->tailer) {
        *ppnode = list->header->next;
        return list->header->next->p;
    }
    else {
        return NULL;
    }
}


void* ulinklist_tailer_value(struct ulinklist* list, struct ulinklist_node** ppnode)
{
    if(list->tailer->prev != list->header) {
        *ppnode = list->tailer->prev;
        return list->tailer->prev->p;
    }
    else {
        return NULL;
    }
}


static int _ulinklist_add_node_after(struct ulinklist* list, struct ulinklist_node* node, struct ulinklist_node* add)
{
    uslog_check_arg(list != NULL, -1);
    uslog_check_arg(node != NULL, -1);
    uslog_check_arg(add  != NULL, -1);
    
    int ret = 0;

    add->next = node->next;
    add->prev = node;

    node->next->prev = add;
    node->next = add;

    list->num ++;

    return ret;
}


struct ulinklist_node* _ulinklist_new_node(void* p)
{
    struct ulinklist_node* add = um_malloc(sizeof(*add));
    add->prev = NULL;
    add->next = NULL;
    add->p = p;

    return add;
}


#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#if 0
struct ulinklist_node* _ulinklist_find_node_by_data(struct ulinklist* list, struct uvdata* data)
{
    struct ulinklist_node* node = (void*)data - ((size_t)&((struct ulinklist_node*)0)->data);
    /* it's just a simple verification. not strict. */
    if(&node->data == data) {
        return node;
    }
    else {
        ulogerr("ulinklist node from data check failed.\n");
        return NULL;
    }
}
#endif



int ulinklist_add_header(struct ulinklist* list, void* p)
{
    int ret = 0;

    struct ulinklist_node* add = _ulinklist_new_node(p);
    _ulinklist_add_node_after(list, list->header, add);
    
    return ret;
}


int ulinklist_add_tailer(struct ulinklist* list, void* p)
{
    int ret = 0;

    struct ulinklist_node* add = _ulinklist_new_node(p);
    _ulinklist_add_node_after(list, list->tailer->prev, add);

    return ret;
}




int _ulinklist_remove_node(struct ulinklist* list, struct ulinklist_node* node)
{
    int ret = 0;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    if(list->pointer_free) {
        if(node->p) {
            list->pointer_free(node->p);
            node->p = NULL;
        }
    }
    um_free(node);
    node = NULL;

    list->num --;

    return ret;
}



#if 0
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
    struct ulinklist_node* node = list->header;
    while(node && t<idx) {
        node = node->next;
        t ++;
    }

    if(node) {
        data = &node->data;
    }

    return data;
}

#endif


void* ulinklist_pop(struct ulinklist* list)
{
    void* retp = NULL;
    if(list->header->next != list->tailer) {
        retp = list->header->next->p;
        _ulinklist_remove_node(list, list->header->next);
    }
    else {
        return NULL;

    }

    return retp;
}


int ulinklist_description(struct ulinklist* list, char* s, size_t size)
{
    int ret = 0;

    struct ulinklist_node* node = list->header->next;
    size_t offset = 0;
    void* p = NULL;
    s[0] = '\0';
    while(node && node != list->tailer) {
        p = node->p;
        offset = strlen(s);
        if(list->pointer_description) {
            list->pointer_description(p, s+offset, size-offset);

            offset = strlen(s);
            snprintf(s+offset, size-offset, " ");
        }
        else {
            snprintf(s+offset, size-offset, "%p ", p);
        }

        node = node->next;
    }

    return ret;
}


#if 0
int ulinklist_add_after(struct ulinklist* list, void* node, void* p)
{
    int ret = 0;

    struct ulinklist_node* add = _ulinklist_new_node(p);
    struct ulinklist_node* node = _ulinklist_find_node_by_data(list, data);

    _ulinklist_add_node_after(list, node, add);
    
    return ret;
}


int ulinklist_add_before(struct ulinklist* list, struct uvdata* data, void* p)
{
    int ret = 0;

    struct ulinklist_node* add = _ulinklist_new_node(p);
    struct ulinklist_node* node = _ulinklist_find_node_by_data(list, data);
    node = node->prev;

    _ulinklist_add_node_after(list, node, add);
    
    return ret;
}



int ulinklist_remove_data(struct ulinklist* list, struct uvdata* data, bool clear)
{
    int ret = 0;

    struct ulinklist_node* node = _ulinklist_find_node_by_data(list, data);
    if(NULL != node) {
        _ulinklist_remove_node(list, node);

        if(clear && NULL != list->data_free) {
            list->data_free(data);
            memset(data, 0, sizeof(*data));
        }
    }
    
    return ret;
}

#endif
