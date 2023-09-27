#include "header.h"


void StackCtor(Stack* stk)
    {

    FIND_ERROR(stk);

    if (logfile == NULL)
        {
        logfile = fopen("logfile.txt", "w");
        }

    stk->stack_size = sz;
    stk->stack_pos = poz;


    if (stk->stack_pos == 0)
        {
        stk->stack_data = (Stack_type*)calloc(stk->stack_size * sizeof(*stk->stack_data) + 2 * sizeof(long long), sizeof(Stack_type));
        }
    else
        {
        for (int i = 0; i < stk->stack_pos; i++)
            {
            stk->stack_data[i] = 0;
            }
        }
    #ifdef CANARY
        *(long long*)((char*)stk->stack_data) = 0xDEAD;
        *(long long*)((char*)stk->stack_data + 2 * sizeof(long long) + stk->stack_size * sizeof(*stk->stack_data)) = 0xDEAD;

        stk->LeftCanary = 0xDEAD;
        stk->RightCanary = 0xDEAD;
    #endif

    stk->hash_data = data(*stk);
    stk->hash_stack = Stack(*stk);

    StackOK(stk);
    StackDump(stk);

    }



void StackPush(Stack* stk, Stack_type value)
    {
    StackOK(stk);
    StackDump(stk);

    if (StackOK(stk) == 1)
        {
        if (stk->stack_pos > stk->stack_size)
            {
            StackResize(stk, stk->stack_size * 2);
            }
        CHANGE(stk->stack_data, stk->stack_pos) = value;
        stk->stack_pos++;
        }

    stk->hash_data = data(*stk);
    stk->hash_stack = Stack(*stk);

    StackOK(stk);
    StackDump(stk);
    }


static void StackResize(Stack* stk, Stack_type stack_Newsize)
    {
	StackDump(stk);

	if (stack_Newsize <= stk->stack_size)
        {
		stk->stack_status[STACK_OVERFLOW] = 1;
		StackDump(stk);
		exit(EXIT_FAILURE);
        }

	else
        {
        Stack_type* dataResize = (Stack_type*)calloc(stack_Newsize * sizeof(*stk->stack_data) + 2 * sizeof(long long), sizeof(Stack_type));

        if (StackOK(stk) == 1)
            {
             #ifdef CANARY
                *(long long*)((char*)dataResize) = 0xDEAD;
            #endif

            for (int i = 0; i < stk->stack_size; i++)
                {
                CHANGE(dataResize, i) = CHANGE(stk->stack_data, i);
                }
            #ifdef CANARY
                *(long long*)((char*)dataResize + 2 * sizeof(long long) + stack_Newsize * sizeof(*stk->stack_data)) = 0xDEAD;
            #endif

            free(stk->stack_data);

            stk->stack_data = dataResize;
            stk->stack_size = stack_Newsize;
            }
        }

    stk->hash_data = data(*stk);
    stk->hash_stack = Stack(*stk);

	StackOK(stk);
	StackDump(stk);
    }



void StackPop(Stack* stk, Stack_type* retvalue)
    {
    StackOK(stk);
    StackDump(stk);

    if (StackOK(stk) == 1)
        {
        if (stk->stack_pos < 0)
            {
            *retvalue = 0;
            }
        else
            {
            stk->stack_pos--;
            *retvalue = CHANGE(stk->stack_data, stk->stack_pos);
            stk->stack_data[stk->stack_pos] = POISON_NUMBER_FOR_VALUE;
            }
        }

    stk->hash_data = data(*stk);
    stk->hash_stack = Stack(*stk);

    StackOK(stk);
    StackDump(stk);
    }


void StackDtor(Stack* stk)
    {
    StackDump(stk);
    free(stk->stack_data);
    stk->stack_data = POISON_VALUE_FOR_ADRESS;
    fclose(logfile);
    }


