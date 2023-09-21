#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


#define StackDump(stack_ptr) StackDumpFunction(stack_ptr, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define POISON_VALUE_FOR_ADRESS (Stack_type*)0xDED


/// StackOk()
/// StackDump() - macro
/// StackDumpFunction()
/// StackTop()


typedef enum
    {
    NO_ERROR = 0,
    NULL_STACK = 1,
	NULL_DATA = 2,
	SIZE_LESS_THAN_ZERO = 4,
	POS_LESS_THAN_ZERO = 8,
	SIZE_LESS_THAN_POS= 16,
	USE_STACK_AFTER_DESTROY = 32,
    }ERROR;


static FILE* logfile = NULL;

const size_t sz = 6;
const size_t poz = 0;


typedef int Stack_type;

struct Stack
    {
    Stack_type* stack_data;
    size_t stack_size;
    size_t stack_pos;
    Stack_type stack_status;
    };



void StackCtor(Stack* stk);

void StackPush(Stack* stk, Stack_type value);

void StackPop(Stack* stk, Stack_type* retvalue);

void StackDtor(Stack* stk);

bool StackOK(Stack* stk);

void StackDumpFunction(Stack* stk, const char* path, const char* signature, unsigned line);

Stack_type StackTop(Stack* stk);



