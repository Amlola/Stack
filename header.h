#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


#define StackDump(stack_ptr) StackDumpFunction(stack_ptr, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define POISON_VALUE_FOR_ADRESS (Stack_type*)0xDEF

#define POISON_VALUE_FOR_VALUES (Stack_type)0xDEF


/// StackOk()
/// StackDump() - macro
/// StackDumpFunction()
/// StackTop()


enum ERROR
    {
    NO_ERROR,
    NULL_STACK,
	NULL_DATA,
	SIZE_LESS_THAN_ZERO,
	POS_LESS_THAN_ZERO,
	SIZE_LESS_THAN_POS,
	USE_STACK_AFTER_DESTROY,
    };


const size_t sz = 10;


typedef int Stack_type;

struct Stack
    {
    Stack_type* stack_data;
    size_t stack_size;
    size_t stack_pos;
    ERROR stack_status;
    };


void StackCtor(Stack* stk);

void StackPush(Stack* stk, Stack_type value);

void StackPop(Stack* stk, Stack_type* retvalue);

void StackDtor(Stack* stk);

bool StackOK(Stack* stk);

void StackDumpFunction(Stack* stk, const char* path, const char* signature, unsigned line);


