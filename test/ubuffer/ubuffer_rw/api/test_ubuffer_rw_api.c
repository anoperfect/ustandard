#include "ustandard/ustandard.h"
//#include "ubuffer_format.inc"
struct ubuffer_rw {
    size_t          size_max;
    expand_type_e   type;  
    expand_arg      arg;

    void*   ptr;
    size_t  total;
    size_t  size;
    size_t  idx;
};
void test_api(void)
{
    struct ubuffer_rw* buffer_rw = NULL;
    expand_arg arg;
    arg.size_step = 95;

    #define LEN 10240
    #define TEST_SIZE_MAX 1000
    char buf_read[LEN];
    char buf_write[LEN];

    #define LEN_STRS    1024
    char strs[LEN_STRS];
    memset(strs, 0, LEN_STRS);
    //--------------------------------------------------------------------------
    printf("test0------\n");
    buffer_rw = 
        ubuffer_rw_create(0, TEST_SIZE_MAX, expand_type_add_step, arg);
    assert(NULL != buffer_rw);

    ubuffer_rw_destroy(buffer_rw);
    buffer_rw = NULL;
    //--------------------------------------------------------------------------



    

    //--------------------------------------------------------------------------
    printf("test1------\n");
    buffer_rw = 
        ubuffer_rw_create(0, TEST_SIZE_MAX, expand_type_add_step, arg);
    assert(NULL != buffer_rw);
    
    size_t size;
    size = ubuffer_rw_size(buffer_rw);
    assert(0 == size);

    size = ubuffer_rw_read(buffer_rw, buf_read, LEN);
    assert(0 == size);

    const char* swrite1 = "1234567890";
    memcpy(buf_write, swrite1, 10);
    size = ubuffer_rw_write(buffer_rw, buf_write, 10);
    assert(10 == size);

    size = ubuffer_rw_read(buffer_rw, buf_read, 1);
    assert(1 == size); 
    assert(0 == memcmp(buf_read, swrite1, 1)); 

    size = ubuffer_rw_read(buffer_rw, buf_read, LEN);
    assert(9 == size); 
    assert(0 == memcmp(buf_read, swrite1+1, 1));

    size = ubuffer_rw_read(buffer_rw, buf_read, LEN);
    assert(0 == size); 
    //--------------------------------------------------------------------------






    //--------------------------------------------------------------------------
    printf("test2------\n");
    size = ubuffer_rw_size(buffer_rw);
    assert(0 == size);

    size = ubuffer_rw_write(buffer_rw, swrite1, 3);
    assert(3 == size); 

    size = ubuffer_rw_size(buffer_rw);
    assert(3 == size);

    size = ubuffer_rw_read(buffer_rw, buf_read, 6);
    assert(3 == size);

    size = ubuffer_rw_size(buffer_rw);
    assert(0 == size);

    size = ubuffer_rw_write(buffer_rw, swrite1, 3);
    assert(3 == size); 

    memset(buf_write, 10, LEN);
    size = ubuffer_rw_write(buffer_rw, buf_write, LEN);
    assert(size == TEST_SIZE_MAX-3);

    size = ubuffer_rw_size(buffer_rw);
    assert(TEST_SIZE_MAX == size);

    memset(buf_read, 11, LEN);
    size = ubuffer_rw_read(buffer_rw, buf_read, 6);
    assert(6 == size);

    assert(0 == memcmp(buf_read, swrite1, 3));
    assert(10 == buf_read[3]);
    assert(10 == buf_read[4]);
    assert(10 == buf_read[5]);
    assert(11 == buf_read[6]);

    size = ubuffer_rw_size(buffer_rw);
    assert((TEST_SIZE_MAX-6) == size);

    memset(buf_read, 11, LEN);
    size = ubuffer_rw_read(buffer_rw, buf_read, 111);
    assert(111 == size);
    assert(0 == memcmp(buf_read, buf_write, 111));

    size = ubuffer_rw_size(buffer_rw);
    assert((TEST_SIZE_MAX-6-111) == size);
    
    memset(buf_write, 20, LEN);
    size = ubuffer_rw_write(buffer_rw, buf_write, LEN);
    assert((111+6) == size);
    
    memset(buf_read, 111, LEN);
    size = ubuffer_rw_read(buffer_rw, buf_read, LEN);
    assert(TEST_SIZE_MAX == size);
    
    size = ubuffer_rw_read(buffer_rw, buf_read, LEN);
    assert(0 == size);

    ubuffer_rw_destroy(buffer_rw);
    buffer_rw = NULL;
    //--------------------------------------------------------------------------






    //--------------------------------------------------------------------------
    printf("test3------\n");
    buffer_rw = 
        ubuffer_rw_create(1, TEST_SIZE_MAX, expand_type_add_step, arg);
    assert(NULL != buffer_rw);

    size_t size_total = 0;
    memset(buf_write, 20, LEN);

    while(size_total < 999) {
        size = ubuffer_rw_write(buffer_rw, buf_write, 9);
        assert(9 == size);
        size_total += size;

        size = ubuffer_rw_size(buffer_rw);
        assert(size_total == size);
    }

    size = ubuffer_rw_size(buffer_rw);
    assert(999 == size);

    size = ubuffer_rw_write(buffer_rw, buf_write, 9);
    assert(1 == size);
    size_total += size;

    size = ubuffer_rw_size(buffer_rw);
    assert(size_total == size);

    ubuffer_rw_destroy(buffer_rw);
    buffer_rw = NULL;
    //--------------------------------------------------------------------------
}


int main()
{
    while(1) {
        test_api();break;
    }




    return 0;
}


//printf( "-----------------------------------------------------------------\n" "%s" "\n-----------------------------------------------------------------\n", strs);
