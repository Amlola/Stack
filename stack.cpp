#include "header.h"



int main()
    {
    Stack_type retvalue = 0;

    Stack_type top = 0;

    Stack stk;

    StackCtor(&stk);

    StackPush(&stk, 2);

    top = StackTop(&stk);

    StackPush(&stk, 3);

    StackPush(&stk, 4);

    StackPop(&stk, &retvalue);

    //printf("%d %d", top, retvalue);

    StackDtor(&stk);

    return 0;
    }

void StackCtor(Stack* stk)
    {

    stk->stack_size = sz;
    stk->stack_pos = poz;

    if (StackOK(stk))
        {
        if (stk->stack_pos == 0)
            {
            stk->stack_data = (Stack_type*)calloc(stk->stack_size, sizeof(Stack_type));
            }
        else
            {
            for (size_t i = 0; i < stk->stack_pos; i++)
                {
                stk->stack_data[i] = 0;
                }
            }
        }

    StackDump(stk);
    }


void StackPush(Stack* stk, Stack_type value)
    {

    if (StackOK(stk))
        {
        if (stk->stack_pos > stk->stack_size)
            {
            stk->stack_data = (Stack_type*)realloc(stk->stack_data, stk->stack_size * 2);
            stk->stack_size *= 2;
            }
        else
            {
            if (stk->stack_pos == stk->stack_size)
                {
                stk->stack_data = (Stack_type*)realloc(stk->stack_data, stk->stack_size * 2);
                stk->stack_size *= 2;
                }
            stk->stack_data[stk->stack_pos] = value;
            stk->stack_pos++;
            }
        }
    StackDump(stk);
    }


void StackPop(Stack* stk, Stack_type* retvalue)
    {

    if (StackOK(stk))
        {
        if (stk->stack_pos < 0)
            {
            *retvalue = 0;
            }
        else
            {
            stk->stack_pos--;
            *retvalue = stk->stack_data[stk->stack_pos];
            }
        }
    StackDump(stk);
    }


Stack_type StackTop(Stack* stk)
    {
    return stk->stack_data[stk->stack_pos];
    }


void StackDtor(Stack* stk)
    {
    StackDump(stk);
    free(stk->stack_data);
    stk->stack_data = POISON_VALUE_FOR_ADRESS;
    }


bool StackOK(Stack* stk)
    {
    assert(stk);
    bool error_status = true;

    if (stk->stack_data == NULL)
        {
        stk->stack_status = NULL_DATA;
		error_status = false;
        }

    else if (stk == NULL)
        {
        stk->stack_status = NULL_STACK;
		error_status = false;
        }

    else if (stk->stack_size < 0)
        {
		stk->stack_status = SIZE_LESS_THAN_ZERO;
		error_status = false;
        }

	else if (stk->stack_pos < 0)
        {
		stk->stack_status = POS_LESS_THAN_ZERO;
		error_status = false;
        }

	else if (stk->stack_pos > stk->stack_size)
        {
		stk->stack_status = SIZE_LESS_THAN_POS;
		error_status = false;
        }

    else if (stk->stack_data == POISON_VALUE_FOR_ADRESS)
        {
        stk->stack_status = USE_STACK_AFTER_DESTROY;
        error_status = false;
        }

    else
        {
        stk->stack_status = NO_ERROR;
        }


    return error_status;
    }


void StackDumpFunction(Stack* stk, const char* path, const char* signature, unsigned line)
    {
    FILE* logfile = fopen("logfile.txt", "w");


    fprintf(logfile, "ERROR: code %d\n", stk->stack_status);
    fprintf(logfile, "path: %s\n", path);
    fprintf(logfile, "in function: %s\n", signature);
    fprintf(logfile, "line: %d\n", line);
    fprintf(logfile, "pos = %d\n", stk->stack_pos);
    fprintf(logfile, "size = %d\n", stk->stack_size);
    fprintf(logfile, "data[%p]\n", stk->stack_data);

    for (size_t i = 0; i < stk->stack_size; i++)
        {
        if (i < stk->stack_pos && stk->stack_data[i] != NULL)
            {
            fprintf(logfile, "*[%d] = %d\n", i, stk->stack_data[i]);
            }
        else
            {
            fprintf(logfile, "[%d] = NAN(POISON)\n", i);
            }
        }
    }


// Error: code 12;
// path: D:\project\main.cpp:
// function int main():
// line 42

// Data* = 0x32424222;
//      Data[0] = 10;
//
// size = 10;
//

