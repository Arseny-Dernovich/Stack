#include "My_features.h"

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

    fprintf (fp , "Checksum_arr = %lu  Maybe_changed_checksum_arr = %lu\n\n" , stk->checksum_arr , Calculate_checksum (stk->data , stk->capacity * sizeof (stack_element)));

    fprintf (fp , "Checksum_stack = %lu Myabe_changed_checksum_stack = %lu\n\n" , stk->checksum_stack , Calculate_stack_checksum (stk));

    fprintf (fp , "&data = %x\n\n" , stk->data);

    fprintf(fp, "[");

   for (int i = 0; i < stk->capacity; i++) {

        if (i < stk->size)

            fprintf(fp, " %lg ", stk->data[i]);

        else

            fprintf(fp, " _ ");
    }

    fprintf(fp, "]");

    fclose (fp);

    return COMPLETE_VALUE;
}
