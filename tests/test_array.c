#include "utest.h"

#include <common/array.h>

typedef array(int) array_int_t;

UTEST_MAIN();

UTEST(array_init, array_init_valid_data)
{
    const size_t INIT_SIZE = 16;

    array_int_t a;
    array_init(a, INIT_SIZE);

    ASSERT_GE(a.capacity, INIT_SIZE);
    ASSERT_EQ(a.size, 0);
    ASSERT_NE(a.data, NULL);

    array_reset(a);
}

UTEST(array_init, array_reserve_increase_capacity)
{
    const size_t RESERVE_SIZE = 16;

    array_int_t a;
    array_init(a, 0);

    array_reserve(a, RESERVE_SIZE);
    ASSERT_GE(a.capacity, RESERVE_SIZE);

    int *ptr = a.data;
    for (size_t i = 0; i < RESERVE_SIZE; ++i)
        array_add(a, 0xDE);
    ASSERT_EQ(ptr, a.data);
    array_reset(a);
}

UTEST(array_init, array_reserve_on_non_empty_array)
{
    array_int_t a;
    array_init(a, 0);

    size_t capacity = a.capacity;
    array_add(a, 0xDE);
    array_reserve(a, capacity);

    ASSERT_EQ(a.size, 1);
    ASSERT_GE(a.capacity, capacity + 1);
    array_reset(a);
}

UTEST(array_resize, resize_and_grow)
{
    array_int_t a;
    array_init(a, 0);

    const size_t new_size = 4;
    array_resize(a, new_size);

    ASSERT_EQ(a.size, new_size);
    for (size_t i = 0; i < a.size; ++i) {
        ASSERT_EQ(a.data[i], 0);
    }

    array_reset(a);
}

UTEST(array_resize, resize_to_a_smaller_size)
{
    array_int_t a;
    array_init(a, 0);

    for (int i = 0; i < 8; ++i)
        array_add(a, i);

    ASSERT_EQ(a.size, 8);
    array_resize(a, 4);

    ASSERT_EQ(a.size, 4);
    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(a.data[i], i);
    }

    array_reset(a);
}
