#include <test.h>
#include <int_array.h>

IntArray* array = NULL;

void test_empty() 
{
    ASSERT(array != NULL);
    ASSERT(array->length == 0);
}

void test_add_elements()
{
    int t1 = 10, t2 = 20, t3 = 30;

    int_array_push(array, t1);
    int_array_push(array, t2);
    int_array_push(array, t3);

    ASSERT(array->length == 3); 
    ASSERT(int_array_get(array, 0) == 10);
    ASSERT(int_array_get(array, 1) == t2);
    ASSERT(int_array_get(array, 2) == t3);
}

void test_access_boundings() 
{
    int t1 = 33;
    int_array_set(array, 5, t1);

    for (int i = 0; i < 5; i++) {
        ASSERT(!int_array_has(array, i));
    }

    ASSERT(int_array_has(array, 5));
    ASSERT(int_array_get(array, 5) == t1);
    ASSERT(!int_array_has(array, 200));
}

void test_check_position() 
{
    ASSERT(!int_array_has(array, 0));
    int_array_push(array, 1);
    ASSERT(int_array_has(array, 0));
}

void test_delete_position()
{
    int_array_push(array, 33);
    ASSERT(int_array_has(array, 0));
    int_array_delete(array, 0);
    ASSERT(!int_array_has(array, 0));
}

void test_delete_range()
{
    int t1[6];
    for (int i = 0; i < 6; i++) {
        t1[i] = i;
        int_array_push(array, t1[i]);
    }

    ASSERT(array->length == 6);

    int_array_splice(array, 2, 3);

    ASSERT(array->length == 3);

    ASSERT(int_array_get(array, 0) == t1[0]);
    ASSERT(int_array_get(array, 1) == t1[1]);
    ASSERT(int_array_get(array, 2) == t1[5]);
}

void test_clear()
{
    test_add_elements();
    int_array_clean(array);
    test_empty();
}

void setup()
{
    array = int_array_create(20);
}

void teardown()
{
    int_array_destroy(array);
}

int main(int argc, char* argv[]) 
{
    Test_describe("int_array()");

    Test_setup(setup);
    Test_teardown(teardown);

    Test_it("should start empty", test_empty);
    Test_it("should add elements", test_add_elements);
    Test_it("should not have access outside the boundings", test_access_boundings);
    Test_it("should check position", test_check_position);
    Test_it("should delete position", test_delete_position);
    Test_it("should delete range", test_delete_range);
    Test_it("should clear array", test_clear);

    return Test_run();
}
