#include "My_features.h"


int main ()
{ //abcdef
    int array[20] = {};
    // memset (array, -1, 80); // (unsigned)(char -1) == 256 ^ sizeof - 1 == 255 == FF
    //                         // (unsigned)(int  -1) == 256 ^ sizeof - 1 == 4 * 256 - 1 == FFFFFFFF
    // for (int i = 0; i < 20; i++)
    //     printf ("%x ", array[i]);


    Stack stk = {};
    int stack_attack[10] = {};

    Stack_Ctor(&stk, 5);
    Stack_Push(&stk, 1000);
    Stack_Push(&stk, 2000);
    Stack_Push(&stk, 3000);
    // printf ("&canary4 = %x\n" ,  ((char*) (stk.data + stk.capacity) + stk.length_bytes_aligment));

    // for (int i = 0 ; i < 100 ; i++)
    //     stack_attack[i] = 52;

    Stack_Push(&stk, 4000);
    Stack_Push(&stk, 5000);

    Stack_Push(&stk, 6000);
    Stack_Pop(&stk);
    Stack_Pop(&stk);
    Stack_Pop(&stk);
    Stack_Pop(&stk);

    Stack_Dump(&stk);
    // printf ("&stack_attack = %x" , stack_attack);

    return 0;
}
