#include "header.h"



int main()
    {
    Stack_type retvalue = 0;

    Stack_type top = 0;

    Stack stk = {};

    StackCtor(&stk);

    //stk.stack_pos = -1;

    StackPush(&stk, 10);

    StackPop(&stk, &retvalue);

    StackPush(&stk, 2);

    top = StackTop(&stk);

    printf("%d %d", top, retvalue);

    StackPush(&stk, 4);

    StackPop(&stk, &retvalue);

    StackDtor(&stk);

    return 0;
    }

void StackCtor(Stack* stk)
    {
    if (logfile == NULL)
        {
        logfile = fopen("logfile.txt", "w");
        }

    stk->stack_size = sz;
    stk->stack_pos = poz;


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

    StackDump(stk);
    }


void StackPush(Stack* stk, Stack_type value)
    {
    StackOK(stk);

    if (StackOK(stk) == 0)
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
    StackOK(stk);

    if (StackOK(stk) == 0)
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
    return stk->stack_data[stk->stack_pos - 1];
    }


void StackDtor(Stack* stk)
    {
    StackDump(stk);
    free(stk->stack_data);
    stk->stack_data = POISON_VALUE_FOR_ADRESS;
    fclose(logfile);
    }


Stack_type StackOK(Stack* stk)
    {
    assert(stk);

    if (stk->stack_data == NULL)
        {
        stk->stack_status |= NULL_DATA;
        }

    if (stk == NULL)
        {
        stk->stack_status |= NULL_STACK;
        }

    if (stk->stack_size < 0)
        {
		stk->stack_status |= SIZE_LESS_THAN_ZERO;
        }

	if (stk->stack_pos < 0)
        {
		stk->stack_status |= POS_LESS_THAN_ZERO;
        }

	if (stk->stack_pos > stk->stack_size)
        {
		stk->stack_status |= SIZE_LESS_THAN_POS;
        }

    if (stk->stack_data == POISON_VALUE_FOR_ADRESS)
        {
        stk->stack_status |= USE_STACK_AFTER_DESTROY;
        }

    else
        {
        stk->stack_status |= NO_ERROR;
        }

    return (Stack_type) stk->stack_status;
    }


void StackDumpFunction(Stack* stk, const char* path, const char* signature, unsigned line)
    {


    fprintf(logfile, "ERROR: code %s\n", stk->stack_status);
    fprintf(logfile, "path: %s\n", path);
    fprintf(logfile, "in function: %s\n", signature);
    fprintf(logfile, "line: %d\n", line);
    fprintf(logfile, "pos = %d\n", stk->stack_pos);
    fprintf(logfile, "size = %d\n", stk->stack_size);
    fprintf(logfile, "data[%p]\n", stk->stack_data);


    if (stk->stack_data != NULL && stk->stack_size > 0)
        {
        for (size_t i = 0; i < stk->stack_size; i++)
            {
            if (i < stk->stack_pos && stk->stack_data[i] != 0)
                {
                fprintf(logfile, "*[%d] = %d\n", i, stk->stack_data[i]);
                }
            else
                {
                fprintf(logfile, "[%d] = NAN(POISON)\n", i);
                }
            }
        }

    else
        {
        fprintf(logfile, "data pointer is NULL\n");
        }
    fprintf(logfile, "\n");
    fprintf(logfile, "\n");
    }


/*char* Name_Error(Stack* stk)
    {
    if (stk->stack_status == 0)
        {
        return "NO ERROR";
        }

    else if (stk->stack_status == 1)
        {
        return "NULL STACK";
        }

    else if (stk->stack_status == 2)
        {
        return "NULL_DATA";
        }

    else if (stk->stack_status == 4)
        {
        return "SIZE LESS THAN ZERO";
        }

    else if (stk->stack_status == 8)
        {
        return "POS LESS THAN ZERO";
        }

    else if (stk->stack_status == 16)
        {
        return "SIZE LESS THAN POS";
        }

    else
        {
        return "USE_STACK_AFTER_DESTROY";
        }

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

