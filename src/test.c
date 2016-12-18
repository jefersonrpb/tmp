#include <test.h>

#define ERROR_ASSERT_FAIL 1
#define ERROR_MALLOC 2
#define ERROR_REALLOC 3
#define ANSI_COLOR_RED   "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct {
    int total;
    int failed;
} AssertionCount;

typedef struct {
    char* label;
    void (*fn)(void);
} TestItem;

typedef struct {
    char* label;
    TestItem** items;
    AssertionCount* assertion;
    int items_size;
    int items_length;
} Test;

typedef struct {
    int tests_length;
    int tests_size;
    Test** tests;
    AssertionCount* assertion;
    char buf[10240];
} Globals;

static void (*tearup)(void) = NULL;
static void (*teardown)(void) = NULL;
static Globals* _globals = NULL; 

static AssertionCount* _assertion_new();

static void _fatal(int code) {

    switch (code) {
        case ERROR_REALLOC:
            printf("Error on reallocate memory\n");
        break;
        case ERROR_MALLOC:
            printf("Error on allocate memory\n");
        break;
        default: 
            printf("Unknow error\n");
    }

    exit(code);
}

static void _globals_init() {

    _globals = malloc(sizeof(Globals));

    if (_globals == NULL) {
        _fatal(ERROR_MALLOC);
    } 

    _globals->tests_length = 0;
    _globals->tests_size = 1;
    _globals->assertion = _assertion_new();

    // alloc space for one test
    _globals->tests = malloc(sizeof(Test));
}

static void _test_item_resize(Test* test, int size) {
    TestItem** items = realloc(test->items, sizeof(TestItem) * size); 
    if (items == NULL) {
        _fatal(ERROR_REALLOC);
    }
    test->items = items;
    test->items_size = size;
}

static void _test_add(Test* test) {
    
    if (_globals->tests_length == _globals->tests_size) {
        _globals->tests_size++;
        _globals->tests = realloc(_globals->tests, sizeof(Test) * _globals->tests_size);
        if (_globals->tests == NULL) {
            _fatal(ERROR_REALLOC);
        }
    }

    _globals->tests[_globals->tests_length++] = test;
}

static void _test_add_item(TestItem* item) {

    Test* test = _globals->tests[_globals->tests_length - 1];

    if (test->items_length == test->items_size) {
        _test_item_resize(test, test->items_size + 10);
    }

    test->items[test->items_length++] = item;
}

static AssertionCount* _assertion_new() {
    AssertionCount* assertion = malloc(sizeof(AssertionCount));
    assertion->total = 0;
    assertion->failed = 0; 
    return assertion;
}

static TestItem* _item_new(char* label, void* fn) {

    TestItem* item = malloc(sizeof(TestItem));
    item->label = malloc(sizeof(char) * strlen(label));
    item->fn = fn;
    strcpy(item->label, label);
    
    return item;
}

static Test* _test_new(char* label) {

    if (_globals == NULL) {
        _globals_init();
    }

    Test* test = malloc(sizeof(Test));
    test->items_length = 0;
    test->items_size = 10;
    test->items = malloc(sizeof(TestItem) * test->items_size);
    test->assertion = _assertion_new();
    test->label = malloc(sizeof(char) * strlen(label));
    strcpy(test->label, label);

    return test; 
} 

static int _test_run(int test_index) {

    Test* test = _globals->tests[test_index];
    printf(" %s\n\n", test->label);

    for (int item_index = 0; item_index < test->items_length; item_index++) {

        TestItem* item = test->items[item_index];

        _globals->assertion->total = 0;
        _globals->assertion->failed = 0;

        if (tearup) {
            (*tearup)();
        }

        (*item->fn)(); 

        if (teardown) {
            (*teardown)();
        }

        test->assertion->total += _globals->assertion->total;
        test->assertion->failed += _globals->assertion->failed;

        // √ × ✓ ✖
        if (_globals->assertion->failed == 0) {
            printf("%s    √ %s%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET, item->label);
        } else {
            printf("%s    × %s%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, item->label);
            printf("%s", _globals->buf);
        }
    }

    printf("\n assertions: %d, failures: %d\n", test->assertion->total, 
            test->assertion->failed);

    return test->assertion->failed;
}

static void _test_clean(int index) {

    Test* test = _globals->tests[index];

    free(test->label);
    free(test->items);
    free(test->assertion);
    free(test);

    _globals->tests[index] = NULL; 
}

static void _globals_clean() {
    for (int index = 0; index < _globals->tests_length; index++) {
        _test_clean(index);
    }
    free(_globals->tests);
    free(_globals->assertion);
    _globals = NULL;
}

void Test_assert(const int result, const char* const expression, 
                 const char * const func, const char * const file, const int line)  {
    _globals->assertion->total++;
    if (result == 0) {
        _globals->assertion->failed++;
        char buf[1024];
        sprintf(buf, "\r         fail %s on %s:%d\n", expression, file, line);
        strcat(_globals->buf, buf);
    }
}

int Test_describe(char* label) {

    Test* test = _test_new(label);
    _test_add(test);
    return 0;
}

int Test_it(char* label, void* fn) {

    TestItem* item = _item_new(label, fn);
    _test_add_item(item);
    return 0;
}

int Test_tearup(void* fn) {
    tearup = fn;
    return 0;
}

int Test_teardown(void* fn) {
    teardown = fn;
    return 0;
}

int Test_run() {

    if (_globals->tests_length == 0 ) {
        return 0;
    }
    
    int failed = 0;

    for (int index = 0; index < _globals->tests_length; index++) {
        if (index <= 1) {
            printf("\n");
        }
        failed += _test_run(index);
    }

    _globals_clean();

    printf("\n");

    return failed > 0 ? ERROR_ASSERT_FAIL : 0;
}
