#include "header.h"


void StackCtor(Stack* stk)
    {
    if (logfile == NULL)
        {
        logfile = fopen("logfile.txt", "w");
        }

    stk->stack_size = sz;
    stk->stack_pos = 0;

    stk->stack_status = NO_ERROR;

    stk->stack_data = ((Stack_type*)calloc(stk->stack_size * sizeof(Stack_type) + StackCanarySize(), sizeof(char)));

    stk->stack_data = (Stack_type*)((char*)(stk->stack_data) + sizeof(canary_type));

    ON_CANARY
        (
        LEFTCANARYDATA   = 0xDEDBEEF;
        RIGHTCANARYDATA  = 0xDEDBEEF;

        stk->LeftCanary  = 0xDEDBEEF;
        stk->RightCanary = 0xDEDBEEF;
        )

    FillPoisonValue(stk);

    ON_HASH
        (
        stk->hash_stack = CalculateHashStack (stk);
        stk->hash_data  = CalculateHashData  (stk);
        )
    }



int StackPush(Stack* stk, Stack_type value)
    {
    if (StackOK(stk) != NO_ERROR) 
        {
        return stk->stack_status;   
        }
    
    ON_DUMP
        (
        StackDump(stk);
        )

    if (stk->stack_pos >= stk->stack_size)
        {
        StackResize(stk, stk->stack_size * Size_extend);
        }

    stk->stack_data[stk->stack_pos] = value;
    stk->stack_pos++;

    ON_HASH
        (
        stk->hash_stack = CalculateHashStack (stk);
        stk->hash_data  = CalculateHashData  (stk);
        )

    ON_DUMP
        (
        StackDump(stk);
        )
    
    return stk->stack_status;
    }


static int StackResize(Stack* stk, int stack_Newsize)
    {

    if (StackOK(stk) != NO_ERROR) 
        {
        return stk->stack_status;
        }

	ON_DUMP
        (
        StackDump(stk);
        )

    Stack_type* dataResize = ((Stack_type*)calloc(stack_Newsize * sizeof(*stk->stack_data) + StackCanarySize(), sizeof(char)));

    dataResize = (Stack_type*)((char*)(dataResize) + sizeof(canary_type));

    Copy(stk, dataResize);

    free((Stack_type*)((char*) stk->stack_data - sizeof(canary_type)));

    stk->stack_data = dataResize;
    stk->stack_size = stack_Newsize;

    ON_CANARY
        (
        LEFTCANARYDATA  = 0xDEDBEEF;
        RIGHTCANARYDATA = 0xDEDBEEF;
        )

    FillPoisonValue(stk);

    ON_HASH
        (
        stk->hash_stack = CalculateHashStack(stk);
        stk->hash_data  = CalculateHashData(stk);
        )

	ON_DUMP
        (
        StackDump(stk);
        )
    
    return stk->stack_status;
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


int StackPop(Stack* stk, Stack_type* retvalue)
    {
    assert(retvalue);

    if (StackOK(stk) != NO_ERROR) 
        {
        return stk->stack_status;
        }

    ON_DUMP
        (
        StackDump(stk);
        )

    if (stk->stack_pos == 0)
        {
        return POP_NULL;
        }

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

    ON_HASH
        (
        stk->hash_stack = CalculateHashStack(stk);
        stk->hash_data  = CalculateHashData(stk);
        )
    
    ON_DUMP
        (
        StackDump(stk);
        )
    
    return stk->stack_status;
    }


static void FillPoisonValue(Stack* stk)
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

    if (stk->stack_data == nullptr)
        {
        stk->stack_status |= NULL_DATA;
        }

    else
        {
        ON_CANARY
            (
            if (LEFTCANARYDATA != 0xDEDBEEF)
                {
                stk->stack_status |= DATA_LEFT_CANARY_DAMAGED;
                }

            if (RIGHTCANARYDATA != 0xDEDBEEF)
                {
                stk->stack_status |= DATA_RIGHT_CANARY_DAMAGED;
                }
            )


        ON_HASH
            (
            if (stk->hash_data != CalculateHashData(stk))
                {
                stk->stack_status |= WRONG_HASH_DATA;
                }
            )

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

    ON_HASH
        (
        if (stk->hash_stack != CalculateHashStack(stk))
            {
            stk->stack_status |= WRONG_HASH_STACK;
            }
        )

    ON_CANARY
        (
        if (stk->LeftCanary != 0xDEDBEEF)
            {
            stk->stack_status |= STACK_LEFT_CANARY_DAMAGED;
            }

        if (stk->RightCanary != 0xDEDBEEF)
            {
            stk->stack_status |= STACK_RIGHT_CANARY_DAMAGED;
            }
        )

    return stk->stack_status;
    }


ON_DUMP
    (
    void StackDumpFunction(Stack* stk, const char* path, const char* signature, unsigned line)
        {
        if (stk->stack_status != NO_ERROR) 
            {
            for (size_t j = 1; j < NUMBER_OF_ERROR; j++)
                {
                if ((stk->stack_status & (1 << j)) > 0)
                    {
                    fprintf(logfile, "ERROR: %s\n", ErrorArray[j].NameError);
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
                if (stk->stack_data != NULL && stk->stack_size > 0 && stk != nullptr && stk->stack_pos > 0)
                    {
                    fprintf(logfile, "hash data = %lld\n", stk->hash_data);
                    fprintf(logfile, "hash stack = %lld\n", stk->hash_stack);
                    }
                )

            ON_CANARY
                (
                if (stk->stack_data != NULL && stk->stack_size > 0 && stk != nullptr && stk->stack_pos > 0)
                    {
                    fprintf(logfile, "left struct canary = 0x%x\n", stk->LeftCanary);
                    fprintf(logfile, "right struct canary = 0x%x\n", stk->LeftCanary);
                    fprintf(logfile, "left data canary = 0x%x\n", LEFTCANARYDATA);
                    fprintf(logfile, "right data canary = 0x%x\n", RIGHTCANARYDATA);
                    }
                )

            if (stk->stack_data != NULL && stk->stack_size > 0 && stk != nullptr && stk->stack_pos > 0)
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

            fprintf(logfile, "\n");
            fprintf(logfile, "\n");
            }

        else 
            {
            fprintf(logfile, "NO ERROR");
            }      
        }
    )



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
