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

struct kb_kstr {
    struct kstr_hdr data;
    uint16_t buffer[1024];
};

UTEST_MAIN();

UTEST(kstr_hdr_init, init_works)
{
    struct kb_kstr src;
    kstr_hdr_init(&src.data, src.buffer, ARRAY_SIZE(src.buffer));

    ASSERT_EQ(src.data.length, 0);
    ASSERT_EQ(src.data.capacity, ARRAY_SIZE(src.buffer));
}

UTEST(kstr_hdr, moving_container_struct_works)
{
    struct kb_kstr src;
    struct kb_kstr dest;

    kstr_hdr_init(&src.data, src.buffer, ARRAY_SIZE(src.buffer));
    kstr_hdr_init(&dest.data, dest.buffer, ARRAY_SIZE(dest.buffer));

    ASSERT_TRUE(kstr_hdr_read(&src.data, test_value, test_value_len));
    ASSERT_EQ(src.data.length, test_value_len);

    struct kstr src_kstr;
    kstr_init_from_kstr_hdr(&src_kstr, &src.data);

    // move struct
    memcpy(&dest, &src, sizeof(src));
    ASSERT_EQ(dest.data.length, test_value_len);
    ASSERT_EQ(kstr_hdr_compare_kstr(&dest.data, &src_kstr), 0);
}
