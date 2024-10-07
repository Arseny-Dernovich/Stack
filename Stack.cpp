#include "My_features.h"
// #define NDEBUG

Stack_error Stack_Ctor (Stack* stk , size_t capacity)
{
    my_assert (stk == NULL);

#ifndef NDEBUG
    stk->canary_1 = val_canary_1;
    stk->canary_2 = val_canary_2;
#endif
    stk->size = 0;
#ifndef NDEBUG
    stk->checksum_arr = 0;
    stk->checksum_stack = 0;
#endif
    stk->capacity = capacity;

    Calculate_Arr_Aligment (capacity , stk);

    stack_element* data = (stack_element*) ((char*) calloc (1 , Calculate_Size_Arr_Bytes (capacity , stk)));
    my_assert(data == NULL);

#ifndef NDEBUG
    *((canary_t*) data) = val_canary_3;
    stk->data = (stack_element*) ((canary_t*) data + 1);
    *((canary_t*) ((char*) (stk->data + capacity) + stk->length_bytes_aligment)) = val_canary_4;
#else
    stk->data = data;
#endif

#ifndef NDEBUG
    stk->checksum_stack = Stack_Calculate_Hashsum(stk);
#endif

    STACK_ASSERT (stk);
    return COMPLETE_VALUE;
}
//---------------------------------------------------------------------
Stack_error Stack_Push (Stack* stk , stack_element value)
{
    my_assert (stk == NULL);
    STACK_ASSERT (stk);

    if (stk->size >= stk->capacity)

        Stack_Realloc_Up (stk);

    stk->data[stk->size++] = value;
#ifndef NDEBUG
    stk->checksum_arr = Calculate_checksum (stk->data , stk->capacity * sizeof (stack_element));
    stk->checksum_stack = Stack_Calculate_Hashsum (stk);
#endif
    STACK_ASSERT (stk);

    return COMPLETE_VALUE;
}
//---------------------------------------------------------------------------------------
stack_element Stack_Pop (Stack* stk)
{
    my_assert(stk == NULL);
    STACK_ASSERT(stk);

    if (stk->size <= 0) {
    printf("Error: массив пустой! Size = %zu\n" , stk->size);
    assert (0);
    }

    stk->size--;

    stack_element popped_value = stk->data[stk->size];

    stk->data[stk->size] = 0;

    if (stk->size < 0.25 * stk->capacity)

        Stack_Realloc_Down (stk);
#ifndef NDEBUG
    stk->checksum_arr = Calculate_checksum (stk->data , stk->capacity * sizeof (stack_element));
    stk->checksum_stack = Stack_Calculate_Hashsum(stk);
#endif
    STACK_ASSERT(stk);


    printf ("%lg\n" , popped_value);

    return popped_value;
}

static Stack_error Stack_Realloc_Up (Stack* stk)
{
    Stack_Realloc (stk , FACTOR_INCR);
    return COMPLETE_VALUE;
}

static Stack_error  Stack_Realloc_Down (Stack* stk)
{
    Stack_Realloc (stk , FACTOR_DECR);
    return COMPLETE_VALUE;
}


static Stack_error Stack_Realloc (Stack* stk , double factor)
{
    my_assert (stk == NULL);

    int new_capacity = stk->capacity * factor;

    Calculate_Arr_Aligment (new_capacity , stk);
#ifndef NDEBUG
    stack_element* new_data = (stack_element*) realloc ((canary_t*) stk->data - 1 , Calculate_Size_Arr_Bytes (new_capacity , stk));
    my_assert (new_data == NULL);

    *((canary_t*) new_data) = val_canary_3;

    stk->data = (stack_element*) ((canary_t*) new_data + 1);
#else
    stack_element* new_data = (stack_element*) realloc (stk->data , Calculate_Size_Arr_Bytes (new_capacity , stk));
    my_assert (new_data == NULL);

    stk->data = new_data;
#endif


    if (factor > 1) {

        memset (stk->data + stk->capacity , 0 , (new_capacity - stk->capacity) * sizeof (stack_element));
    }
#ifndef NDEBUG
    *((canary_t*) ((char*) (stk->data + new_capacity) + stk->length_bytes_aligment)) = val_canary_4;
#endif
    stk->capacity = new_capacity;


    return COMPLETE_VALUE;
}
//-----------------------------------------------------------------------------------------------------------
Stack_error Stack_Dtor (Stack* stk)
{
    my_assert(stk == NULL);
#ifndef NDEBUG
    if (stk->data != NULL) {

        free ((canary_t*) stk->data - 1);
        stk->data = NULL;
    }
#else
    if (stk->data != NULL) {

        free (stk->data);
        stk->data = NULL;
    }
#endif
    return COMPLETE_VALUE;
}

