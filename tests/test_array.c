#include "utest.h"

#include <stdint.h>
#include <common/array.h>

typedef array(int) array_int_t;

UTEST_MAIN();

UTEST(array_init, array_init_initialize_all_fields)
{
    array_int_t a;
    array_init(&a);

    ASSERT_EQ(a.capacity, 0);
    ASSERT_EQ(a.size, 0);
    ASSERT_EQ(a.data, NULL);

    array_reset(&a);
}

UTEST(array_reserve, reserve_increase_capacity)
{
    const size_t RESERVE_SIZE = 16;

    array_int_t a;
    array_init(&a);

    array_reserve(&a, RESERVE_SIZE);
    ASSERT_GE(a.capacity, RESERVE_SIZE);

    int *ptr = a.data;
    for (size_t i = 0; i < RESERVE_SIZE; ++i)
        array_add(&a, 0xDE);
    ASSERT_EQ(ptr, a.data);
    array_reset(&a);
}

UTEST(array_reserve, reserve_on_non_empty_array)
{
    array_int_t a;
    array_init(&a);

    size_t capacity = a.capacity;
    array_add(&a, 0xDE);
    array_reserve(&a, capacity);

    ASSERT_EQ(a.size, 1);
    ASSERT_GE(a.capacity, capacity + 1);
    array_reset(&a);
}

UTEST(array_resize, resize_and_grow)
{
    array_int_t a;
    array_init(&a);

    const size_t new_size = 4;
    array_resize(&a, new_size);

    ASSERT_EQ(a.size, new_size);
    for (size_t i = 0; i < a.size; ++i) {
        ASSERT_EQ(a.data[i], 0);
    }

    array_reset(&a);
}

UTEST(array_resize, resize_to_a_smaller_size)
{
    array_int_t a;
    array_init(&a);

    for (int i = 0; i < 8; ++i)
        array_add(&a, i);

    ASSERT_EQ(a.size, 8);
    array_resize(&a, 4);

    ASSERT_EQ(a.size, 4);
    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(a.data[i], i);
    }

    array_reset(&a);
}

UTEST(array_resize, resize_init_to_0)
{
    array_int_t a;
    array_init(&a);

    array_resize(&a, 4);
    for (size_t i = 0; i < a.size; ++i) {
        ASSERT_EQ(a.data[i], 0);
    }

    array_reset(&a);
}

UTEST(array_remove, remove_first_element)
{
    array_int_t a;
    array_init(&a);

    for (int i = 0; i < 3; ++i)
        array_add(&a, i);
    array_remove(&a, 0);

    ASSERT_EQ(a.size, 2);
    array_reset(&a);
}

UTEST(array_remove, remove_last_element)
{
    array_int_t a;
    array_init(&a);

    for (int i = 0; i < 3; ++i)
        array_add(&a, i);
    array_remove(&a, a.size - 1);

    ASSERT_EQ(a.size, 2);
    array_reset(&a);
}

UTEST(array_remove, remove_middle_element)
{
    array_int_t a;
    array_init(&a);

    for (int i = 0; i < 3; ++i)
        array_add(&a, i);
    array_remove(&a, 1);

    ASSERT_EQ(a.size, 2);
    array_reset(&a);
}

UTEST(array_remove_ordered, remove_first_element)
{
    array_int_t a;
    array_init(&a);

    for (int i = 0; i < 3; ++i)
        array_add(&a, i);
    array_remove_ordered(&a, 0);

    ASSERT_EQ(a.size, 2);
    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(a.data[i], i + 1);
    }

    array_reset(&a);
}

UTEST(array_remove_ordered, remove_last_element)
{
    array_int_t a;
    array_init(&a);

    for (int i = 0; i < 3; ++i)
        array_add(&a, i);
    array_remove_ordered(&a, a.size - 1);

    ASSERT_EQ(a.size, 2);
    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(a.data[i], i);
    }

    array_reset(&a);
}

UTEST(array_remove_ordered, remove_middle_element)
{
    array_int_t a;
    array_init(&a);

    for (int i = 0; i < 3; ++i)
        array_add(&a, i);
    array_remove_ordered(&a, 1);

    ASSERT_EQ(a.size, 2);
    ASSERT_EQ(a.data[0], 0);
    ASSERT_EQ(a.data[1], 2);

    array_reset(&a);
}

UTEST(array_push, array_push_10_times)
{
    const int ARRAY_SIZE = 10;

    array_int_t a;
    array_init(&a);

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        int *ptr = array_push(&a, 1);
        *ptr = i;
    }

    ASSERT_EQ(a.size, ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        ASSERT_EQ(a.data[i], i);
    }

    array_reset(&a);
}