bool StackOK(Stack* stk)
    {
    assert(stk);

    if (stk->stack_data == NULL)
        {
        stk->stack_status[NULL_DATA] = 1;
        stk->stack_status[NO_ERROR] = 0;
        }

    else
        {
        if (*(long long*)((char*)stk->stack_data) != 0xDEAD)
            {
            stk->stack_status[DATA_LEFT_CANARY_DAMAGED] = 1;
            stk->stack_status[NO_ERROR] = 0;
            }

        if (*(long long*)((char*)stk->stack_data + 2 * sizeof(long long) + stk->stack_size * sizeof(*stk->stack_data)) != 0xDEAD)
            {
            stk->stack_status[DATA_RIGHT_CANARY_DAMAGED] = 1;
            stk->stack_status[NO_ERROR] = 0;
            }
        }

    if (stk == NULL)
        {
        stk->stack_status[NULL_STACK] = 1;
        stk->stack_status[NO_ERROR] = 0;
        }

    if (stk->stack_size < 0)
        {
        stk->stack_status[SIZE_LESS_THAN_ZERO] = 1;
        stk->stack_status[NO_ERROR] = 0;
        }

	if (stk->stack_pos < 0)
        {
        stk->stack_status[SIZE_LESS_THAN_POS] = 1;
        stk->stack_status[NO_ERROR] = 0;
        }

	if (stk->stack_pos > stk->stack_size)
        {
        stk->stack_status[SIZE_LESS_THAN_POS] = 1;
        stk->stack_status[NO_ERROR] = 0;
        }

    if (stk->stack_data == POISON_VALUE_FOR_ADRESS)
        {
        stk->stack_status[USE_STACK_AFTER_DESTROY] = 1;
        stk->stack_status[NO_ERROR] = 0;
        }

    if (stk->LeftCanary != 0xDEAD)
        {
        stk->stack_status[STACK_LEFT_CANARY_DAMAGED] = 1;
        stk->stack_status[NO_ERROR] = 0;
        }

    if (stk->RightCanary != 0xDEAD)
        {
        stk->stack_status[STACK_RIGHT_CANARY_DAMAGED] = 1;
        stk->stack_status[NO_ERROR] = 0;
        }


    return stk->stack_status[0];
    }


void StackDumpFunction(Stack* stk, const char* path, const char* signature, unsigned line)
    {

    for (size_t j = 0; j < NUMBER_OF_ERROR - 1; j++)
        {
        if (stk->stack_status[j] == 1)
            {
            fprintf(logfile, "ERROR: %s\n", Name_Error(j));
            }
        }

    fprintf(logfile, "path: %s\n", path);
    fprintf(logfile, "in function: %s\n", signature);
    fprintf(logfile, "line: %d\n", line);
    fprintf(logfile, "pos = %d\n", stk->stack_pos);
    fprintf(logfile, "size = %d\n", stk->stack_size);
    fprintf(logfile, "data[%p]\n", stk->stack_data);
    #ifdef HASH
        fprintf(logfile, "hash value = %d\n", stk->hash_data);
    #endif

    #ifdef CANARY
        fprintf(logfile, "left_canary = %lld\n", stk->LeftCanary);
        fprintf(logfile, "right_canary = %lld\n", stk->LeftCanary);
    #endif


    if (stk->stack_data != NULL && stk->stack_size > 0)
        {
        for (int i = 0; i < stk->stack_pos; i++)
            {
            fprintf(logfile, "*[%d] = %d\n", i, CHANGE(stk->stack_data, i));
            }
        for (int i = stk->stack_pos; i <= stk->stack_size; i++)
            {
            fprintf(logfile, "[%d] = NAN(POISON)\n", i);
            }
        }
    else
        {
        fprintf(logfile, "data pointer is NULL\n");
        }

    fprintf(logfile, "\n");
    fprintf(logfile, "\n");
    }


void FIND_ERROR(Stack* stk)
    {
    stk->stack_status = (Stack_type*)calloc(NUMBER_OF_ERROR, sizeof(Stack_type));
    stk->stack_status[0] = 1;
    for (size_t i = 1; i < NUMBER_OF_ERROR - 1; i++)
        {
        stk->stack_status[i] = 0;
        }
    }


const char* Name_Error(size_t j)
    {
    if (j == 0)
        {
        return "NO ERROR\0";
        }

    else if (j == 1)
        {
        return "NULL STACK\0";
        }

    else if (j == 2)
        {
        return "NULL_DATA\0";
        }

    else if (j == 3)
        {
        return "SIZE LESS THAN ZERO\0";
        }

    else if (j == 4)
        {
        return "POS LESS THAN ZERO\0";
        }

    else if (j == 5)
        {
        return "SIZE LESS THAN POS\0";
        }

    else if (j == 7)
        {
        return "STACK_LEFT_CANARY_DAMAGED\0";
        }

    else if (j == 8)
        {
        return "STACK_RIGHT_CANARY_DAMAGED\0";
        }

    else if (j == 9)
        {
        return "DATA_LEFT_CANARY_DAMAGED\0";
        }

    else if (j == 10)
        {
        return "DATA_RIGHT_CANARY_DAMAGED\0";
        }

    else if (j == 13)
        {
        return "USE_STACK_AFTER_DESTROY\0";
        }

    return 0;

    }
