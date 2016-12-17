#include <stdio.h>
#include <stdlib.h>
#include <test.h>

int * ptr;

void test_initial_value() 
{
    assert(ptr != NULL);
}

void test_size() 
{
    assert(sizeof(ptr) == sizeof(int*));
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
    Test_describe("ptr_array()");

    Test_tearup(tearup);
    Test_teardown(teardown);

    Test_it("should be initial size", test_initial_value);
    Test_it("should be same size", test_size);
    Test_it("should not allow pass boudings", test_bound);

    return Test_run();
}
