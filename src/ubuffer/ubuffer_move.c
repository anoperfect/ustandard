#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ubuffer_move.h"

/*
void* ubuffer_move_left(void* p, size_t size, size_t size_move)
{
    void* dest = p - size_move;
    int i;

    for(i=0; i<size; i++) {
        ((unsigned char*)dest)[i] = ((unsigned char*)p)[i];
    }

    return p;
}



void* ubuffer_move_right(void* p, size_t size, size_t size_move)
{
    void* dest = p + size_move;
    int i;

    for(i=size-1; i>=0; i--) {
        ((unsigned char*)dest)[i] = ((unsigned char*)p)[i];
    }

    return p;
}
*/
