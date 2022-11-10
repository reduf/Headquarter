#include <assert.h>
#include <stdint.h>

#include "utest.h"

#include <common/macro.h>
#include "kstr.h"
#include "kstr.c"

static uint16_t test_value[32] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
};

const size_t test_value_len = ARRAY_SIZE(test_value);

UTEST_MAIN();

UTEST(kstr_copy, init_works)
{
    DECLARE_KSTR(dest1, 32);
    DECLARE_KSTR(dest2, 64);

    struct kstr src;
    kstr_init(&src, test_value, test_value_len, test_value_len);
    ASSERT_EQ(src.length, test_value_len);
    ASSERT_EQ(src.capacity, test_value_len);
    ASSERT_EQ((const uint16_t *)src.buffer, (const uint16_t *)test_value);
}

UTEST(kstr_copy, copy_works_check_the_sizes)
{
    DECLARE_KSTR(dest1, 64);
    DECLARE_KSTR(dest2, 32);
    DECLARE_KSTR(dest3, 16);

    struct kstr src;
    kstr_init(&src, test_value, test_value_len, test_value_len);

    ASSERT_TRUE(kstr_copy(&dest1, &src));
    ASSERT_EQ(dest1.length, src.length);
    ASSERT_EQ(dest1.capacity, 64);
    ASSERT_NE(dest1.buffer, src.buffer);
    ASSERT_TRUE(!memcmp(dest1.buffer, src.buffer, src.length * 2));

    ASSERT_TRUE(kstr_copy(&dest2, &src));
    ASSERT_EQ(dest2.length, src.length);
    ASSERT_EQ(dest2.capacity, 32);
    ASSERT_NE(dest2.buffer, src.buffer);
    ASSERT_TRUE(!memcmp(dest2.buffer, src.buffer, src.length * 2));

    ASSERT_FALSE(kstr_copy(&dest3, &src));
}

UTEST(kstr_compare, compare_works)
{
    struct kstr left;
    struct kstr right;

    kstr_init(&left, NULL, 0, 0);
    kstr_init(&right, NULL, 0, 0);
    ASSERT_EQ(kstr_compare(&left, &right), 0);

    kstr_init(&left, test_value, test_value_len, test_value_len);
    kstr_init(&right, test_value, test_value_len, test_value_len);

    ASSERT_EQ(kstr_compare(&left, &right), 0);

    left.length -= 1;
    ASSERT_LT(kstr_compare(&left, &right), 0);

    left.length += 1;
    right.length -= 1;
    ASSERT_GT(kstr_compare(&left, &right), 0);
}

UTEST(kstr_read, read_empty_string)
{
    struct kstr dest;
    kstr_init(&dest, NULL, 0, 0);

    ASSERT_TRUE(kstr_read(&dest, NULL, 0));

    uint16_t data1[] = { '\0', 'a', 'b' };
    ASSERT_TRUE(kstr_read(&dest, data1, ARRAY_SIZE(data1)));

    uint16_t data2[] = { 'a', '\0' };
    ASSERT_FALSE(kstr_read(&dest, data2, ARRAY_SIZE(data2)));
}

UTEST(kstr_read, trim_null_character)
{
    uint16_t buffer[] = { 'H', 'E', 'L', 'L', '0', '\0' };
    const size_t buffer_length = ARRAY_SIZE(buffer) - 1;

    DECLARE_KSTR(s1, 6);
    ASSERT_TRUE(kstr_read(&s1, buffer, ARRAY_SIZE(buffer)));
    ASSERT_EQ(s1.length, ARRAY_SIZE(buffer) - 1);
    ASSERT_TRUE(!memcmp(s1.buffer, buffer, s1.length * 2));

    DECLARE_KSTR(s2, 4);
    ASSERT_FALSE(kstr_read(&s2, buffer, ARRAY_SIZE(buffer)));
}

UTEST(kstr_read, drop_what_comes_after_null_character)
{
    uint16_t buffer[] = { 'H', 'E', 'L', 'L', '0', '\0', 'W', 'O', 'R', 'L', 'D', '\0' };

    DECLARE_KSTR(s1, 16);
    ASSERT_TRUE(kstr_read(&s1, buffer, ARRAY_SIZE(buffer)));
    ASSERT_EQ(s1.length, 5);
    ASSERT_TRUE(!memcmp(s1.buffer, buffer, s1.length * 2));
}

