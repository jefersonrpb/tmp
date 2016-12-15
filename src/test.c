#include <test.h>

static void* tearup = NULL;
static void* teardown = NULL;

static void it(char* label, void* fn);

void Test_assert(const int result, const char* const expression, 
                 const char * const func, const char * const file, const int line)  {
    if (result == 0) {
        // fail
    }
}

int Test_describe(char* label) {
    return 0;
}

int Test_it(char* label, void* fn) {
    return 0;
}

int Test_tearup(void* fn) {
    return 0;
}

int Test_teardown(void* fn) {
    return 0;
}

int Test_run() {
    return 0;
}
