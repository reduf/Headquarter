#include "utest.h"

#include <stdint.h>

#include <portal/prf.h>

UTEST_MAIN();

UTEST(tls_prf_sha256, tls_prf_sha256_master_secret)
{
    const char label[] = "master secret";
    const size_t label_len = sizeof(label) - 1;

    const uint8_t random[] = \
        "\x18\x36\x66\x89\x6B\xA4\xB4\x53\x34\x13\x1A\x67\x80\xCF\x1A\x86"
        "\x95\x30\x62\x83\xEE\xFC\xD8\x52\x85\x22\xCB\xC0\x1C\xB2\xED\x44"
        "\x18\x36\x66\x86\xFC\x66\x08\x26\xF8\x85\xA0\xB7\xEA\x54\x75\xEE"
        "\x92\x0D\xB2\x57\x53\x71\x61\x14\xC6\x04\xA4\xF3\x65\x12\x38\x73";
    const size_t random_len = sizeof(random) - 1;

    const uint8_t secret[] = \
        "\xD8\xD7\xA7\xC2\x10\x49\xC7\x14\x98\x93\x4A\x3F\xCB\xC4\x5C\x6B"
        "\xD3\x04\xE7\x58\x92\x26\xDF\xEF\xA0\xA1\x95\xC7\x48\xFB\x67\x74"
        "\xF0\xAF\x77\x89\xC5\xE8\x84\xF4\xC5\x59\x39\xAE\xC1\xC0\x75\xB4"
        "\x5F\x0F\x24\x09\x7A\xC7\x8C\xD5\xEE\x29\x50\x95\x37\xB9\xFB\xC5"
        "\xDC\xAB\x9D\xB3\x5F\x3D\x33\x85\x69\xAC\x40\x87\x73\x2D\xD2\x19"
        "\xDC\xBF\x20\x26\xDF\x6B\xF7\x55\x12\x2B\x1F\x2D\x25\xDC\xCA\x59"
        "\xF1\xF7\xBE\x04\x28\xCA\x56\x77\x1F\xD8\x0C\x50\xD4\x4C\xF6\x68"
        "\x84\x74\x5A\xDD\x0A\x9D\x3E\x27\x38\x52\x7C\x43\xB7\x7B\x8B\x80";
    const size_t secret_len = sizeof(secret) - 1;

    char output[48];
    const size_t output_len = sizeof(output);

    int ret = tls_prf_sha256(
        secret, secret_len,
        label, label_len,
        random, random_len,
        output, output_len);

    const uint8_t expected[] = \
        "\xCA\x6A\xDB\x47\x89\x76\xF5\x34\xC3\xB3\x67\xB6\x66\x24\x77\xAF"
        "\xE7\xB9\xA0\xF7\x5E\xBD\xFF\x14\xBE\x71\x1B\x69\x33\x35\x3F\xC2"
        "\x02\x5E\xBB\x87\xC8\x96\x7F\xAB\xB9\x45\x94\x5C\x0C\xFB\x3B\xC9";
    const size_t expected_len = sizeof(expected) - 1;

    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output_len, expected_len);
    ASSERT_TRUE(memcmp(output, expected, expected_len) == 0);
}

UTEST(tls_prf_sha256, tls_prf_sha256_key_expansion)
{
    const char label[] = "key expansion";
    const size_t label_len = sizeof(label) - 1;

    const uint8_t random[] = \
        "\x18\x36\x66\x86\xFC\x66\x08\x26\xF8\x85\xA0\xB7\xEA\x54\x75\xEE"
        "\x92\x0D\xB2\x57\x53\x71\x61\x14\xC6\x04\xA4\xF3\x65\x12\x38\x73"
        "\x18\x36\x66\x89\x6B\xA4\xB4\x53\x34\x13\x1A\x67\x80\xCF\x1A\x86"
        "\x95\x30\x62\x83\xEE\xFC\xD8\x52\x85\x22\xCB\xC0\x1C\xB2\xED\x44";
    const size_t random_len = sizeof(random) - 1;

    const uint8_t secret[] = \
        "\xCA\x6A\xDB\x47\x89\x76\xF5\x34\xC3\xB3\x67\xB6\x66\x24\x77\xAF"
        "\xE7\xB9\xA0\xF7\x5E\xBD\xFF\x14\xBE\x71\x1B\x69\x33\x35\x3F\xC2"
        "\x02\x5E\xBB\x87\xC8\x96\x7F\xAB\xB9\x45\x94\x5C\x0C\xFB\x3B\xC9";
    const size_t secret_len = sizeof(secret) - 1;

    char output[20];
    const size_t output_len = sizeof(output);

    int ret = tls_prf_sha256(
        secret, secret_len,
        label, label_len,
        random, random_len,
        output, output_len);

    const uint8_t expected[] = \
        "\xD2\xC9\x42\xA2\x6D\xFC\x83\x78\x93\x4A\x08\xD7\x1F\x0B\xA3\x9F"
        "\xC4\xE8\xAA\xAB";
    const size_t expected_len = sizeof(expected) - 1;

    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output_len, expected_len);
    ASSERT_TRUE(memcmp(output, expected, expected_len) == 0);
}