UTEST(kstr_read, read_ascii_doesnt_rely_on_null_character)
{
    DECLARE_KSTR(str, ARRAY_SIZE(test_value));
    ASSERT_TRUE(kstr_read(&str, test_value, ARRAY_SIZE(test_value)));
    ASSERT_EQ(str.length, ARRAY_SIZE(test_value));
    ASSERT_TRUE(!memcmp(str.buffer, test_value, str.length * 2));
}

UTEST(kstr_write, write_succeed_or_fail_appropriately)
{
    uint16_t data[ARRAY_SIZE(test_value)];
    memcpy(data, test_value, sizeof(data));

    DECLARE_KSTR(str, ARRAY_SIZE(data));
    kstr_read(&str, data, ARRAY_SIZE(data));
    // The test data shouldn't contain a null character.
    ASSERT_EQ(str.length, ARRAY_SIZE(data));

    uint16_t buffer1[ARRAY_SIZE(data) * 2];
    ASSERT_TRUE(kstr_write(&str, buffer1, ARRAY_SIZE(buffer1)));
    ASSERT_TRUE(!memcmp(buffer1, test_value, test_value_len));

    uint16_t buffer2[ARRAY_SIZE(data) + 1];
    ASSERT_TRUE(kstr_write(&str, buffer2, ARRAY_SIZE(buffer2)));
    ASSERT_TRUE(!memcmp(buffer2, test_value, test_value_len));

    uint16_t buffer3[ARRAY_SIZE(data)];
    ASSERT_FALSE(kstr_write(&str, buffer3, ARRAY_SIZE(buffer3)));

    ASSERT_FALSE(kstr_write(&str, NULL, 0));
}

UTEST(kstr_read_ascii, read_ascii_doesnt_rely_on_null_character)
{
    char data_ascii[] = { 'H', 'E', 'L', 'L', 'O' };
    uint16_t data_u16[] = { 'H', 'E', 'L', 'L', 'O' };

    DECLARE_KSTR(str, ARRAY_SIZE(data_ascii));
    ASSERT_TRUE(kstr_read_ascii(&str, data_ascii, ARRAY_SIZE(data_ascii)));
    ASSERT_EQ(str.length, ARRAY_SIZE(data_ascii));
    ASSERT_TRUE(!memcmp(str.buffer, data_u16, str.length * 2));
}

UTEST(kstr_read_ascii, trim_null_characters)
{
    char buffer[] = { 'H', 'E', 'L', 'L', '0', '\0', 'W', 'O', 'R', 'L', 'D', '\0' };
    uint16_t result[] = { 'H', 'E', 'L', 'L', '0', '\0', 'W', 'O', 'R', 'L', 'D', '\0' };

    DECLARE_KSTR(s1, 6);
    ASSERT_TRUE(kstr_read_ascii(&s1, buffer, 6));
    ASSERT_EQ(s1.length, 5);
    ASSERT_TRUE(!memcmp(s1.buffer, result, s1.length * 2));

    DECLARE_KSTR(s2, 4);
    ASSERT_FALSE(kstr_read_ascii(&s2, buffer, 6));

    DECLARE_KSTR(s3, 16);
    ASSERT_TRUE(kstr_read_ascii(&s3, buffer, ARRAY_SIZE(buffer)));
    ASSERT_EQ(s3.length, 5);
    ASSERT_TRUE(!memcmp(s3.buffer, result, s3.length * 2));
}

UTEST(kstr_write_ascii, write_succeed_or_fail_appropriately)
{
    char data_ascii[] = { 'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D' };
    char data[ARRAY_SIZE(data_ascii)];
    memcpy(data, data_ascii, sizeof(data));

    DECLARE_KSTR(str, ARRAY_SIZE(data));
    kstr_read_ascii(&str, data, ARRAY_SIZE(data));
    // The test data shouldn't contain a null character.
    ASSERT_EQ(str.length, ARRAY_SIZE(data));

    char buffer1[ARRAY_SIZE(data) * 2];
    ASSERT_TRUE(kstr_write_ascii(&str, buffer1, ARRAY_SIZE(buffer1)));
    ASSERT_TRUE(!memcmp(buffer1, data_ascii, ARRAY_SIZE(data_ascii)));

    char buffer2[ARRAY_SIZE(data) + 1];
    ASSERT_TRUE(kstr_write_ascii(&str, buffer2, ARRAY_SIZE(buffer2)));
    ASSERT_TRUE(!memcmp(buffer2, data_ascii, ARRAY_SIZE(data_ascii)));

    char buffer3[ARRAY_SIZE(data)];
    ASSERT_FALSE(kstr_write_ascii(&str, buffer3, ARRAY_SIZE(buffer3)));

    ASSERT_FALSE(kstr_write_ascii(&str, NULL, 0));
}
