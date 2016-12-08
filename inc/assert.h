
#include <stdio.h>

void mock_assert(const int result, const char* const expression, 
                        const char * const file, const int line)  {
    if (result > 0) return;
    printf(" ✓ : '%s' %s:%d\n", expression, file, line);
}
#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);

#define SAFE_BLOCK(block) do {\
	block \
} while(0)

#define describe(describe_text, describe_block) SAFE_BLOCK(\
    printf("describe: %s\n", (char *) describe_text); \
    (describe_block); \
)

#define it(it_text, it_block) SAFE_BLOCK(\
    printf(" -- it: %s\n", (char *) it_text); \
    (it_block); \
)
