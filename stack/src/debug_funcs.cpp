#include <stdio.h>
#include "../include/all_funcs.h"

extern FILE * err_file; // TODO: think if you can avoid global variable

int returnStackError(Stack *stk)
{

   stk->code_of_error |= CHECK(!stk->data, STACK_ERROR_NULL);

   stk->code_of_error |= CHECK((stk->size < 0), STACK_ERROR_SIZE_BELOW_ZERO);

   stk->code_of_error |= CHECK((stk->capacity < 0), STACK_ERROR_CAPACITY_BELOW_ZERO);

   stk->code_of_error |= CHECK((stk->size > stk->capacity), STACK_ERROR_OVERSIZED);

   stk->code_of_error |= CHECK((!isLeftBufCanary(stk)), STACK_ERROR_LEFTBUF_CANARY_DIED);

   stk->code_of_error |= CHECK((!isRightBufCanary(stk)), STACK_ERROR_RIGHTBUF_CANARY_DIED);

   stk->code_of_error |= CHECK((!isLeftStructCanary(stk)), STACK_ERROR_LEFTSTRUCT_CANARY_DIED);

   stk->code_of_error |= CHECK((!isRightStructCanary(stk)), STACK_ERROR_RIGHTSTRUCT_CANARY_DIED);

   return stk->code_of_error;

}

//todo documentation

const char * stackStrError(Stack *stk)
{

    char * result = (char *)calloc(100, sizeof(char));

    if (stk->code_of_error & STACK_ERROR_NULL)
        strcat(result, "ERROR: Data pointer = NULL\n");

    if (stk->code_of_error & STACK_ERROR_SIZE_BELOW_ZERO)
        strcat(result, "ERROR: Size < 0\n");

    if (stk->code_of_error & STACK_ERROR_CAPACITY_BELOW_ZERO)
        strcat(result, "ERROR: Capacity < 0\n");

    if (stk->code_of_error & STACK_ERROR_OVERSIZED)
        strcat(result, "ERROR: Size > capacity\n");

    if (stk->code_of_error & STACK_ERROR_LEFTBUF_CANARY_DIED)
        strcat(result, "ERROR: LEFT BUF CANARY IS DEAD\n");

    if (stk->code_of_error & STACK_ERROR_RIGHTBUF_CANARY_DIED)
        strcat(result, "ERROR: RIGHT BUF CANARY IS DEAD\n");
   
   if (stk->code_of_error & STACK_ERROR_LEFTSTRUCT_CANARY_DIED)
      strcat(result, "ERROR: RIGHT STRUCT CANARY IS DEAD\n");

   if (stk->code_of_error & STACK_ERROR_RIGHTSTRUCT_CANARY_DIED)
      strcat(result, "ERROR: RIGHT STRUCT CANARY IS DEAD\n");

    if(stk->code_of_error == 0)
     {
        strcat(result,"(ok)");        
     }

    return result;

}

Stack_Error printStack(Stack *stk)
{
   fprintf(err_file, "{\n" 
                     "size = %d\n"
                     "capacity = %d\n"
                     "data[%p]\n"
                     "code_of_error = %d\n"
                     "buf hash      = %ld\n"
                     "hash          = %d\n"
                     "buf    canaries: %ld %ld\n"
                     "struct canaries: %ld %ld\n"
                     "\t}\n",
                  stk->size, stk->capacity, &DATA_ACCESS(0),
                  stk->code_of_error,
                  stk->buf_hash, stk->hash, 
                  LEFT_BUF_CANARY, RIGHT_BUF_CANARY,
                  stk->left_canary, stk->right_canary); // TODO: split in many printfs

   int counter = stk->capacity;
   while(counter--)
   {
      // TODO: mark edge of filled with elements block (> size, < capaicty)

      // TODO: no need to detect poison with print
      //       hex speak would speak for itself *wink wink*
      if (isPoison(DATA_ACCESS(counter)))
         fprintf(err_file, "\t[%d] = NAN(POISON)\n", counter);
      else
         printElement(DATA_ACCESS(counter), counter);
   }

   fprintf(err_file, "\t}\n}\n");
   
   return LOOK_AT_ERR_FILE_BLET;
}

Stack_Error stackDump(Stack *stk, const char * name_of_file, const char * name_of_func, 
                                                                        int number_of_line)
{

   fprintf(err_file, "%s at %s(%d)\n", name_of_func, name_of_file, number_of_line);
#ifdef DEBUG  
   ASSERTED(); 
#endif

#if ERR_FILE == LOG
   fprintf(err_file, "%s", stackStrError(stk));
#endif

// #if ERR_FILE == STDERR
//    fprintf(err_file, "\e[0;31m%s\e[0m", stackStrError(stk));
// #endif
   
   fprintf(err_file, "CANARIES STATUS IN BUFFER: %d %d\n", isLeftBufCanary(stk), isRightBufCanary(stk));
   fprintf(err_file, "CANARIES STATUS IN STRUCT: %d %d\n", isLeftStructCanary(stk), isRightStructCanary(stk));

#ifdef DEBUG  
   ASSERTED(); 
#endif
    fprintf(err_file, "Stack[%p] ""%s"" at %s at %s(%d)\n", 
            stk,
            stk->var_info.name_of_var, stk->var_info.name_of_func, 
            stk->var_info.name_of_file, stk->var_info.number_of_line);
    printStack(stk);
#ifdef DEBUG  
   ASSERTED(); 
#endif

   return LOOK_AT_ERR_FILE_BLET;
}