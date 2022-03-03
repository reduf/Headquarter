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
}
