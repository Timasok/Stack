#include <stdio.h>
#include <stdlib.h>
#include "../include/all_funcs.h"

#ifdef ERR_FILE // TODO: Why?
FILE *err_file = fopen("err_file.txt","w");
#endif
#ifndef ERR_FILE
FILE *err_file = stderr;
#endif

int main()
{

    setvbuf(err_file, NULL, _IONBF, 0);  // TODO: extract log file configuration
    Stack stk1 = {};
#ifdef DEBUG  
   ASSERTED(); 
#endif
    stackCtor(stk1, 8);
    // TODO:        ^ same thing about defaults as below
#ifdef DEBUG  
   ASSERTED(); 
#endif
    ASSERT_OK(&stk1);
    // printStack(&stk1);
#ifdef DEBUG  
   ASSERTED(); 
#endif
    stackPush(&stk1, 2.6);
    // TODO: can you ^~~ make a default? Or at least make this in a const
#ifdef DEBUG  
   ASSERTED(); 
#endif
    printStack(&stk1);

    elem_t value = 0.0;  
    stackPop(&stk1, &value);

    printStack(&stk1);
    stackDtor(&stk1);

    fclose(err_file);       

    return 1; // TODO: Use constants like EXIT_FAILURE?
}

// #ifdef DEBUG  
//    ASSERTED(); 
// #endif




// #ifdef DEBUG  
//    ASSERTED(); 
// #endif
//     printStack(&stk1);
// #ifdef DEBUG  
//    ASSERTED(); 
// #endif


// #ifdef DEBUG  
//    ASSERTED(); 
// #endif

// #ifdef DEBUG  
//    ASSERTED(); 
// #endif
//     printStack(&stk1);
// #ifdef DEBUG  
//    ASSERTED(); 
// #endif
//     stackPush(&stk1, 5);
// #ifdef DEBUG  
//    ASSERTED(); 
// #endif    
//     printStack(&stk1);
// #ifdef DEBUG  
//    ASSERTED(); 
// #endif
//     stackPush(&stk1, 5);
// #ifdef DEBUG  
//    ASSERTED(); 
// #endif
//     printStack(&stk1);

//     stackPush(&stk1, 5);
// #ifdef DEBUG  
//    ASSERTED(); 
// #endif
//     printStack(&stk1);
// #ifdef DEBUG  
//    ASSERTED(); 
// #endif
//     elem_t value = 0.0;
// #ifdef DEBUG  
//    ASSERTED(); 
// #endif

