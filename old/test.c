#include <stdio.h>

extern void mock_assert(const int result, const char* const expression, 
                        const char * const file, const int line)  {
    if (result > 0) return;
    printf("assert: '%s' %s:%d\n", expression, file, line);
}
#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);

// A test case that does nothing and succeeds.
void null_test_success(void **state) {
}

int main(int argc, char* argv[]) {

    char* array[30] = {
        [0] = "hello",
        [1] = "world"
    };


    size_t i;
    for (i = 0; i < 2; i++) {
        printf("#%li: %s\n", i, array[i]);
    }

    if (array[2] == NULL) {
        printf("#2: null\n");
    }
    
    printf("#3: assert(NULL)\n");
    assert(array[3] != NULL);
    assert(array[4] == NULL);

    return 0;
}
