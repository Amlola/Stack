#include "header.h"



int main()
    {
    Stack stk = {};

    Stack_type retvalue = 0;

    StackCtor(&stk);

    //stk.stack_data = nullptr;

    //stk.stack_size = -1;

    //stk.stack_pos = -1;

    StackPush(&stk, 4);

    StackPush(&stk, 4);
    StackPush(&stk, 4);
    StackPush(&stk, 4);
    StackPush(&stk, 4);


    StackPop(&stk, &retvalue);
        
    StackDtor(&stk);

    return 0;
    }
