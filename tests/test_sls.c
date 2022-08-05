#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <common/win32/win32.h>
#include <Ws2tcpip.h>

#include "utest.h"

#include <mbedtls/aes.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/md.h>
#include <mbedtls/sha1.h>
#include <mbedtls/sha256.h>

#include <common/array.h>
#include <common/macro.h>

#include <portal/stream.h>
#include <portal/socket.h>

#include <portal/ssl.h>

UTEST_STATE();
int main(int argc, char **argv)
{
    int ret;

    WSADATA wsa_data;
    if ((ret = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0) {
        fprintf(stderr, "'WSAStartup' failed: %d\n", WSAGetLastError());
        return 1;
    }

    ret = utest_main(argc, argv);

    WSACleanup();
    return ret;
}

#if 0
UTEST(test_with_fake_server, ensure_handhsake_work)
{
    const uint8_t client_private[] = \
        "\x72\x61\x6A\xE5\xD8\xC0\xF7\xB7\x8E\x1B\x10\x9B\xAA\x18\x7D\x1D"
        "\x34\x73\xA6\x19\x38\xF4\xEA\x5D\x47\xD4\x23\x75\x05\xC8\xC6\x38";
    const size_t client_private_len = sizeof(client_private) - 1;

    const uint8_t client_random[] = \
        "\x18\x42\xb8\x82\x39\x87\xae\x42\x9d\xc6\xe1\x25\xdd\xbf\xb9\xa9"
        "\xd0\xf7\xe7\xe8\xf4\xa6\x7f\xfd\x9d\xd3\x9d\x78\xb5\xe2\xca\x28";
    const size_t client_random_len = sizeof(client_random) - 1;

    const uint8_t iv[] = "\xA6\x66\xC4\xAF\x08\x56\x42\x49\x99\x8B\x76\x3F\x90\x2D\x89\xB9";
    const size_t iv_len = sizeof(iv) - 1;    

    int ret;
    struct ssl_sts_connection ssl;

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6112);
    addr.sin_addr.s_addr = htonl(0x7f000001);

    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT_NE(fd, INVALID_SOCKET);

    ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    ASSERT_EQ(ret, 0);

    ssl_sts_connection_init(&ssl);
    ssl.fd = fd;
    ret = ssl_sts_connection_init_srp(&ssl, "test@gmail.com", "root");
    ASSERT_EQ(ret, 0);
    ret = ssl_sts_connection_seed_test(
        &ssl,
        client_private, client_private_len,
        client_random, client_random_len,
        iv, iv_len);
    ASSERT_EQ(ret, 0);
    ret = ssl_sts_connection_handshake(&ssl);
    ASSERT_EQ(ret, 0);
}
#endif

