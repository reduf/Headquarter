#include "utest.h"

#include <mbedtls/aes.h>
#include <mbedtls/md.h>

UTEST_MAIN();

UTEST(tls_aes_crc_encrypt, aes_iv_generation)
{
    int ret;

    mbedtls_aes_context encrypt;
    mbedtls_aes_init(&encrypt);

    const uint8_t key[] = \
        "\xD7\x94\x61\x09\xF9\xF0\xEF\xC3\xF4\x2E\xFC\xC1\x88\x87\xC4\x5F"
        "\xD2\xDA\x7B\xF2\x96\x6C\xC9\x6C\x35\xF5\x00\x2D\x80\xE1\x2E\x4C";
    const size_t key_len = sizeof(key) - 1;

    const size_t AES_IO_LEN = 16;
    const uint8_t input[] = "\x4C\xBC\x36\x87\x52\x87\xE7\x4B\x90\xAC\xF0\x68\xD2\xFB\xD7\xB2";
    const uint8_t expected[] = "\xAD\xB7\x47\x62\x21\xDD\x26\xBC\x60\xDA\x88\x8F\x60\x1B\x9F\x7C";
    uint8_t output[sizeof(expected) - 1];

    ASSERT_EQ(AES_IO_LEN, sizeof(input) - 1);
    ASSERT_EQ(AES_IO_LEN, sizeof(expected) - 1);

    ret = mbedtls_aes_setkey_enc(&encrypt, key, key_len * 8);
    ASSERT_EQ(ret, 0);

    ret = mbedtls_internal_aes_encrypt(&encrypt, input, output);
    ASSERT_EQ(ret, 0);
    ASSERT_TRUE(!memcmp(expected, output, AES_IO_LEN));

    mbedtls_aes_free(&encrypt);
}

UTEST(tls_aes_crc_encrypt, test_first_encryption)
{
    int ret;

    mbedtls_aes_context encrypt;
    mbedtls_aes_init(&encrypt);

    const uint8_t key[] = \
        "\x8F\x67\xE6\xAC\x37\x89\x61\x41\x5B\xCE\x31\xE2\x78\x2B\x11\x48"
        "\xF9\x8F\xE9\xF2\x99\x17\xDF\xD3\x1A\xD1\x49\x0E\xE2\x00\x91\x0B";
    const size_t key_len = sizeof(key) - 1;

    const uint8_t decrypted[] = \
        "\x14\x00\x00\x0C\x3A\x1C\x6D\xBE\x76\xB8\xB8\x63\x41\x1A\x36\xF0"
        "\xAB\x22\xCC\xF6\x7B\x80\x5C\x2D\x13\xCC\x7B\xA3\xA9\x69\x17\x9F"
        "\x0F\x93\xD9\xF7\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B";
    const size_t decrypted_len = sizeof(decrypted) - 1;

    const uint8_t expected[] = \
        "\xA7\x4A\x83\x93\x89\x4F\x05\xA2\x9D\xA4\xA3\x8A\x5A\xA4\xAB\xDB"
        "\x5D\x7D\x9A\xBC\xB9\x39\x88\xF6\x00\x68\x37\xF8\x59\x56\xF1\x1D"
        "\xD1\x53\x48\xCC\x51\x47\x58\xED\xCA\xBD\xCF\x7C\x31\xD4\x78\x29";
    const size_t expected_len = sizeof(expected) - 1;

    uint8_t iv[] = "\xEE\x99\x8B\x73\xCE\x15\x76\xB7\xF6\xC4\xAA\x67\x5B\x88\x91\xCF";

    ret = mbedtls_aes_setkey_enc(&encrypt, key, key_len * 8);
    ASSERT_EQ(ret, 0);

    uint8_t output[sizeof(decrypted) - 1];
    ret = mbedtls_aes_crypt_cbc(
        &encrypt,
        MBEDTLS_AES_ENCRYPT,
        decrypted_len,
        iv,
        decrypted,
        output);

    ASSERT_EQ(ret, 0);
    ASSERT_EQ(decrypted_len, expected_len);
    ASSERT_TRUE(!memcmp(expected, output, expected_len));

    mbedtls_aes_free(&encrypt);
}

