#include <stdio.h>
#include "../include/all_funcs.h"

extern FILE * err_file;

// TODO: use something that reads in hex
const elem_t POISON = -10.0;

// TODO: use statics!

Stack_Error fillWithPoison(Stack *stk, size_t size, size_t capacity)
{
    // memset((elem_t*)((canary_t*) stk->data + 1) + size, POISON, capacity-size); //Хаха memset не пашет

    for (int counter = size; counter < capacity; counter++) // TODO: Trying to beat memset, huh?)
        DATA_ACCESS(counter) = POISON;
        
    reHash(stk);
    ASSERT_OK(stk);
    return LOOK_AT_ERR_FILE_BLET;
}

bool isPoison(elem_t element) // TODO: do we need to detect this?
{
    return (element == POISON) ? (true) : (false);

}

Stack_Error stack_Ctor(Stack *stk, size_t capacity, const char * name_of_var, 
                                        const char * name_of_file, const char * name_of_func, int number_of_line)
{   
    HARD_ASSERT(stk != NULL);

    // TODO: learn about named designators!
    stk->var_info.name_of_func = name_of_func;
    stk->var_info.name_of_file = name_of_file;
    stk->var_info.name_of_var  = name_of_var;
    stk->var_info.number_of_line  = number_of_line;

    stk->data = (elem_t *)calloc(1, capacity * sizeof(elem_t) + 2 * sizeof(BUF_CANARY));
    // TODO: calloc may fail, check (also learn about perror, strerror)

    stk->code_of_error = 0;
    stk->capacity = capacity;
    stk->size = 0;    

    LEFT_BUF_CANARY  = BUF_CANARY;
    RIGHT_BUF_CANARY = BUF_CANARY;

    stk->left_canary = STRUCT_CANARY; // TODO: You can compute canary depending on stack's address
    stk->right_canary = STRUCT_CANARY; //      XOR is useful for that, think if you like it


    fillWithPoison(stk, stk->size, stk->capacity);

    reHash(stk);    // TODO: extract in function
    ASSERT_OK(stk);

    return LOOK_AT_ERR_FILE_BLET; // TODO: make "SUCCESS" for "SUCCESS"
                                  // TODO: also, no reason to always return same value
                                  //       report errors through error_code

}

// TODO: you can conditionally compile function calls, e.g.
// #ifdef DEBUG
// #define MY_FUNC() ...
// #else
// #define MY_FUNC() ((void) 0) // TODO: think why cast 0 to void
// #endif

Stack_Error stackPush(Stack *stk, elem_t element)
{
   
    reHash(stk);
    ASSERT_OK(stk);
#ifdef DEBUG   // TODO: extract, in general try to conditionally compile function calls
   ASSERTED(); //       not function's code, it's much clearer this way
#endif

    if (stk->size + 1 > stk->capacity)
        stackResize(stk, stk->capacity * STACK_RESIZE_IF_PUSH); 

#ifdef DEBUG  
   ASSERTED(); 
#endif                      
    DATA_ACCESS(stk->size) = element;
    stk->size++;

#ifdef DEBUG  
   ASSERTED(); 
#endif
    reHash(stk);
    ASSERT_OK(stk);

   return LOOK_AT_ERR_FILE_BLET; // TODO: same thing

}

Stack_Error stackPop(Stack *stk, elem_t * element) 
{
    reHash(stk);
    ASSERT_OK(stk);
    if (stk->size == 0)
    {
        fprintf(err_file,"You attained the end of stack\n");
        return STACK_ERROR_OVERSIZED;
    }

    if (stk->size * STACK_RESIZE_IF_POP < stk->capacity)
        stackResize(stk, stk->capacity);

    elem_t result = DATA_ACCESS(stk->size-1);

    DATA_ACCESS(--stk->size) = POISON;
    
    reHash(stk);
    ASSERT_OK(stk);
// ^ TODO: misaligned

   return LOOK_AT_ERR_FILE_BLET;
}

