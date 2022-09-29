#include <stdio.h>
#include "../include/all_funcs.h"

extern FILE * err_file;

const elem_t POISON = -1.0;

Stack_Error fillWithPoison(Stack *stk, size_t size, size_t capacity)
{
    ASSERT_OK(stk);

    // memset(stk->data + size*sizeof(elem_t), POISON, capacity-size); //Хаха memset не пашет

    for (int i = size; i < capacity; i++) 
        stk->data[i] = POISON;

    ASSERT_OK(stk);
    return LOOK_AT_ERR_FILE_BLET;
}

int isPoison(elem_t element)
{
    return (element == POISON) ? (1) : (0);

}


Stack_Error stack_Ctor(Stack *stk, size_t capacity, const char * name_of_var, const char * name_of_file, const char * name_of_func, int number_of_line)
{   
    assert(stk != NULL);


    stk->var_info.name_of_func = name_of_func;
    stk->var_info.name_of_file = name_of_file;
    stk->var_info.name_of_var  = name_of_var;
    stk->var_info.number_of_line  = number_of_line;

    stk->code_of_error = 0;
    stk->capacity = capacity;
    stk->data = (elem_t *)calloc(capacity, sizeof(elem_t));
    stk->size = 0;

    fillWithPoison(stk, stk->size, stk->capacity);

    ASSERT_OK(stk);

    return LOOK_AT_ERR_FILE_BLET;

}

Stack_Error stackPush(Stack *stk, elem_t element)
{
   
    ASSERT_OK(stk);
#ifdef DEBUG  
   ASSERTED(); 
#endif

    if (stk->size + 1 > stk->capacity)
        stackResize(stk, stk->capacity * STACK_RESIZE_IF_PUSH); 

#ifdef DEBUG  
   ASSERTED(); 
#endif                              
    stk->data[stk->size] = element;
    stk->size++;

#ifdef DEBUG  
   ASSERTED(); 
#endif
    ASSERT_OK(stk);

   return LOOK_AT_ERR_FILE_BLET;

}

Stack_Error stackPop(Stack *stk, elem_t * element) 
{
    ASSERT_OK(stk);
    if (!stk->size)
    {
        fprintf(err_file,"You attained the end of stack\n");
        return STACK_ERROR_OVERSIZED;
    }

    if (stk->size * STACK_RESIZE_IF_POP < stk->capacity)
        stackResize(stk, stk->capacity);

    elem_t result = stk->data[stk->size-1];
    stk->data[--stk->size] = POISON;
    ASSERT_OK(stk);

   return LOOK_AT_ERR_FILE_BLET;
}


Stack_Error stackResize(Stack * stk, size_t new_capacity)
{
    ASSERT_OK(stk);

    if (new_capacity - stk->capacity > 0)
        size_t diff = new_capacity - stk->capacity;
    else
        return STACK_ERROR_SIZE_T_OVERFLOW;

    elem_t* tmp_ptr = (elem_t*)realloc(stk->data, new_capacity * sizeof(elem_t));

    assert (tmp_ptr != NULL); // TODO use your assert ХЗ

    stk->data = tmp_ptr;
    stk->capacity = new_capacity;

    fillWithPoison(stk, stk->size, stk->capacity);
    ASSERT_OK(stk);

   return LOOK_AT_ERR_FILE_BLET;
}


// if()
// fprintf(err_file, "Stack size is %d capacity is %d", stk->size, stk->capacity);
 

// int stackDtor
// int getSize

// int getCapacity


// int isEmpty

// int stackTop()//polchit znachenie nizhnego elementa

//poison

//gethash