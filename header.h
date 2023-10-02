#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


typedef int Stack_type;
typedef long long hash_type;
typedef unsigned long long canary_type;


#define POISON_VALUE_FOR_ADRESS (Stack_type*)0xDED

#define POISON_NUMBER_FOR_VALUE 7182818

#define RIGHTCANARYDATA *(canary_type*)((char*)stk->stack_data + stk->stack_size * sizeof(*stk->stack_data))

#define LEFTCANARYDATA *(canary_type*)((char*)stk->stack_data - sizeof(canary_type))


#ifdef DUMP
    #define ON_DUMP(...) __VA_ARGS__
    #define StackDump(stack_ptr) StackDumpFunction(stack_ptr, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#else
    #define ON_DUMP(...)
#endif


#ifdef CANARY
    #define ON_CANARY(...) __VA_ARGS__
#else
    #define ON_CANARY(...)
#endif


#ifdef HASH
    #define ON_HASH(...) __VA_ARGS__
#else
    #define ON_HASH(...)
#endif


typedef enum
    {
    NO_ERROR = 0,
    NULL_STACK = 1,
	NULL_DATA = 2,
	SIZE_LESS_THAN_ZERO = 3,
	POS_LESS_THAN_ZERO = 4,
	SIZE_LESS_THAN_POS = 5,
	STACK_OVERFLOW = 6,
	STACK_LEFT_CANARY_DAMAGED = 7,
	STACK_RIGHT_CANARY_DAMAGED = 8,
	DATA_LEFT_CANARY_DAMAGED = 9,
	DATA_RIGHT_CANARY_DAMAGED = 10,
	WRONG_HASH_DATA = 11,
	WRONG_HASH_STACK = 12,
	USE_STACK_AFTER_DESTROY = 13,
	NUMBER_OF_ERROR = 14,
    }ERROR;


static FILE* logfile = NULL;


const int sz = 4;
const int Size_extend = 2;


struct Stack
    {
    ON_CANARY
        (
        canary_type LeftCanary;
        )

    Stack_type* stack_data;
    int stack_size;
    size_t stack_pos;
    int stack_status[NUMBER_OF_ERROR] = {0};
    ON_HASH
        (
        hash_type hash_stack;
        hash_type hash_data;
        )

    ON_CANARY
        (
        canary_type RightCanary;
        )
    };


void StackCtor(Stack* stk);

void StackPush(Stack* stk, Stack_type value);

void StackPop(Stack* stk, Stack_type* retvalue);

void StackDtor(Stack* stk);

bool StackOK(Stack* stk);

void StackDumpFunction(Stack* stk, const char* path, const char* signature, unsigned line);

void CreateErrorArray(Stack* stk);

static void StackResize(Stack* stk, int stack_Newsize);

static size_t StackCanarySize();

const char* Name_Error(size_t j);

hash_type static Hash(void* memory, size_t number_of_bytes);

hash_type static CalculateHashStack(Stack* stk);

hash_type static CalculateHashData(Stack* stk);

static void PoisonValue(Stack* stk);

static void Copy(Stack* stk, Stack_type* dataResize);

//Stack_type StackTop(Stack* stk);
