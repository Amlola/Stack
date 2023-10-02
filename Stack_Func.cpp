#include "header.h"


void StackCtor(Stack* stk)
    {
    CreateErrorArray(stk);

    if (logfile == NULL)
        {
        logfile = fopen("logfile.txt", "w");
        }

    stk->stack_size = sz;
    stk->stack_pos = 0;

    stk->stack_data = ((Stack_type*)calloc(stk->stack_size * sizeof(Stack_type) + StackCanarySize(), sizeof(char)));

    stk->stack_data = (Stack_type*)((char*)(stk->stack_data) + sizeof(canary_type));

    ON_CANARY
        (
        LEFTCANARYDATA = 0xDEAD;
        RIGHTCANARYDATA = 0xDEAD;

        stk->LeftCanary = 0xDEAD;
        stk->RightCanary = 0xDEAD;
        )

    PoisonValue(stk);

    ON_HASH
        (
        stk->hash_stack = CalculateHashStack(stk);
        stk->hash_data = CalculateHashData(stk);
        )

    ON_DUMP
        (
        StackDump(stk);
        )
    }



void StackPush(Stack* stk, Stack_type value)
    {
    if (StackOK(stk) == 1)
        {
        if (stk->stack_pos >= stk->stack_size)
            {
            StackResize(stk, stk->stack_size * Size_extend);
            }
        stk->stack_data[stk->stack_pos] = value;
        stk->stack_pos++;
        }

    ON_HASH
        (
        stk->hash_stack = CalculateHashStack(stk);
        stk->hash_data  = CalculateHashData(stk);
        )

    StackOK(stk);

    ON_DUMP
        (
        StackDump(stk);
        )
    }


static void StackResize(Stack* stk, int stack_Newsize)
    {
	ON_DUMP
        (
        StackDump(stk);
        )

    Stack_type* dataResize = ((Stack_type*)calloc(stack_Newsize * sizeof(*stk->stack_data) + StackCanarySize(), sizeof(char)));

    dataResize = (Stack_type*)((char*)(dataResize) + sizeof(canary_type));

    if (StackOK(stk) == 1)
        {
        Copy(stk, dataResize);

        free((Stack_type*)((char*) stk->stack_data - sizeof(canary_type)));

        stk->stack_data = dataResize;
        stk->stack_size = stack_Newsize;

        ON_CANARY
            (
            LEFTCANARYDATA = 0xDEAD;
            RIGHTCANARYDATA = 0xDEAD;
            )

        PoisonValue(stk);
        }

    ON_HASH
        (
        stk->hash_stack = CalculateHashStack(stk);
        stk->hash_data  = CalculateHashData(stk);
        )

    StackOK(stk);

	ON_DUMP
        (
        StackDump(stk);
        )
    }


static void Copy(Stack* stk, Stack_type* dataResize)
    {
    for (int i = 0; i < stk->stack_size; i++)
        {
        dataResize[i] = stk->stack_data[i];
        }
    }


static size_t StackCanarySize()
    {
    #ifdef CANARY
        return 2 * sizeof(canary_type);
    #else
        return 0;
    #endif
    }


void StackPop(Stack* stk, Stack_type* retvalue)
    {
    ON_DUMP
        (
        StackDump(stk);
        )

    if (StackOK(stk) == 1)
        {
        if (stk->stack_pos < 0)
            {
            *retvalue = 0;
            }
        else
            {
            stk->stack_pos--;
            *retvalue = stk->stack_data[stk->stack_pos];
            stk->stack_data[stk->stack_pos] = POISON_NUMBER_FOR_VALUE;

            if (stk->stack_pos < stk->stack_size / 4)
                {
                ON_HASH
                    (
                    stk->hash_stack = CalculateHashStack(stk);
                    stk->hash_data  = CalculateHashData(stk);
                    )
                StackResize(stk, stk->stack_size / 2);
                }
            }
        }

    ON_HASH
        (
        stk->hash_stack = CalculateHashStack(stk);
        stk->hash_data  = CalculateHashData(stk);
        )

    StackOK(stk);

    ON_DUMP
        (
        StackDump(stk);
        )
    }


static void PoisonValue(Stack* stk)
    {
    if (stk->stack_size > 0 && stk->stack_data != nullptr)
        {
        for (size_t i = stk->stack_pos; i < stk->stack_size; i++)
            {
            stk->stack_data[i] = POISON_NUMBER_FOR_VALUE;
            }
        }
    }


