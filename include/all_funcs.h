#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <math.h>
#include <ctype.h>

#define ASSERTED() fprintf(stderr, "Succeeded file: %s func: %s line: %d\n",          \
                                                            __FILE__, __FUNCTION__, __LINE__)
#define CHECK(condition, code_of_error)  (condition) ? code_of_error : STACK_IS_CORRECT;

#define ASSERT_OK(stkPtr)  if (returnStackError(stkPtr) != 0)                        \
                                stackDump(stkPtr, __FILE__, __FUNCTION__, __LINE__);

#define stackCtor(stk, capacity) stack_Ctor(&stk, capacity, #stk,                     \
                                                    __FILE__, __FUNCTION__,__LINE__)

// #define DEBUG 777
// #define MODE DEEP_DEBUG

#define ERR_FILE LOG

// #define GREEN_ "\e[0;32m
// #define RED_ "\e[0;31m
// #define COLOR_END \e[0m"

typedef double elem_t;

enum Stack_Error 
{
    STACK_IS_CORRECT                    = 0,
    STACK_ERROR_NULL                    = 1,
    STACK_ERROR_SIZE_BELOW_ZERO         = 2,
    STACK_ERROR_CAPACITY_BELOW_ZERO     = 4,
    STACK_ERROR_OVERSIZED               = 8,
    STACK_ERROR_SIZE_T_OVERFLOW         = 16,
    LOOK_AT_ERR_FILE_BLET               = 47
};

const int STACK_RESIZE_IF_POP = 4;
const int STACK_RESIZE_IF_PUSH = 2;

// #if MODE == DEBUG
struct Var_Info
{
    const char * name_of_var;
    const char * name_of_file;
    const char * name_of_func;
    int number_of_line;
};
// #endif

struct Stack
{
    elem_t * data;
    size_t size;
    size_t capacity;
    int code_of_error; 
    Stack_Error Coda_blya;
    Var_Info var_info;
};


Stack_Error stack_Ctor(Stack *stk, size_t capacity, const char * name_of_var, const char * name_of_file, 
                                                                    const char * name_of_func, int number_of_line);

Stack_Error fillWithPoison(Stack *stk, size_t size, size_t capacity);
int isPoison(elem_t element);

Stack_Error stackPush(Stack *stk, elem_t element);
Stack_Error stackPop(Stack *stk, elem_t * element);
Stack_Error stackResize(Stack * stk, size_t new_capacity);

int returnStackError(Stack *stk);
const char * stackStrError(Stack *stk);

Stack_Error printStack(Stack *stk);
Stack_Error stackDump(Stack *stk, const char * name_of_file, 
                                                    const char * name_of_func, int number_of_line);