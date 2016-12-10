#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
    TEST_DESCRIBE("ptr_array()");

    TEST_TEARUP(tearup);
    TEST_TEARDOWN(teardown);

    TEST_IT(test_size);
    TEST_IT(test_initial_value);
    TEST_IT(test_bound);

    return TEST_RUN();
}