UTEST(tls_hmac, test_hmac_key)
{
    int ret;

    const uint8_t key[] = \
        "\x5F\xF8\x73\x50\xEE\xAE\xDE\x5E\x67\x5C\x23\xD8\x5E\x9B\x57\x02"
        "\x4E\x43\xED\xAF";
    const size_t key_len = sizeof(key) - 1;

    const uint8_t expected[] = \
        "\xFE\xF0\x4C\xFA\x69\xE0\x67\xC7\xBF\xC8\x63\xC1\x27\x5B\x53\x5E"
        "\x2C\x70\xBD\xDC";
    const size_t expected_len = sizeof(expected) - 1;

    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
    ASSERT_NE(md_info, NULL);

    mbedtls_md_context_t hmac;
    mbedtls_md_init(&hmac);

    const int is_hmac = 1;
    ret = mbedtls_md_setup(&hmac, md_info, is_hmac);
    ASSERT_EQ(ret, 0);
    ret = mbedtls_md_hmac_starts(&hmac, key, key_len);
    ASSERT_EQ(ret, 0);

    const size_t p1_len = 8;
    const size_t p2_len = 5;
    const size_t p3_len = 16;

    const uint8_t data[] = \
        "\x00\x00\x00\x00\x00\x00\x00\x00\x16\x03\x03\x00\x10\x14\x00\x00"
        "\x0C\x6E\x0F\x7E\xB0\xD5\x55\x8F\x5D\xE5\x95\x3D\x12";
    const size_t data_len = sizeof(data) - 1;

    ASSERT_EQ(p1_len + p2_len + p3_len, data_len);

    ret = mbedtls_md_hmac_update(&hmac, data, p1_len);
    ASSERT_EQ(ret, 0);
    ret = mbedtls_md_hmac_update(&hmac, data + p1_len, p2_len);
    ASSERT_EQ(ret, 0);
    ret = mbedtls_md_hmac_update(&hmac, data + p1_len + p2_len, p3_len);
    ASSERT_EQ(ret, 0);

    char output1[sizeof(expected) - 1];
    ret = mbedtls_md_hmac_finish(&hmac, output1);
    ASSERT_EQ(ret, 0);
    ASSERT_TRUE(!memcmp(expected, output1, expected_len));
    mbedtls_md_free(&hmac);

    char output2[sizeof(expected) - 1];
    ret = mbedtls_md_hmac(md_info, key, key_len, data, data_len, output2);
    ASSERT_EQ(ret, 0);
    ASSERT_TRUE(!memcmp(expected, output1, expected_len));
}

#if 0
UTEST(tls_hmac, test_multiple_hmac)
{
    int ret;

    const uint8_t key[] = \
        "\xE4\x6C\xB7\x04\xBA\x42\xD1\x37\xB1\xAE\x0D\xFB\x8E\x07\xFE\x69"
        "\xBD\x55\x5A\x03";
    const size_t key_len = sizeof(key) - 1;

    const uint8_t input1[] = \
        "\x00\x00\x00\x00\x00\x00\x00\x00\x16\x03\x03\x00\x10\x14\x00\x00"
        "\x0C\x5F\x6D\xDF\x42\xA1\x1C\x95\x44\xBE\x7E\xCC\xC9";
    const size_t input1_len = sizeof(input1) - 1;

    const uint8_t expected1[] = \
        "\xC7\x22\x0A\xFB\x7C\x97\xF8\x45\x0C\x67\x66\xDF\xF1\xB3\xE7\x42"
        "\x98\x65\x40\xCA";
    const size_t expected1_len = sizeof(expected1) - 1;

    const uint8_t input2[] = \
        "\x00\x00\x00\x00\x00\x00\x00\x01\x17\x03\x03\x00\x1C\x50\x20\x2F"
        "\x53\x74\x73\x2F\x50\x69\x6E\x67\x20\x53\x54\x53\x2F\x31\x2E\x30"
        "\x0D\x0A\x6C\x3A\x30\x0D\x0A\x0D\x0A";
    const size_t input2_len = sizeof(input2) - 1;

    const uint8_t expected2[] = \
        "\x81\x25\x03\x12\x20\x16\xAD\xEB\x70\xB5\xAD\xEF\xE4\x02\xF8\xE5"
        "\xB8\x2F\x73\x91";
    const size_t expected2_len = sizeof(expected2) - 1;

    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
    ASSERT_NE(md_info, NULL);

    char output[sizeof(expected1) - 1];
    ret = mbedtls_md_hmac(md_info, key, key_len, input1, input1_len, output);
    ASSERT_EQ(ret, 0);
    ASSERT_TRUE(!memcmp(expected1, output, expected1_len));

    ret = mbedtls_md_hmac(md_info, key, key_len, input2, input2_len, output);
    ASSERT_EQ(ret, 0);
    ASSERT_TRUE(!memcmp(expected2, output, expected2_len));
}
#endif