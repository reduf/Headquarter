#include "utest.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <common/array.h>
#include <common/macro.h>
#include <portal/sts.h>

UTEST_MAIN();

UTEST(sts, ssl_tls12_write_client_hello)
{
    struct ssl_tls12_context ctx;
    ssl_tls2_init(&ctx);

    ctx.random_time = 0x11223344;
    memset(ctx.random_bytes, 0xAA, sizeof(ctx.random_bytes));

    static const char email[] = "abcdefg.12345678@email.com";
    ctx.srp_username_len = ARRAY_SIZE(email) - 1;
    memcpy(ctx.srp_username, email, ctx.srp_username_len);

    static const uint8_t expected[] = \
        "\x01\x00\x00\x58\x03\x03\x11\x22\x33\x44\xAA\xAA\xAA\xAA\xAA\xAA"
        "\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA"
        "\xAA\xAA\xAA\xAA\xAA\xAA\x00\x00\x0C\xC0\x20\xC0\x1D\xFF\x02\xFF"
        "\x01\xFF\x04\xFF\x03\x01\x00\x00\x25\xAD\xAE\x00\x00\x00\x0C\x00"
        "\x1B\x1A\x61\x62\x63\x64\x65\x66\x67\x2E\x31\x32\x33\x34\x35\x36"
        "\x37\x38\x40\x65\x6D\x61\x69\x6C\x2E\x63\x6F\x6D";

    int ret = ssl_tls12_write_client_hello(&ctx);
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(ctx.buffer.size, ARRAY_SIZE(expected) - 1);
    ASSERT_TRUE(!memcmp(ctx.buffer.data, expected, ctx.buffer.size));
}
