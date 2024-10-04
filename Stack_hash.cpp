#include "My_features.h"

unsigned long Calculate_checksum (void* stk , int size_in_bytes)
{
    unsigned long checksum = 0;

    for (int i = 0 ; i < size_in_bytes ; i++)

        checksum += *((char*) stk + i);

    return checksum;
}
//------------------------------------------------------------------------------------------
unsigned long Calculate_stack_checksum (Stack* stk)
{
    unsigned long checksum = 0;

    checksum += Calculate_checksum ((void*) &stk->length_bytes_aligment , sizeof (stk->length_bytes_aligment));
    checksum += Calculate_checksum ((void*) &stk->size , sizeof (stk->size));
    checksum += Calculate_checksum ((void*) &stk->capacity , sizeof (stk->capacity));
    checksum += Calculate_checksum ((void*) &stk->data , sizeof (stk->data));

    return checksum;
}
