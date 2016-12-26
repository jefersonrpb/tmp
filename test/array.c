#include <test.h>
#include <array.h>

void** array = NULL;
int array_length = 0;
int array_size = 20;

void test_add_elements_sequentially()
{
    int t1 = 10, t2 = 20, t3 = 30;

    array_push(array, &t1, &array_length, &array_size);
    array_push(array, &t2, &array_length, &array_size); 
    array_push(array, &t3, &array_length, &array_size);

    ASSERT(array_length == 3); 
    ASSERT(*((int*) array_get(array, 0, array_length)) == 10);
    ASSERT(*((int*) array_get(array, 1, array_length)) == t2);
    ASSERT(array_get(array, 2, array_length) == &t3);
}

void test_access_boundings() 
{
    int t1 = 33;
    array_set(array, 5, &t1, &array_length, &array_size);

    ASSERT(!array_has(array, 1, array_length));
    ASSERT(array_get(array, 1, array_length) == NULL);
    for (int i = 0; i < 5; i++) {
        ASSERT(!array_has(array, i, array_length));
        ASSERT(array_get(array, i, array_length) == NULL);
    }

    ASSERT(array_has(array, 5, array_length));
    ASSERT(!array_has(array, 200, array_length));

    ASSERT(array_get(array, 5, array_length) == &t1);
    ASSERT(array_get(array, 200, array_length) == NULL);
}

void test_check_position() 
{
    ASSERT(!array_has(array, 0, array_length));
    array_push(array, (void*) 1, &array_length, &array_size);
    ASSERT(array_has(array, 0, array_length));
}

void test_delete_position()
{
    array_push(array, (void *) 33, &array_length, &array_size);
    ASSERT(array_has(array, 0, array_length));
    array_delete(array, 0, &array_length);
    ASSERT(!array_has(array, 0, array_length));
}

void test_delete_range()
{
    int t1[6];
    for (int i = 0; i < 6; i++) {
        t1[i] = i;
        array_push(array, &t1[i], &array_length, &array_size);
    }

    ASSERT(array_length == 6);

    array_splice(array, 2, 3, &array_length);

    ASSERT(array_length == 3);

    ASSERT(array_get(array, 0, array_length) == &t1[0]);
    ASSERT(array_get(array, 1, array_length) == &t1[1]);
    ASSERT(array_get(array, 2, array_length) == &t1[5]);
}

void setup()
{
    array = array_new(array_size);
}

void teardown()
{
    array_free(array);
    array_length = 0;
    array_size = 20;
}

int main(int argc, char* argv[]) 
{
    Test_describe("array()");

    Test_setup(setup);
    Test_teardown(teardown);

    Test_it("should add elements sequentially", test_add_elements_sequentially);
    Test_it("should not have access outside the boundings", test_access_boundings);
    Test_it("should check position", test_check_position);
    Test_it("should delete position", test_delete_position);
    Test_it("should delete range", test_delete_range);

    return Test_run();
}
