
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>
// #include "Stack.h"


const char* const yellow          = "\033[3;33;40m";
const char* const red             = "\033[3;31;40m";
const char* const blue_dark       = "\033[3;34;40m";
const char* const blue            = "\033[3;36;40m";
const char* const blue_dark_flash = "\033[5;34;40m";
const char* const green           = "\033[3;32;40m";
const char* const end             = "\033[0m";

typedef double stack_element;
typedef long long int canary_t;


#define STACK_ASSERT(stk)                                   \
        if (Stack_Error (stk) != COMPLETE_VALUE) {          \
            Stack_Dump (stk);                               \
            assert (0);                                     \
        }

// stk.data[5] = 10;

struct Stack {
    canary_t canary_1;
    stack_element* data; // poison
    unsigned int length_bytes_aligment;
    size_t size;
    size_t capacity;
    unsigned long checksum;
    canary_t canary_2;
};

enum Stack_Error {
    BAD_HASH = 52 ,
    BAD_SIZE = 111 , // size > capacity
    BAD_CANARY_1 = 1 ,
    BAD_CANARY_2 = 2 ,
    BAD_CANARY_3 = 3 ,
    BAD_CANARY_4 = 4 ,
    COMPLETE_VALUE = 0 ,

};

enum canary_values {
    val_canary_1 = 777 ,
    val_canary_2 = 666 ,
    val_canary_3 = 334 ,
    val_canary_4 = 333
};

const double FACTOR_INCR = 2;
const double FACTOR_DECR = 0.5;

int Stack_Dump (Stack* stk);