void StackDtor(Stack* stk)
    {
    ON_DUMP
        (
        StackDump(stk);
        )

    if (stk->stack_data != nullptr)
        {
        free((Stack_type*)((char*) stk->stack_data - sizeof(canary_type)));
        stk->stack_data = POISON_VALUE_FOR_ADRESS;
        }

    ON_HASH
        (
        stk->hash_stack = 0xABCDEEEF;
        stk->hash_data  = 0xABCDEEEF;
        )

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
        ON_CANARY
            (
            if (LEFTCANARYDATA != 0xDEAD)
                {
                stk->stack_status[DATA_LEFT_CANARY_DAMAGED] = 1;
                stk->stack_status[NO_ERROR] = 0;
                }

            if (RIGHTCANARYDATA != 0xDEAD)
                {
                stk->stack_status[DATA_RIGHT_CANARY_DAMAGED] = 1;
                stk->stack_status[NO_ERROR] = 0;
                }
            )


        ON_HASH
            (
            if (stk->hash_data != CalculateHashData(stk))
                {
                stk->stack_status[WRONG_HASH_DATA] = 1;
                stk->stack_status[NO_ERROR] = 0;
                }
            )

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

    ON_HASH
        (
        if (stk->hash_stack != CalculateHashStack(stk))
            {
            stk->stack_status[WRONG_HASH_STACK] = 1;
            stk->stack_status[NO_ERROR] = 0;
            }
        )

    ON_CANARY
        (
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
        )

    return stk->stack_status[NO_ERROR];
    }


ON_DUMP
    (
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

        ON_HASH
            (
            fprintf(logfile, "hash data = %lld\n", stk->hash_data);
            fprintf(logfile, "hash stack = %lld\n", stk->hash_stack);
            )

        ON_CANARY
            (
            fprintf(logfile, "left_canary = %lld\n", stk->LeftCanary);
            fprintf(logfile, "right_canary = %lld\n", stk->LeftCanary);
            )

        if (stk->stack_data != NULL && stk->stack_size > 0 && stk != nullptr)
            {
            for (int i = 0; i < stk->stack_pos; i++)
                {
                fprintf(logfile, "*[%d] = %d\n", i, stk->stack_data[i]);
                }
            for (int i = stk->stack_pos; i <= stk->stack_size - 1; i++)
                {
                fprintf(logfile, "[%d] = POISON\n", i);
                }
            }
        else
            {
            fprintf(logfile, "data pointer is NULL\n");
            }

        fprintf(logfile, "\n");
        fprintf(logfile, "\n");
        }
    )


void CreateErrorArray(Stack* stk)
    {
    if (stk != nullptr)
        {
        stk->stack_status[NO_ERROR] = 1;
        for (size_t i = 1; i < NUMBER_OF_ERROR; i++)
            {
            stk->stack_status[i] = 0;
            }
        }
    }


const char* Name_Error(size_t j)
    {
    switch (j)
        {
        case NO_ERROR:
            return "NO ERROR\0";

        case NULL_STACK:
            return "NULL STACK\0";

        case NULL_DATA:
            return "NULL DATA\0";

        case SIZE_LESS_THAN_ZERO:
            return "SIZE LESS THAN ZERO\0";

        case POS_LESS_THAN_ZERO:
            return "POS LESS THAN ZERO\0";

        case SIZE_LESS_THAN_POS:
            return "SIZE LESS THAN POS\0";

        ON_CANARY
            (
            case STACK_LEFT_CANARY_DAMAGED:
                return "STACK LEFT CANARY DAMAGED\0";

            case STACK_RIGHT_CANARY_DAMAGED:
                return "STACK RIGHT CANARY DAMAGED\0";

            case DATA_LEFT_CANARY_DAMAGED:
                return "DATA LEFT CANARY DAMAGED\0";

            case DATA_RIGHT_CANARY_DAMAGED:
                return "DATA RIGHT CANARY DAMAGED\0";
            )

        ON_HASH
            (
            case WRONG_HASH_DATA:
                return "WRONG HASH DATA\0";

            case WRONG_HASH_STACK:
                return "WRONG HASH STACK\0";
            )


        case USE_STACK_AFTER_DESTROY:
            return "USE STACK AFTER DESTROY\0";

        default:
            return 0;
        }
    }


ON_HASH
    (
    hash_type static Hash(void* memory, size_t number_of_bytes)
        {
        hash_type sum = 0;

        for (size_t i = 0; i < number_of_bytes; i++)
            {
            sum += (*((char*)memory + i) + 1143) * (32 * number_of_bytes * i);
            }

        return sum;
        }


    hash_type static CalculateHashStack(Stack* stk)
        {
        hash_type hash_stack_old = stk->hash_stack;
        hash_type hash_data_old  = stk->hash_data;
        stk->hash_data = 0;
        stk->hash_stack = 0;

        hash_type hash = Hash((void*)stk, sizeof(*stk));
        hash += (hash_type)(stk->stack_data);

        stk->hash_stack = hash_stack_old;
        stk->hash_data  = hash_data_old;

        return hash;
        }


    hash_type static CalculateHashData(Stack* stk)
        {
        hash_type hash = 0;
        if (stk->stack_data != nullptr && stk->hash_stack == CalculateHashStack(stk) && stk->stack_size > 0)
            {
            hash = Hash((void*)stk->stack_data, stk->stack_size * sizeof(Stack_type));
            }

        return hash;
        }
    )
