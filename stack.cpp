#include "header.h"
//#include "TXLib.h"



int main()
    {
    Stack_type retvalue = 0;

    Stack stk;

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
