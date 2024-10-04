#include "My_features.h"


int main ()
{
    Stack stk = {};
    int init_capacity = 5;
    int fuck_your_stack[10] = {};

    Stack_Ctor(&stk, 3);
    Stack_Push(&stk, 1000);
    Stack_Push(&stk, 2000);
    Stack_Push(&stk, 3000);
    for (size_t pizdec = 0; pizdec < 1020; pizdec++)
    {
        fuck_your_stack[pizdec] = 0;
    }
    Stack_Push(&stk, 4000);
    Stack_Push(&stk, 5000);

    Stack_Push(&stk, 6000);
    Stack_Pop(&stk);
    Stack_Pop(&stk);
    Stack_Pop(&stk);
    Stack_Pop(&stk);

    Stack_Dump(&stk);

    return 0;
}
