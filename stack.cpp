#include "header.h"
//#include "TXLib.h"



int main()
    {
    Stack_type retvalue = 0;

    Stack stk;

    StackCtor(&stk);

    //stk.stack_size = -4;

    //stk.stack_data = nullptr;

    StackPush(&stk, 10);

    StackPush(&stk, 2);

    StackPush(&stk, 4);

    StackPop(&stk, &retvalue);

    printf("%d", retvalue);

    StackPush(&stk, 4);

    StackDtor(&stk);

    return 0;
    }



Stack_type old_hash_stack(Stack* stk)
    {
    return stk->stack_size + stk->stack_pos;
    }


Stack_type old_hash_data(Stack* stk)
    {
    int sum = 0;
    if (stk->stack_data != NULL)
        {
        for(int i = 0; i <= stk->stack_pos; i++)
            {
            if (stk->stack_data[i] != POISON_NUMBER_FOR_VALUE)
                {
                sum += stk->stack_data[i];
                }
            }
        }
    return sum;
    }


void hash_check(Stack* stk)
    {
    if (stk->hash_data != old_hash_data(stk))
    {
        stk->stack_status[WRONG_HASH_DATA] = 1;
    }
    if (stk->hash_stack != old_hash_stack(stk))
    {
        stk->stack_status[WRONG_HASH_STACK] = 1;
    }
}


/*Stack_type StackTop(Stack* stk)
    {
    StackDump(stk);
    if(StackOK(stk) == 1)
        {
        return stk->stack_data[stk->stack_pos - 1];
        }
    StackDump(stk);
    }*/



// Error: code 12;
// path: D:\project\main.cpp:
// function int main():
// line 42

// Data* = 0x32424222;
//      Data[0] = 10;
//
// size = 10;
//