UTEST(test_cipher_and_checksum, test_expected_results)
{
    const uint8_t mac_enc_key[] = \
        "\x66\x7B\x8D\x28\x46\xCC\x45\x94\x7B\x64\xDA\x8B\x0D\xF9\xC7\x19"
        "\xBE\x38\x2A\x32";
    const size_t mac_enc_key_len = sizeof(mac_enc_key) - 1;
    const uint8_t mac_dec_key[] = \
        "\x5D\xAE\x02\x9C\x0D\x17\xD6\xC9\x2C\x78\x4D\x90\x26\xFC\xB6\x1A"
        "\xD1\x4E\xB1\x9E";
    const size_t mac_dec_key_len = sizeof(mac_dec_key) - 1;
    const uint8_t cipher_enc_key[] = \
        "\x03\xAD\xBD\xF2\x67\x3C\xAF\x9C\x0F\x43\xDB\x72\x82\x9E\xA1\x46"
        "\x5A\xE4\xF9\x9E\x56\xAC\xE6\x65\xEC\xA8\xBC\x6E\xA4\x48\x4F\x6E";
    const size_t cipher_enc_key_len = sizeof(cipher_enc_key) - 1;
    const uint8_t cipher_dec_key[] = \
        "\x18\xD7\xF5\x73\x54\xE8\x75\xDC\x2D\x21\xDF\xB0\x71\x43\x0D\xEC"
        "\x2D\xD6\x0D\xAA\x6F\x2F\xCC\xE2\x2C\xE2\x17\xBD\x13\xF5\x57\x19";
    const size_t cipher_dec_key_len = sizeof(cipher_dec_key) - 1;
    uint8_t iv[] = "\x79\xFD\x7C\xF3\x82\x40\x87\xD7\x1D\x93\x88\x26\xE6\x7E\xEB\xB1";
    const size_t iv_len = sizeof(iv) - 1;

    const uint8_t packet_id[] = "\x00\x00\x00\x00\x00\x00\x00\x00";
    const size_t packet_id_len = sizeof(packet_id) - 1;
    const uint8_t hmac_data[] = \
        "\x16\x03\x03\x00\x10\x14\x00\x00\x0C\x9C\x41\x13\x89\x57\xA4\x39"
        "\x99\x0D\x3C\x9D\x0C";
    const size_t hmac_data_len = sizeof(hmac_data) - 1;
    const uint8_t hmac_expected[] = \
        "\xB9\x49\x7B\x1D\x53\x4B\x4F\x36\xE2\x6E\xF4\x5F\xF9\x7F\x2E\xD6"
        "\x09\x72\xAC\x90";
    const size_t hmac_expected_len = sizeof(hmac_expected) - 1;

    const uint8_t input[] = \
        "\x14\x00\x00\x0C\x9C\x41\x13\x89\x57\xA4\x39\x99\x0D\x3C\x9D\x0C"
        "\xB9\x49\x7B\x1D\x53\x4B\x4F\x36\xE2\x6E\xF4\x5F\xF9\x7F\x2E\xD6"
        "\x09\x72\xAC\x90\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B";
    const size_t input_len = sizeof(input) - 1;

    const uint8_t expected[] = \
        "\x0C\xC6\x81\xB4\x90\xF2\xA7\xA8\xCC\xDD\x01\x55\xA2\x7B\x9D\xB8"
        "\x17\x55\xA2\x48\x78\xD9\xD4\x80\xC6\x1C\x73\x6F\xE4\x8F\x22\x19"
        "\x51\xA2\xB3\xC6\x73\x0B\x94\x21\x7A\x24\xD1\x4D\x1E\x93\x49\x19";
    const size_t expected_len = sizeof(expected) - 1;

    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
    ASSERT_NE(md_info, NULL);

    int ret;
    mbedtls_aes_context cipher_enc;
    mbedtls_aes_context cipher_dec;
    mbedtls_md_context_t mac_enc;
    mbedtls_md_context_t mac_dec;

    mbedtls_aes_init(&cipher_enc);
    mbedtls_aes_init(&cipher_dec);
    mbedtls_md_init(&mac_enc);
    mbedtls_md_init(&mac_dec);

    ret = mbedtls_aes_setkey_enc(&cipher_enc, cipher_enc_key, cipher_enc_key_len * 8);
    ASSERT_EQ(ret, 0);
    ret = mbedtls_aes_setkey_dec(&cipher_dec, cipher_dec_key, cipher_dec_key_len * 8);
    ASSERT_EQ(ret, 0);

    const int is_hmac = 1;
    ret = mbedtls_md_setup(&mac_enc, md_info, is_hmac);
    ASSERT_EQ(ret, 0);
    ret = mbedtls_md_hmac_starts(&mac_enc, mac_enc_key, mac_enc_key_len);
    ASSERT_EQ(ret, 0);
    ret = mbedtls_md_setup(&mac_dec, md_info, is_hmac);
    ASSERT_EQ(ret, 0);
    ret = mbedtls_md_hmac_starts(&mac_dec, mac_dec_key, mac_dec_key_len);
	ASSERT_EQ(ret, 0);

    ret = mbedtls_md_hmac_update(&mac_enc, packet_id, packet_id_len);
    ASSERT_EQ(ret, 0);
    ret = mbedtls_md_hmac_update(&mac_enc, hmac_data, hmac_data_len);
    ASSERT_EQ(ret, 0);

    char hmac_result[sizeof(hmac_expected) - 1];
    ret = mbedtls_md_hmac_finish(&mac_enc, hmac_result);
    ASSERT_EQ(ret, 0);
    ret = memcmp(hmac_result, hmac_expected, hmac_expected_len);
    ASSERT_EQ(ret, 0);

    uint8_t output[sizeof(expected) - 1];
    ret = mbedtls_aes_crypt_cbc(
        &cipher_enc,
        MBEDTLS_AES_ENCRYPT,
        input_len,
        iv,
        input,
        output);
    ASSERT_EQ(ret, 0);

    ret = memcmp(output, expected, expected_len);
    ASSERT_EQ(ret, 0);

    mbedtls_aes_free(&cipher_enc);
    mbedtls_aes_free(&cipher_dec);
    mbedtls_md_free(&mac_enc);
    mbedtls_md_free(&mac_dec);
}

