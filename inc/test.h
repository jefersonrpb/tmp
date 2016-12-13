#include <stdio.h>
#include <string.h>

#define ANSI_COLOR_RED   "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#undef assert

typedef struct __test_assertions_t {
    int count;
    int fails;
} __test_assertions;

typedef struct __test_item_t {
    char * label;
    void (*fn)(void);
} __test_item;

typedef struct __test_t {
    char* label;
    __test_item** items;
    __test_assertions assertions;
    int items_size;
    int items_length;
} __test;

static int __tests_length;
static int __tests_size = 1;
static __test ** __tests;
static __test_assertions __tests_curr_assert = {0,0};
static char * __tests_curr_assert_buff;

static int test_describe(char * label)
{
    if (__tests_length == __tests_size) {
        __tests_size += 1;
        __tests = realloc(__tests, sizeof(__tests) * __tests_size);
        if (!__tests) {
            printf("error on realloc tests\n");
            return 1;
        }
    }

    if (!__tests) {
        __tests = (__test**) malloc(sizeof(__test));
    }

    __test * test = (__test*) malloc(sizeof(__test));

    test->label = (char*) malloc(1024 * sizeof(char));

    test->assertions.count = 0;
    test->assertions.fails = 0;

    test->label = label;
    test->items_size = 10;
    test->items_length = 0;
    test->items = (__test_item**) malloc(sizeof(__test_item) * test->items_size);

    __tests[__tests_length++] = test;
    return 0;
}

static int __test_item_resize(__test * test, int size)
{
    __test_item** items = (__test_item**) realloc(test->items, sizeof(__test_item) * size);
    if (!items) {
        return 1;
    }
    test->items = items;
    test->items_size = size;
    return 0; 
}

static int test_it(char * label, void * fn) 
{
    __test_item * item = (__test_item*) malloc(sizeof(__test_item));
    item->label = malloc(sizeof(char) * 1024);
    item->fn = fn;
    item->label = label; 

    __test * test = __tests[__tests_length - 1];
    if (test->items_length == test->items_size) {
        int resize_error = __test_item_resize(test, test->items_size + 10);
        if (resize_error) {
            return resize_error;
        }
    }

    test->items[test->items_length++] = item;
    return 0;
}

static int test_run()
{
    if (__tests_length == 0) {
        return 0;
    } 

    __test * test = __tests[__tests_length-1];

    if (__tests_length == 1) {
        printf("\n");
    }

    printf(" %s\n\n", test->label);

    for (int i = 0; i < test->items_length; i++) {
        __test_item * item = test->items[i];
        
        __tests_curr_assert.count = 0;
        __tests_curr_assert.fails = 0;
        __tests_curr_assert_buff = malloc(sizeof(char) * 1024);

        (*item->fn)();

        test->assertions.count += __tests_curr_assert.count;
        test->assertions.fails += __tests_curr_assert.fails;

        // √ × ✓ ✖
        if (__tests_curr_assert.fails == 0) {
            printf("%s    √ %s%s\n",ANSI_COLOR_GREEN, ANSI_COLOR_RESET, item->label);
        } else {
            printf("%s    × %s%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, item->label);
            printf("%s", __tests_curr_assert_buff);
        }

    }

    printf("\n assertions: %d, fails: %d\n", test->assertions.count, 
            test->assertions.fails);

    if (__tests_length == __tests_size) {
        printf("\n");
    }

    return test->assertions.fails > 0;
}

static void __test_assert(const int result, const char* const expression, 
                        const char * const func, const char * const file, const int line)  {
    __tests_curr_assert.count += 1;
    if (result == 0) {
        __tests_curr_assert.fails += 1;
        char line_buf[12];
        sprintf(line_buf, "%d", line);
        strcat(__tests_curr_assert_buff, "\r         fail ");
        strcat(__tests_curr_assert_buff, expression);
        strcat(__tests_curr_assert_buff, " on ");
        strcat(__tests_curr_assert_buff, file);
        strcat(__tests_curr_assert_buff, ":");
        strcat(__tests_curr_assert_buff, line_buf);
        strcat(__tests_curr_assert_buff, "\n");
    }
}

#define assert(expression) \
    __test_assert((int)(expression), #expression, __func__, __FILE__, __LINE__);

#define test_tearup(fn) \
    printf("tearup()\n");

#define test_teardown(fn) \
    printf("teardown()\n");
