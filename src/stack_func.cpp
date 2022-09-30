#include <stdio.h>
#include "../include/all_funcs.h"

extern FILE * err_file;

const elem_t POISON = -10.0;


Stack_Error fillWithPoison(Stack *stk, size_t size, size_t capacity)
{
    memset((elem_t*)((canary_t*) stk->data + 1) + size, POISON, capacity-size); //Хаха memset не пашет

    for (int counter = size; counter < capacity; counter++) 
        DATA_ACCESS(counter) = POISON;
        
    rehash(stk);
    ASSERT_OK(stk);
    return LOOK_AT_ERR_FILE_BLET;
}

int isPoison(elem_t element) // TODO bool
{
    return (element == POISON) ? (1) : (0);

}

Stack_Error stack_Ctor(Stack *stk, size_t capacity, const char * name_of_var, 
                                        const char * name_of_file, const char * name_of_func, int number_of_line)
{   
    assert(stk != NULL);

    stk->var_info.name_of_func = name_of_func;
    stk->var_info.name_of_file = name_of_file;
    stk->var_info.name_of_var  = name_of_var;
    stk->var_info.number_of_line  = number_of_line;

    stk->data = (elem_t *)calloc(1, capacity * sizeof(elem_t) + 2 * sizeof(BUF_CANARY));

    stk->code_of_error = 0;
    stk->capacity = capacity;
    stk->size = 0;    

    LEFT_BUF_CANARY  = BUF_CANARY;
    RIGHT_BUF_CANARY = BUF_CANARY;

    stk->left_canary = STRUCT_CANARY;
    stk->right_canary = STRUCT_CANARY;


    fillWithPoison(stk, stk->size, stk->capacity);

    rehash(stk);
    ASSERT_OK(stk);

    return LOOK_AT_ERR_FILE_BLET;

}

Stack_Error stackPush(Stack *stk, elem_t element)
{
   
    rehash(stk);
    ASSERT_OK(stk);
#ifdef DEBUG  
   ASSERTED(); 
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
    rehash(stk);
    ASSERT_OK(stk);

   return LOOK_AT_ERR_FILE_BLET;

}

Stack_Error stackPop(Stack *stk, elem_t * element) 
{
    rehash(stk);
    ASSERT_OK(stk);
    if (stk->size == 0)
    {
        fprintf(err_file,"You attained the end of stack\n");
        return STACK_ERROR_OVERSIZED;
        abort(); // TODO remove
    }

    if (stk->size * STACK_RESIZE_IF_POP < stk->capacity)
        stackResize(stk, stk->capacity);

    elem_t result = DATA_ACCESS(stk->size-1);

    DATA_ACCESS(--stk->size) = POISON;
    
    rehash(stk);
    ASSERT_OK(stk);

   return LOOK_AT_ERR_FILE_BLET;
}

Stack_Error stackResize(Stack * stk, size_t new_capacity)
{
    rehash(stk);
    ASSERT_OK(stk);

    if (new_capacity - stk->capacity > 0)
        size_t diff = new_capacity - stk->capacity;
    else
        return STACK_ERROR_SIZE_T_OVERFLOW;

    elem_t* tmp_ptr = (elem_t*)realloc(stk->data, new_capacity * sizeof(elem_t) + 2 * sizeof(BUF_CANARY));

    assert (tmp_ptr != NULL); // TODO use your hard assert

    stk->data = tmp_ptr;
    stk->capacity = new_capacity;

    fillWithPoison(stk, stk->size, stk->capacity);
    rehash(stk);
    ASSERT_OK(stk);

   return LOOK_AT_ERR_FILE_BLET;
}

Stack_Error stackDtor(Stack *stk)
{
    rehash(stk);
    ASSERT_OK(stk);

    fillWithPoison(stk, stk->size, stk->capacity);
    free((void *)stk->data);

    return HA_HA_HA_PODAVYS;
}

int checkLeftBufCanary(Stack *stk) // todo USE BOOL
{   
    return (LEFT_BUF_CANARY  == BUF_CANARY) ? (1) : (0);
}

int checkRightBufCanary(Stack *stk)
{
    return (RIGHT_BUF_CANARY == BUF_CANARY) ? (1) : (0);
}

int checkLeftStructCanary(Stack *stk)
{
    return (stk->left_canary == STRUCT_CANARY) ? (1) : (0);
}

int checkRightStructCanary(Stack *stk)
{
    return (stk->right_canary == STRUCT_CANARY) ? (1) : (0);
}

long hash(void * arr, size_t size)//не смотреть
{
    long hash = 0;

    char * arr_new = (char *)arr;
    long tmp = 0;

    for (size_t counter = 0; counter < size; counter++)
    {
        tmp = hash >> 5;
        hash |= ((long)arr_new[counter] << 3) & tmp; 
    }

    return hash;
}

Stack_Error rehash(Stack *stk)
{
    rehash(stk);
    ASSERT_OK(stk);

    stk->buf_hash = ((void *)stk->data, stk->size + 2);
    stk->hash     = hash((void *)stk, 9);

    return LOOK_AT_ERR_FILE_BLET;
}

//checkhash()

// int getSize

// int getCapacity

// int isEmpty

// int stackTop()//polchit znachenie nizhnego elementa