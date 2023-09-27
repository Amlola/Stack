#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define DUMP
#define HASH
#define CANARY


#ifdef DUMP
    #define StackDump(stack_ptr) StackDumpFunction(stack_ptr, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#endif

#ifdef HASH
    #define check_hash(stk) hash_check(&stk)
    #define data(stk) old_hash_stack(&stk)
    #define Stack(stk) old_hash_data(&stk)
#endif



#define POISON_VALUE_FOR_ADRESS (Stack_type*)0xDED

#define POISON_NUMBER_FOR_VALUE 7182818

#define CHANGE(stack_data, i) (*(Stack_type*)((char*)(stack_data) + 2 * sizeof(long long) + (i) * sizeof(*stack_data)))

typedef int Stack_type;

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


const Stack_type sz = 6;
const Stack_type poz = 0;



struct Stack
    {
    long long LeftCanary;
    Stack_type* stack_data;
    Stack_type stack_size;
    Stack_type stack_pos;
    Stack_type* stack_status;
    Stack_type hash_data;
    Stack_type hash_stack;
    long long RightCanary;
    };



void StackCtor(Stack* stk);

void StackPush(Stack* stk, Stack_type value);

void StackPop(Stack* stk, Stack_type* retvalue);

void StackDtor(Stack* stk);

bool StackOK(Stack* stk);

void StackDumpFunction(Stack* stk, const char* path, const char* signature, unsigned line);

void FIND_ERROR(Stack* stk);

static void StackResize(Stack* stk, Stack_type stack_Newsize);

const char* Name_Error(size_t j);

Stack_type old_hash_stack(Stack* stk);

Stack_type old_hash_data(Stack* stk);

void hash_check(Stack* stk);

//Stack_type StackTop(Stack* stk);






