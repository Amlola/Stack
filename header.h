#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>



#define POISON_VALUE_FOR_ADRESS (Stack_type*)0xDED

#define POISON_NUMBER_FOR_VALUE 7182818

#define VAL(stack_data, i) (*(Stack_type*)((char*)(stack_data) + 2 * sizeof(long long) + (i) * sizeof(*stack_data)))

#define RIGHTCANARYDATA(stack_data, stack_size) *(long long*)((char*)stk->stack_data + 2 * sizeof(long long) + stk->stack_size * sizeof(*stk->stack_data))

#define LEFTCANARYDATA(stack_data) *(long long*)((char*)stk->stack_data)

#ifdef DUMP
    #define StackDump(stack_ptr) StackDumpFunction(stack_ptr, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#endif

typedef int Stack_type;
typedef long long hash_t;

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


const Stack_type sz = 2;
const size_t poz = 0;



struct Stack
    {
    #ifdef CANARY
        unsigned long long LeftCanary;
        unsigned long long RightCanary;
    #endif
    Stack_type* stack_data;
    Stack_type stack_size;
    size_t stack_pos;
    int stack_status[NUMBER_OF_ERROR] = {0};
    #ifdef HASH
        hash_t hash_stack;
        hash_t hash_data;
    #endif
    };



void StackCtor(Stack* stk);

void StackPush(Stack* stk, Stack_type value);

void StackPop(Stack* stk, Stack_type* retvalue);

void StackDtor(Stack* stk);

bool StackOK(Stack* stk);

void StackDumpFunction(Stack* stk, const char* path, const char* signature, unsigned line);

void CreateErrorArray(Stack* stk);

static void StackResize(Stack* stk, int stack_Newsize);

static size_t StackCanary(Stack* stk);

const char* Name_Error(size_t j);

hash_t static Hash(void* memory, size_t number_of_bytes);

hash_t static CalculateHashStack(Stack* stk);

hash_t static CalculateHashData(Stack* stk);

int old_hash_data(Stack* stk);

void hash_check(Stack* stk);

//Stack_type StackTop(Stack* stk);






