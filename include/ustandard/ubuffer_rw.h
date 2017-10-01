#ifndef __UBUFFER_RW_H__
#define __UBUFFER_RW_H__
__BEGIN_DECLS
typedef size_t (*func_expand)(size_t size_cur, size_t size_need, size_t size_max);

typedef enum {
    expand_type_equal = 0,  /*equal to need.*/
    expand_type_2power,     /*near 2power.*/
    expand_type_percent,    /*more percent.*/
    expand_type_const,      /*more const value.*/
    expand_type_add_step,   /*add by step.*/
    expand_type_define
}expand_type_e;

typedef union {
    int percent;
    size_t size_const;
    size_t size_step;
    func_expand expand;
}expand_arg;

struct ubuffer_rw* ubuffer_rw_create(size_t size_min, size_t size_max, 
    expand_type_e type, expand_arg arg);


int ubuffer_rw_destroy(struct ubuffer_rw* buffer_rw);


size_t ubuffer_rw_size(struct ubuffer_rw* buffer_rw);


size_t ubuffer_rw_write(struct ubuffer_rw* buffer_rw, const void* ptr, size_t size);


size_t ubuffer_rw_read(struct ubuffer_rw* buffer_rw, void* ptr, size_t size);

__END_DECLS
#endif /* ubuffer_rw.h */
