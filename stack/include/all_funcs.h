#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>

// TODO: extract functions from here in other headers!

#define ASSERTED() fprintf(stderr, "Succeeded file: %s func: %s line: %d\n",          \
                           __FILE__, __FUNCTION__, __LINE__)

// TODO: extract in function (also, wrap your macros in do...while)
#define HARD_ASSERT(condition) if(!condition)                               \
                                {                                           \
                                    fputs("CRITICAL ERROR\n", stderr);      \
                                    abort();                                \
                                }             

// TODO: same thing
#define ASSERT_OK(stkPtr)  if (returnStackError(stkPtr) != 0)                        \
                                stackDump(stkPtr, __FILE__, __FUNCTION__, __LINE__);

// TODO: why is this a macro? Check is way too generic.
#define CHECK(condition, code_of_error)  (condition) ? code_of_error : STACK_IS_CORRECT;

// TODO: consider using same naming style for all macros
#define stackCtor(stk, capacity) stack_Ctor(&stk, capacity, #stk,                     \
                                            __FILE__, __FUNCTION__,__LINE__)

// TODO: move most of this macros out of header
// TODO: undef local macros after use

// TODO: READ ABOUT STAGES OF COMPILATION, LEARN TO USE COMPILER BY HAND!

#ifdef CANARY_PROT
#define ON_CANARY_PROT() ....
#else
#define ON_CANARY_PROT()
#endif

// #define DEBUG 
// #define MODE DEEP_DEBUG

// TODO: why macro? You can define this with compiler flag "-D"
#define ERR_FILE LOG

// TODO: reduce number of sizeofs (they are "automatic" if types match
// TODO: why is this a macro? Make it into function!
// TODO: reconsider naming, use verbs, imperative, and make it clear
#define DATA_ACCESS(i)    *(elem_t *)((char *)stk->data + (i)*sizeof(elem_t) + 1*sizeof(canary_t))

// TODO: same
#define LEFT_BUF_CANARY  *(canary_t *)((char *)stk->data)

// TODO: thing
#define RIGHT_BUF_CANARY *(canary_t *)((char *)stk->data + stk->capacity * sizeof(elem_t) + sizeof(canary_t))
; // TODO: useful semicolon :)
typedef unsigned long long canary_t;
typedef double elem_t;

enum Stack_Error 
{
    STACK_ERROR_NULL                     = 1 << 0,
    STACK_ERROR_SIZE_BELOW_ZERO          = 1 << 1,
    STACK_ERROR_CAPACITY_BELOW_ZERO      = 1 << 2,
    STACK_ERROR_OVERSIZED                = 1 << 3,
    STACK_ERROR_SIZE_T_OVERFLOW          = 1 << 4,
    STACK_ERROR_LEFTSTRUCT_CANARY_DIED   = 1 << 5,
    STACK_ERROR_RIGHTSTRUCT_CANARY_DIED  = 1 << 6,
    STACK_ERROR_LEFTBUF_CANARY_DIED      = 1 << 7,
    STACK_ERROR_RIGHTBUF_CANARY_DIED     = 1 << 8,
    STACK_ERROR_INVALID_POINTER          = 1 << 9,
    STACK_ERROR_POINTER_IS_NULL          = 1 << 10,

    // DOUBLE_DTOR_INIT
    // DOUBLE_CTOR_INI
    //WRONG HASH
    //WRONG STRUCT HASH
    STACK_IS_CORRECT                     = 0,
    LOOK_AT_ERR_FILE_BLET                = 0,
    HA_HA_HA_PODAVYS                     = 0
};

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
    canary_t right_canary;
    canary_t left_canary;  

    elem_t * data;
    size_t size;
    size_t capacity;
    int code_of_error; 
    
    long hash;
    long buf_hash;

    Var_Info var_info;

};

const int STACK_RESIZE_IF_POP = 4;
const int STACK_RESIZE_IF_PUSH = 2;

const canary_t STRUCT_CANARY = 0xDEADDEAD;
const canary_t BUF_CANARY    = 0xBADDED32;

// Stack_Error printElement(double element, int index);
// Stack_Error printElement(int element, int index);
// Stack_Error printElement(char element, int index);
// Stack_Error printElement(char * element, int index);
Stack_Error printElement(elem_t element, int index);// пользователь должен написать самостоятельно

bool isLeftBufCanary(Stack *stk);
bool isRightBufCanary(Stack *stk);
bool isLeftStructCanary(Stack *stk);
bool isRightStructCanary(Stack *stk);

Stack_Error stack_Ctor(Stack *stk, size_t capacity, const char * name_of_var, const char * name_of_file, 
                                                                    const char * name_of_func, int number_of_line);
Stack_Error stackDtor(Stack *stk);

Stack_Error fillWithPoison(Stack *stk, size_t size, size_t capacity);
bool isPoison(elem_t element);

Stack_Error stackPush(Stack *stk, elem_t element);
Stack_Error stackPop(Stack *stk, elem_t * element);
Stack_Error stackResize(Stack * stk, size_t new_capacity);

int returnStackError(Stack *stk);
const char * stackStrError(Stack *stk);

Stack_Error printStack(Stack *stk);
Stack_Error stackDump(Stack *stk, const char * name_of_file, 
                                                    const char * name_of_func, int number_of_line);
Stack_Error reHash(Stack *stk);//what does it?
long Hash(void * arr, size_t size);

// TODO: use consistent naming!