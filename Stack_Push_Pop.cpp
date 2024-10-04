#include "My_features.h"


static int Stack_Realloc (Stack* stk , double factor)
{
    my_assert (stk == NULL);

    int new_capacity = stk->capacity * factor;
    stk->length_bytes_aligment = 0;

    if ((new_capacity * sizeof(stack_element)) % 8 != 0) {
        stk->length_bytes_aligment = 8 - (new_capacity * sizeof(stack_element)) % 8;
    }

    stack_element* new_data = (stack_element*) realloc ((canary_t*) stk->data - 1,
            new_capacity * sizeof (stack_element) + 2 * sizeof (canary_t) + stk->length_bytes_aligment);
    my_assert (new_data == NULL);

    *((canary_t*) new_data) = val_canary_3;

    stk->data = (stack_element*) ((canary_t*) new_data + 1);

    if (factor > 1) {

        memset (stk->data + stk->capacity , 0 , (new_capacity - stk->capacity) * sizeof(stack_element));
    }

    *((canary_t*) ((char*) (stk->data + new_capacity) + stk->length_bytes_aligment)) = val_canary_4;

    stk->capacity = new_capacity;

    return COMPLETE_VALUE;
}
//--------------------------------------------------------------------------------------------
int Stack_Push (Stack* stk , stack_element value)
{
    my_assert (stk == NULL);
    STACK_ASSERT (stk);

    if (stk->size >= stk->capacity)

        Stack_Realloc (stk , FACTOR_INCR);

    stk->data[stk->size++] = value;
    stk->checksum_arr = Calculate_checksum (stk->data , stk->capacity * sizeof (stack_element));
    stk->checksum_stack = Calculate_stack_checksum (stk);

    STACK_ASSERT (stk);
    void_arr (stk->size);

    return COMPLETE_VALUE;
}
//---------------------------------------------------------------------------------------
stack_element Stack_Pop (Stack* stk)
{
    Stack_Dump (stk);
    my_assert(stk == NULL);
    STACK_ASSERT(stk);
    void_arr (stk->size);

    stk->size--;

    stack_element popped_value = stk->data[stk->size];

    stk->data[stk->size] = 0;

    if (stk->size < 0.25 * stk->capacity)

        Stack_Realloc(stk, FACTOR_DECR);

    stk->checksum_arr = Calculate_checksum (stk->data , stk->capacity * sizeof (stack_element));
    stk->checksum_stack = Calculate_stack_checksum (stk);

    STACK_ASSERT(stk);


    printf ("%lg\n" , popped_value);

    return popped_value;
}