#ifndef NDEBUG


    #define my_assert(condition)                                                                    \
        if ((condition) == true) {                                                                  \
            printf ("Assertaion Error : "#condition" , file  %s , line %d" , __FILE__ , __LINE__);  \
            abort ();                                                                               \
        }                                                                                           \

#else

    #define my_assert(condition)

#endif




int Stack_Ctor (Stack* stk , int capacity)
{
    my_assert (stk == NULL);
    stk->canary_1 = val_canary_1;
    stk->canary_2 = val_canary_2;
    stk->size = 0;
    stk->checksum = 0;
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

    // printf ("%d" , *stk->data);
    // Stack_Dump (stk);

    return COMPLETE_VALUE;
}
//-------------------------------------------------------------------------------------------------
unsigned long Calculate_checksum (Stack* stk)
{
    unsigned long checksum = 0;

    for (int i = 0 ; i < stk->capacity * sizeof (stack_element) ; i++)

        checksum += *((char*) stk->data + i);

    return checksum;
}
//------------------------------------------------------------------------------------------
int Stack_Realloc (Stack* stk , double factor)
{

    int new_capacity = stk->capacity * factor;
    stk->length_bytes_aligment = 0;

    if ((new_capacity * sizeof(stack_element)) % 8 != 0) {
        stk->length_bytes_aligment = 8 - (new_capacity * sizeof(stack_element)) % 8;
    }

    stack_element* new_data = (stack_element*) realloc((canary_t*) stk->data - 1,
            new_capacity * sizeof(stack_element) + 2 * sizeof(canary_t) + stk->length_bytes_aligment);
    my_assert (new_data == NULL);

    *((canary_t*) new_data) = val_canary_3;

    stk->data = (stack_element*) ((canary_t*) new_data + 1);

    if (factor > 1) {

        memset(stk->data + stk->capacity, 0, (new_capacity - stk->capacity) * sizeof(stack_element));
    }

    *((canary_t*) ((char*) (stk->data + new_capacity) + stk->length_bytes_aligment)) = val_canary_4;

    stk->capacity = new_capacity;

    return COMPLETE_VALUE;
}
//---------------------------------------------------------------------------------------------------------------
int Stack_Error (Stack* stk)
{

    my_assert(stk == NULL);

    canary_t* left_data_canary = (canary_t*) stk->data - 1;
    canary_t* right_data_canary = (canary_t*) ((char*) (stk->data + stk->capacity) + stk->length_bytes_aligment);

    if (stk->size > stk->capacity) {

        printf("Error: size больше чем capacity Size: %zu , Capacity: %zu\n" , stk->size , stk->capacity);
        return BAD_SIZE;
    }

    if (stk->canary_1 != val_canary_1) {

        printf("Error: Stack canary_1 повреждено! Expected: %lld , Found: %lld\n" , val_canary_1 , stk->canary_1);
        return BAD_CANARY_1;
    }

    if (stk->canary_2 != val_canary_2) {

        printf("Error: Stack canary_2 повреждено! Expected: %lld , Found: %lld\n" , val_canary_2 , stk->canary_2);
        return BAD_CANARY_2;
    }

    if (*left_data_canary != val_canary_3) {

        printf("Error: Left data canary повреждено! Expected: %lld , Found: %lld\n" , val_canary_3 , *left_data_canary);
        return BAD_CANARY_2;
    }

    if (*right_data_canary != val_canary_4) {

        printf("Error: Right data canary повреждено! Expected: %lld , Found: %lld\n" , val_canary_4 , *right_data_canary);
        return BAD_CANARY_4;
    }

    if (stk->checksum != Calculate_checksum (stk)) {

        printf ("Error: Содержимое массива данных повреждено! Expected: %lu , Found: %lu\n" , stk->checksum , Calculate_checksum (stk));
        return BAD_HASH;
    }

    return COMPLETE_VALUE;
}
//----------------------------------------------------------------------------------------------------------------------
FILE* Open_Log_File (const char* filename_write)
{
    FILE* fp = fopen (filename_write , "w");

    if (fp == NULL) {

        printf ("Error: Oткрыть файл %s не удалось" , filename_write);
        perror ("EDA");

    }

    return fp;
}
//--------------------------------------------------------------------------------------------------------
int Stack_Dump (Stack* stk)
{
    my_assert (stk == NULL);

    canary_t* left_data_canary = (canary_t*) stk->data - 1;
    canary_t* right_data_canary = (canary_t*) ((char*) (stk->data + stk->capacity) + stk->length_bytes_aligment);
    // FILE* fp = stdout;
    FILE* fp = Open_Log_File ("Log_File.txt");

    fprintf (fp , "\n\ncanary_1 = %d , &canary_1 = %x\n\n" , stk->canary_1 , &stk->canary_1);

    fprintf (fp , "canary_2 = %d , &canary_2 = %x\n\n" , stk->canary_2 , &stk->canary_2);

    fprintf (fp , "canary_3 = %d , &canary_3 = %x\n\n" , *left_data_canary , left_data_canary);

    fprintf (fp , "canary_4 = %d , &canary_4 = %x\n\n" , *right_data_canary , right_data_canary);

    fprintf (fp , "size = %d\n\n" , stk->size);

    fprintf (fp , "capacity = %d\n\n" , stk->capacity);

    fprintf (fp , "Checksum = %lu  Maybe_changed_checksum = %lu\n\n" , stk->checksum , Calculate_checksum (stk));

    fprintf (fp , "&data = %x\n\n" , stk->data);

    fprintf (fp , "data = [");

    for (int i = 0 ; i < stk->capacity ; i++) {

        fprintf (fp , " %lg " , stk->data[i]);
    }

    fprintf (fp , "]");

    fclose (fp);

    return COMPLETE_VALUE;
}
//--------------------------------------------------------------------------------------------
int Stack_Push (Stack* stk , stack_element value)
{
    my_assert (stk == NULL); //

    STACK_ASSERT (stk);

    if (stk->size >= stk->capacity)

        Stack_Realloc (stk , FACTOR_INCR);

    stk->data[stk->size++] = value;

    stk->checksum = Calculate_checksum (stk);

    STACK_ASSERT (stk);

    return COMPLETE_VALUE;
}
//---------------------------------------------------------------------------------------
stack_element Stack_Pop (Stack* stk)
{
    my_assert(stk == NULL);

    STACK_ASSERT(stk);

    stk->size--;

    stack_element popped_value = stk->data[stk->size];

    stk->data[stk->size] = 0;

    if (stk->size < stk->capacity / 4)

        Stack_Realloc(stk, FACTOR_DECR);

    stk->checksum = Calculate_checksum (stk);

    STACK_ASSERT(stk);

    printf ("%lg\n" , popped_value);

    return popped_value;
}
//---------------------------------------------------------------------
int Stack_Dtor (Stack* stk)
{
    my_assert(stk == NULL);

    if (stk->data != NULL) {

        free((canary_t*) stk->data - 1);
        stk->data = NULL;
    }

    return COMPLETE_VALUE;
}




int main() {

    Stack stk = {};
    int init_capacity = 5;
    Stack_Ctor (&stk , init_capacity);
    Stack_Push (&stk , 2);
    Stack_Dump (&stk);

    Stack_Push (&stk , 322);
        // Stack_Dump (&stk);

    Stack_Push (&stk , 444);
        // Stack_Dump (&stk);

    Stack_Pop (&stk);
        // Stack_Dump (&stk);

    Stack_Pop (&stk);
        // Stack_Dump (&stk);



    Stack_Pop (&stk);
        // Stack_Dump (&stk);

     Stack_Dtor (&stk);

    return 0;
}