Stack_Error stackResize(Stack * stk, size_t new_capacity)
{
    reHash(stk);
    ASSERT_OK(stk);

    if (new_capacity - stk->capacity > 0)
        size_t diff = new_capacity - stk->capacity; // TODO: what is this? Useless var
    else
        return STACK_ERROR_SIZE_T_OVERFLOW;

    elem_t* tmp_ptr = (elem_t*)realloc(stk->data, new_capacity * sizeof(elem_t) + 2 * sizeof(BUF_CANARY));

    // TODO: realloc may fail, check!

    reHash(stk);
    HARD_ASSERT(tmp_ptr != NULL);

    stk->data = tmp_ptr;
    stk->capacity = new_capacity;

    fillWithPoison(stk, stk->size, stk->capacity);
    reHash(stk);
    ASSERT_OK(stk);

   return LOOK_AT_ERR_FILE_BLET;
}

Stack_Error stackDtor(Stack *stk)
{
    reHash(stk);
    ASSERT_OK(stk);

    fillWithPoison(stk, stk->size, stk->capacity);
    free((void *)stk->data); // TODO: maybe transition stk to some kind of "zombie" state

    return HA_HA_HA_PODAVYS;
}

bool isLeftBufCanary(Stack *stk)
{   
    return (LEFT_BUF_CANARY  == BUF_CANARY) ? (true) : (false);
    // TODO:                                ^~~~~~~~~~~~~~~~~~ Why?!
}

bool isRightBufCanary(Stack *stk)
{
    return (RIGHT_BUF_CANARY == BUF_CANARY) ? (true) : (false);
}

bool isLeftStructCanary(Stack *stk)
{
    return (stk->left_canary == STRUCT_CANARY) ? (true) : (false);
}

bool isRightStructCanary(Stack *stk)
{
    return (stk->right_canary == STRUCT_CANARY) ? (true) : (false);
}

// Stack_Error printElement(double element, int index)
// {
//     fprintf(err_file, "\t*[%s] = \n", index, element); 
    
//     return LOOK_AT_ERR_FILE_BLET;

// }

// Stack_Error printElement(int element, int index)
// {
//     fprintf(err_file, "\t*[%s] = \n", index, element); 
    
//     return LOOK_AT_ERR_FILE_BLET;

// }

// Stack_Error printElement(char element, int index)
// {
//     fprintf(err_file, "\t*[%lx] = \n", index, element); 

//     return LOOK_AT_ERR_FILE_BLET;

// }

// Stack_Error printElement(char * element, int index)
// {
//     fprintf(err_file, "\t*[%s] = \n", index, element); 
    
//     return LOOK_AT_ERR_FILE_BLET;

// }

Stack_Error printElement(elem_t element, int index)
{
    fprintf(err_file, "\t*[%lx] = \n", index, element); 

    return LOOK_AT_ERR_FILE_BLET;

}

long hash(void * arr, size_t size)//sizeof(элемента arr)
{
    long hash = 0;

    char * buffer = (char *)arr;
    long hashvalue = 0;

    for (size_t i = 0; buffer[i] != 0; i++)
    {
        hashvalue = buffer[i] + 31 * hashvalue;
    }

    // TODO: can you make a better hash? So hash(zero_filled, size) != 0
    return hashvalue % size;
}

Stack_Error reHash(Stack *stk)
{
    // reHash(stk);
    ASSERT_OK(stk);

    stk->buf_hash = ((void *)stk->data, stk->size + 2);
    // TODO:                                        ^ magic number
    // TODO: I think you can hash whole buffer (with uninitialized elements included)
    stk->hash     = hash((void *)stk, 9);
    // TODO:                          ^ magic number

    return LOOK_AT_ERR_FILE_BLET;
}

//checkhash()

// int getSize

// int getCapacity

// int isEmpty

// int stackTop()//polchit znachenie nizhnego elementa