UTEST(test_premaster_computation, test_expected_results)
{
    const uint8_t prime[] = \
        "\xee\xaf\x0a\xb9\xad\xb3\x8d\xd6\x9c\x33\xf8\x0a\xfa\x8f\xc5\xe8"
        "\x60\x72\x61\x87\x75\xff\x3c\x0b\x9e\xa2\x31\x4c\x9c\x25\x65\x76"
        "\xd6\x74\xdf\x74\x96\xea\x81\xd3\x38\x3b\x48\x13\xd6\x92\xc6\xe0"
        "\xe0\xd5\xd8\xe2\x50\xb9\x8b\xe4\x8e\x49\x5c\x1d\x60\x89\xda\xd1"
        "\x5d\xc7\xd7\xb4\x61\x54\xd6\xb6\xce\x8e\xf4\xad\x69\xb1\x5d\x49"
        "\x82\x55\x9b\x29\x7b\xcf\x18\x85\xc5\x29\xf5\x66\x66\x0e\x57\xec"
        "\x68\xed\xbc\x3c\x05\x72\x6c\xc0\x2f\xd4\xcb\xf4\x97\x6e\xaa\x9a"
        "\xfd\x51\x38\xfe\x83\x76\x43\x5b\x9f\xc6\x1d\x2f\xc0\xeb\x06\xe3";
    const size_t prime_len = sizeof(prime) - 1;

    const uint8_t generator[] = "\x02";
    const size_t generator_len = sizeof(generator) - 1;

    const uint8_t salt[] = "\x72\xad\x14\x6f\x1e\xd0\x57\xdf";
    const size_t salt_len = sizeof(salt) - 1;

    const uint8_t server_public[] = \
        "\xbe\x8c\xa3\xaa\xed\xf8\xbe\x84\x9d\x62\xae\x97\x95\xc0\xf2\xb4"
        "\x26\xc1\xf8\xc9\xd8\xc4\x78\xe5\xdd\xd6\x34\x8d\xe3\xa5\xa7\xe8"
        "\x96\xd4\x7b\x4b\x1d\x02\xa6\x37\x92\x13\x97\x85\xf3\x6a\x37\x67"
        "\xab\x3e\xbf\xb8\x76\x2f\x99\xb2\xdc\x65\xac\xc0\x2a\x29\x95\x23"
        "\x9f\xa2\x1a\xc0\x1e\x4c\x22\x9d\x1b\x95\x7b\xe4\x20\x72\x36\x60"
        "\xb0\x40\xed\x2c\x29\x8f\x28\x94\x55\x7b\x95\x07\x4c\xae\x1d\xba"
        "\x94\xce\xb3\xa7\x93\x22\x31\xe4\x0a\x27\xc3\x89\x5e\xe9\x8f\xaa"
        "\x2b\x6d\x99\xb5\x4f\x32\x19\x62\x9c\x06\x86\xd4\x56\xbf\xae\x75";
    const size_t server_public_len = sizeof(server_public) - 1;

    const uint8_t client_private[] = \
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x71\xE2\x02\xDD\x22\x16\x22\x23\x02\x1E\xBD\xA7\x27\x43\x25\x22"
        "\x6C\x24\xEC\xE0\x18\xC6\x51\x89\x7B\x10\x63\xD1\x7F\x83\xA7\x17";
    const size_t client_private_len = sizeof(client_private) - 1;

    const uint8_t client_public[] = \
        "\x16\x62\xc1\x1a\x2c\x7c\x63\x6e\x7b\xb0\xea\xbe\xbd\x54\x7f\x22"
        "\x79\xa9\xff\x27\x21\x42\xe9\x86\x6b\x59\x1f\x48\x1f\x15\xa5\x5c"
        "\x98\x00\xf8\xdd\x8b\xa6\x1b\x77\xf9\xc8\xd9\xb2\xba\x28\x78\xb7"
        "\x41\x84\xa7\xad\xfe\x5c\x58\x47\x83\x53\x40\x7a\xff\xd2\xbc\x7d"
        "\x5b\x5c\x19\x31\x06\xbb\xa3\xc9\xf2\xb4\x37\xdc\xbf\x3f\x22\xd6"
        "\xa9\xd8\xf4\x73\xee\x6e\xc6\xa4\xbe\x52\x87\xb7\x28\x5e\xf4\x12"
        "\xd7\xfa\xdb\x62\x2b\x72\xdc\xa6\xa8\xfd\xb4\xd6\x0d\x7c\x80\xf4"
        "\x26\x9d\x89\x0b\xbb\x2a\x75\x82\x30\x46\x43\x0d\x72\x10\x40\x31";
    const size_t client_public_len = sizeof(client_public) - 1;

    const uint8_t expected[] = \
        "\xAB\xD6\x52\x05\x66\xC7\xB9\x0F\x11\x2C\x41\xFA\x6C\xCC\xD7\x41"
        "\xA7\xDD\xD5\x56\xC2\x99\x62\xD1\x23\x82\x3E\xF4\x36\x62\x68\x9D"
        "\x84\x6D\x9D\x20\xBA\xD3\x9C\xDB\x7F\x2F\xB8\xF7\xFC\x0A\xC4\x6F"
        "\x58\xC3\xC9\xF5\x8D\x00\x89\x1A\x1B\x9B\xF1\xA1\x95\x36\x75\x32"
        "\x6C\x92\x5E\x7E\x46\x28\x32\x47\xE3\xC4\x65\xC0\xE5\xDE\xE4\x25"
        "\x12\xCA\xC9\xB9\x42\x41\x27\x18\x2B\x96\x9F\x4F\x93\x0A\x32\x54"
        "\xDE\x1C\x83\xC8\xD2\x22\x9B\x0B\x7C\xE6\x42\x82\x11\x1F\x4D\xEB"
        "\xB5\x1B\x0D\x43\x37\x05\xC7\xA8\x0D\x08\x85\xC9\x36\x0C\x35\xF6";
    size_t expected_len = sizeof(expected) - 1;

    struct ssl_sts_connection ssl;
    ssl_sts_connection_init(&ssl);

    ASSERT_EQ(sizeof(ssl.client_key.private), client_private_len);
    memcpy(ssl.client_key.private, client_private, client_private_len);
    ASSERT_EQ(sizeof(ssl.client_key.public), client_public_len);
    memcpy(ssl.client_key.public, client_public, client_public_len);

    ASSERT_EQ(sizeof(ssl.server_key.prime), prime_len);
    memcpy(ssl.server_key.prime, prime, prime_len);
    ASSERT_EQ(sizeof(ssl.server_key.generator), generator_len);
    memcpy(ssl.server_key.generator, generator, generator_len);
    ASSERT_EQ(sizeof(ssl.server_key.salt), salt_len);
    memcpy(ssl.server_key.salt, salt, salt_len);
    ASSERT_EQ(sizeof(ssl.server_key.server_public), server_public_len);
    memcpy(ssl.server_key.server_public, server_public, server_public_len);

    int ret;

    ret = ssl_sts_connection_init_srp(&ssl, "test@gmail.com", "root");
    ASSERT_EQ(ret, 0);
    ret = ssl_srp_compute_premaster_secret(&ssl);
    ASSERT_EQ(ret, 0);

    ASSERT_EQ(sizeof(ssl.premaster_secret), expected_len);
    ret = memcmp(ssl.premaster_secret, expected, expected_len);
    ASSERT_EQ(ret, 0);

    ssl_sts_connection_free(&ssl);
}
