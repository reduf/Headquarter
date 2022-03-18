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

UTEST(test_premaster_computationm, test_expected_results)
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
