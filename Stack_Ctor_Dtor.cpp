#include "My_features.h"


int Stack_Ctor (Stack* stk , int capacity)
{
    my_assert (stk == NULL);
    stk->canary_1 = val_canary_1;
    stk->canary_2 = val_canary_2;
    stk->size = 0;
    stk->checksum_arr = 0;
    stk->checksum_stack = 0;
    stk->capacity = capacity;


    if (((capacity * sizeof (stack_element)) % 8) == 0)

            stk->length_bytes_aligment = 0;
    else

        stk->length_bytes_aligment = (8 - (capacity * sizeof (stack_element)) % 8);

    stack_element* data = (stack_element*) ((char*) calloc (1, capacity * sizeof(stack_element) + 2 * sizeof(canary_t) + stk->length_bytes_aligment));
    my_assert(data == NULL);

    *((canary_t*) data) = val_canary_3;

    stk->data = (stack_element*) ((canary_t*) data + 1);

    *((canary_t*) ((char*) (stk->data + capacity) + stk->length_bytes_aligment)) = val_canary_4;

    stk->checksum_stack = Calculate_stack_checksum (stk);

    return COMPLETE_VALUE;
}
//---------------------------------------------------------------------------------------------------------------
int Stack_Dtor (Stack* stk)
{
    my_assert(stk == NULL);

    if (stk->data != NULL) {

        free ((canary_t*) stk->data - 1);
        stk->data = NULL;
    }

    return COMPLETE_VALUE;
}
