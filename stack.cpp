#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


typedef int Stack_type;

struct Stack {
    Stack_type* stack_data;
    Stack_type size_stack;
    Stack_type stack_pos;
};



void StackCtor(Stack* stk);
void PushStack(Stack* stk, Stack_type value);

int main()
    {
    Stack stk {};
    StackCtor(&stk);
    PushStack(&stk, 2);
    PushStack(&stk, 3);
    for (Stack_type i = 0; i < stk.stack_pos; i++)
        {
        printf("%d\n", stk.stack_data[i]);
        }
    return 0;
    }


void StackCtor(Stack* stk)
    {
    stk->size_stack = 10;
    stk->stack_pos = 0;
    stk->stack_data = (Stack_type*)calloc(stk->size_stack, sizeof(Stack_type));
    }


void PushStack(Stack* stk, Stack_type value)
    {
    if (stk->stack_pos > stk->size_stack)
        {
        stk->stack_data = (Stack_type*)realloc(stk->stack_data, stk->size_stack * 2);
        stk->size_stack = stk->stack_pos * 2;
        }
    else
        {
        stk->stack_data[stk->stack_pos++] = value;
        }
    }

