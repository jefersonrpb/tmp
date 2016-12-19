#include <stdio.h>
#include <stdlib.h>
#include <test.h>
#include <ptr_array.h>

PtrArray* array;

void test_empty() 
{
    ASSERT(array != NULL);
    ASSERT(array->length == 0);
}

void test_add_elements_sequentially()
{
    int t1 = 10, t2 = 20, t3 = 30;

    ptr_array_add(array, &t1);
    ptr_array_add(array, &t2);
    ptr_array_add(array, &t3);

    ASSERT(array->length == 3); 
    ASSERT(*((int*) ptr_array_get(array, 0)) == 10);
    ASSERT(*((int*) ptr_array_get(array, 1)) == 20);
    ASSERT(*((int*) ptr_array_get(array, 2)) == 30);
}

void test_access_boundings() 
{
    ASSERT(ptr_array_get(array, 0) == NULL);
    ASSERT(ptr_array_get(array, 200) == NULL);
}

void test_check_position() 
{
    ASSERT(!ptr_array_has(array, 0));
    ptr_array_add(array, (void*) 1);
    ASSERT(ptr_array_has(array, 0));
}

void test_delete_position()
{
    ptr_array_add(array, (void*) 1);
    ptr_array_delete(array, 0);
    ASSERT(!ptr_array_has(array, 0));
}

void test_clear()
{
    test_add_elements_sequentially();
    ptr_array_clean(array);
    test_empty();
}

void tearup()
{
    array = ptr_array_new(1);
}

void teardown()
{
    ptr_array_free(array);
}

int main(int argc, char* argv[]) 
{
    Test_describe("ptr_array()");

    Test_tearup(tearup);
    Test_teardown(teardown);

    Test_it("should start empty", test_empty);
    Test_it("should add elements sequentially", test_add_elements_sequentially);
    Test_it("should not have access outside the boundings", test_access_boundings);
    Test_it("should check position", test_check_position);
    Test_it("should delete position", test_delete_position);
    Test_it("should clear array", test_clear);

    return Test_run();
}