UTEST(tls_prf_sha256, tls_prf_sha256_client_finished)
{
    const char label[] = "client finished";
    const size_t label_len = sizeof(label) - 1;

    const uint8_t random[] = \
        "\xE3\xC4\x89\x40\x5A\x2E\xFC\xE0\x4E\x2C\x2C\xD3\xD8\xDC\x17\x27"
        "\x61\xCB\x56\x73\xA5\x27\x40\xA9\x77\xA6\x7D\x81\x2E\x2F\x11\x03";
    const size_t random_len = sizeof(random) - 1;

    const uint8_t secret[] = \
        "\xCA\x6A\xDB\x47\x89\x76\xF5\x34\xC3\xB3\x67\xB6\x66\x24\x77\xAF"
        "\xE7\xB9\xA0\xF7\x5E\xBD\xFF\x14\xBE\x71\x1B\x69\x33\x35\x3F\xC2"
        "\x02\x5E\xBB\x87\xC8\x96\x7F\xAB\xB9\x45\x94\x5C\x0C\xFB\x3B\xC9";
    const size_t secret_len = sizeof(secret) - 1;

    char output[12];
    const size_t output_len = sizeof(output);

    int ret = tls_prf_sha256(
        secret, secret_len,
        label, label_len,
        random, random_len,
        output, output_len);

    const uint8_t expected[] = "\x81\x4C\x3B\x43\x03\xF4\xD2\x16\x55\x86\x33\xEE";
    const size_t expected_len = sizeof(expected) - 1;

    ASSERT_EQ(ret, 0);
    ASSERT_EQ(output_len, expected_len);
    ASSERT_TRUE(memcmp(output, expected, expected_len) == 0);
}

UTEST(tls_prf_sha256, tls_prf_sha256_multiple_finish)
{
    const char label[] = "key expansion";
    const size_t label_len = sizeof(label) - 1;

    const uint8_t random[] = \
        "\x18\x37\x94\x2C\xCB\xDA\x07\xB7\x33\x71\x48\x8E\x49\xE1\x7A\xE1"
        "\x35\xE6\x27\xCA\xA0\xEF\xB0\x7F\x7E\x31\x5E\x0A\xA0\x3C\x00\x88"
        "\x18\x37\x94\x31\x19\x1F\x18\x49\x2E\xFF\x3B\x7E\x64\x71\x1E\x3D"
        "\xB8\x01\x24\x76\xE9\x03\x34\xCC\x23\x86\xD6\xF5\x29\xC6\xE7\x34";
    const size_t random_len = sizeof(random) - 1;

    const uint8_t secret[] = \
        "\x38\x4F\x34\xF5\xB2\xD2\xC4\x10\x97\x7B\x4C\x9A\x13\x9E\x2E\x52"
        "\x5F\x6F\x50\x9C\x04\xB9\x74\xF6\x47\x31\x5E\x43\xFF\x14\x0B\xFD"
        "\x97\x2B\x1A\xA0\xAE\x6F\x4D\x81\x81\xA9\xAB\x5C\x04\x7E\x5C\xDD";
    const size_t secret_len = sizeof(secret) - 1;

    const uint8_t expected1[] = \
        "\xF1\x35\xBD\xB8\x3D\x58\xC6\xAD\x71\x48\xDE\x44\xAE\x61\x3E\x17"
        "\x0B\xDC\xE1\x1B";
    const size_t expected1_len = sizeof(expected1) - 1;

    const uint8_t expected2[] = \
        "\x05\xFD\xA5\x94\x8E\xA4\x88\xBC\x1D\x90\x86\x5E\xF7\x29\x2F\xC7"
        "\xD7\x0A\x71\x37";
    const size_t expected2_len = sizeof(expected2) - 1;

    const uint8_t expected3[] = \
        "\xEB\xE5\xAE\xF5\xD6\xCC\x59\x45\x47\x7E\xC2\x70\x7F\x9F\xB5\xDA"
        "\xCC\xEC\xA4\x7C\x07\xC5\x3C\xC3\x83\xC1\xA1\x79\x20\x18\x0F\x80";
    const size_t expected3_len = sizeof(expected3) - 1;

    const uint8_t expected4[] = \
        "\xA5\x48\x80\xE5\x95\x06\xF3\x1B\x0A\xD0\xF6\x9A\x71\x85\x3B\x3C"
        "\x5B\xF4\x41\x4E\x1C\x12\x26\x7B\x62\xE2\x30\x52\x13\x56\x7D\x27";
    const size_t expected4_len = sizeof(expected4) - 1;

    uint8_t output[104];
    const size_t output_len = sizeof(output);

    const size_t expected_len = expected1_len + expected2_len + expected3_len + expected4_len;
    ASSERT_EQ(output_len, expected_len);

    int ret = tls_prf_sha256(
        secret, secret_len,
        label, label_len,
        random, random_len,
        output, output_len);

    ASSERT_EQ(ret, 0);

    const uint8_t *offset = output;
    ASSERT_TRUE(memcmp(offset, expected1, expected1_len) == 0);

    offset = offset + expected1_len;
    ASSERT_TRUE(memcmp(offset, expected2, expected2_len) == 0);

    offset = offset + expected2_len;
    ASSERT_TRUE(memcmp(offset, expected3, expected3_len) == 0);

    offset = offset + expected3_len;
    ASSERT_TRUE(memcmp(offset, expected4, expected4_len) == 0);
}
