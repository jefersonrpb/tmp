#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#undef assert

#define assert(expression) \
    Test_assert((int)(expression), #expression, __func__, __FILE__, __LINE__);

void Test_assert(const int result, const char* const expression, 
                 const char * const func, const char * const file, const int line);

int Test_describe(char* label);
int Test_it(char* label, void * fn);
int Test_tearup(void* fn);
int Test_teardown(void* fn);
int Test_run();

