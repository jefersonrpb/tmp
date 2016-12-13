#include <stdio.h>
#include <stdlib.h>
#include <test.h>

int * ptr;

int test_size() 
{
    assert(1 == 1);
    return sizeof(ptr) == 10 * sizeof(int);
}

int test_initial_value() 
{
    assert(1 == 2);
    assert(1 == 1);
    assert(1 == 5);
    return 0;
}

void test_bound()
{
    assert(1 == 1);
}

void tearup()
{
    ptr = malloc(10 * sizeof(int));
}

void teardown()
{
    free(ptr);
    ptr = NULL;
}

int main(int argc, char* argv[]) 
{
    test_describe("ptr_array()");

    test_tearup(tearup);
    test_teardown(teardown);

    test_it("should be same size", test_size);
    test_it("should be initial size", test_initial_value);
    test_it("should not allow pass boudings", test_bound);

    return test_run();
}
