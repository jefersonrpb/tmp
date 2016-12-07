
#include <stdio.h>

void mock_assert(const int result, const char* const expression, 
                        const char * const file, const int line)  {
    if (result > 0) return;
    printf("[ fail ]: '%s' %s:%d\n", expression, file, line);
}
#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);

