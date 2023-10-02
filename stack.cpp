#include "header.h"



int main()
    {
    Stack stk;

    Stack_type retvalue = 0;

    StackCtor(&stk);

    //stk.stack_data = nullptr;

    for (int i = 1; i < 6; i++)
        {
        StackPush(&stk, i);
        }

    for (int i = 0; i < 2; i++)
        {
        StackPop(&stk, &retvalue);
        }

    printf("%d", retvalue);

    StackPush(&stk, 4);

    StackDtor(&stk);

    return 0;
    }
