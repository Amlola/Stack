#include "header.h"
//#include "TXLib.h"



int main()
    {
    Stack_type retvalue = 0;

    Stack stk;

    StackCtor(&stk);

    //stk.stack_size = -1;

    for (int i = 1; i < 10; i++)
        {
        StackPush(&stk, i);
        }

    for (int i = 0; i < 8; i++)
        {
        StackPop(&stk, &retvalue);
        }

    printf("%d", retvalue);

    StackPush(&stk, 4);

    StackDtor(&stk);

    return 0;
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

