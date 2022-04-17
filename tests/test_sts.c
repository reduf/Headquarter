#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "utest.h"

#include <common/array.h>
#include <common/macro.h>
#include <common/uuid.h>

#include <portal/stream.h>
#include <portal/socket.h>

#include <portal/sts.h>
#include <portal/login.h>

UTEST_MAIN();

#if 0
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
        "\x01\xFF\x04\xFF\x03\x01\x00\x00\x23\xAD\xAE\x00\x00\x00\x0C\x00"
        "\x1B\x1A\x61\x62\x63\x64\x65\x66\x67\x2E\x31\x32\x33\x34\x35\x36"
        "\x37\x38\x40\x65\x6D\x61\x69\x6C\x2E\x63\x6F\x6D";

    int ret = ssl_tls12_write_client_hello(&ctx);
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(ctx.buffer.size, ARRAY_SIZE(expected) - 1);
    ASSERT_TRUE(!memcmp(ctx.buffer.data, expected, ctx.buffer.size));

    ssl_tls2_free(&ctx);
}
#endif

UTEST(sts, sts_write_request)
{
    const char url[] = "/Sts/Connect";
    const char content[] = "<Connect>\n<ConnType>400</ConnType>\n<Address>127.127.1.127</Address>\n<ProductType>0</ProductType>\n<ProductName>Gw</ProductName>\n<AppIndex>1</AppIndex>\n<Epoch>666952962</Epoch>\n<Program>1</Program>\n<Build>1002</Build>\n<Process>1337</Process>\n</Connect>\n";
    const uint8_t expected[] = \
        "\x50\x20\x2F\x53\x74\x73\x2F\x43\x6F\x6E\x6E\x65\x63\x74\x20\x53"
        "\x54\x53\x2F\x31\x2E\x30\x0D\x0A\x6C\x3A\x32\x35\x31\x0D\x0A\x0D"
        "\x0A\x3C\x43\x6F\x6E\x6E\x65\x63\x74\x3E\x0A\x3C\x43\x6F\x6E\x6E"
        "\x54\x79\x70\x65\x3E\x34\x30\x30\x3C\x2F\x43\x6F\x6E\x6E\x54\x79"
        "\x70\x65\x3E\x0A\x3C\x41\x64\x64\x72\x65\x73\x73\x3E\x31\x32\x37"
        "\x2E\x31\x32\x37\x2E\x31\x2E\x31\x32\x37\x3C\x2F\x41\x64\x64\x72"
        "\x65\x73\x73\x3E\x0A\x3C\x50\x72\x6F\x64\x75\x63\x74\x54\x79\x70"
        "\x65\x3E\x30\x3C\x2F\x50\x72\x6F\x64\x75\x63\x74\x54\x79\x70\x65"
        "\x3E\x0A\x3C\x50\x72\x6F\x64\x75\x63\x74\x4E\x61\x6D\x65\x3E\x47"
        "\x77\x3C\x2F\x50\x72\x6F\x64\x75\x63\x74\x4E\x61\x6D\x65\x3E\x0A"
        "\x3C\x41\x70\x70\x49\x6E\x64\x65\x78\x3E\x31\x3C\x2F\x41\x70\x70"
        "\x49\x6E\x64\x65\x78\x3E\x0A\x3C\x45\x70\x6F\x63\x68\x3E\x36\x36"
        "\x36\x39\x35\x32\x39\x36\x32\x3C\x2F\x45\x70\x6F\x63\x68\x3E\x0A"
        "\x3C\x50\x72\x6F\x67\x72\x61\x6D\x3E\x31\x3C\x2F\x50\x72\x6F\x67"
        "\x72\x61\x6D\x3E\x0A\x3C\x42\x75\x69\x6C\x64\x3E\x31\x30\x30\x32"
        "\x3C\x2F\x42\x75\x69\x6C\x64\x3E\x0A\x3C\x50\x72\x6F\x63\x65\x73"
        "\x73\x3E\x31\x33\x33\x37\x3C\x2F\x50\x72\x6F\x63\x65\x73\x73\x3E"
        "\x0A\x3C\x2F\x43\x6F\x6E\x6E\x65\x63\x74\x3E\x0A";

    array_uint8_t request;
    array_init(&request);
    int ret = sts_write_request(
        &request,
        url, ARRAY_SIZE(url) - 1,
        content, ARRAY_SIZE(content) - 1);

    ASSERT_EQ(ret, 0);
    ASSERT_EQ(request.size, ARRAY_SIZE(expected) - 1);
    ASSERT_TRUE(!memcmp(request.data, expected, request.size));

    array_reset(&request);
}

UTEST(sts, sts_write_request_with_sequence_number)
{
    array_uint8_t request;
    array_init(&request);

    const char url[] = "/Sts/Connect";
    const char expected[] = \
        "\x50\x20\x2F\x53\x74\x73\x2F\x43\x6F\x6E\x6E\x65\x63\x74\x20\x53"
        "\x54\x53\x2F\x31\x2E\x30\x0D\x0A\x6C\x3A\x30\x0D\x0A\x73\x3A\x31"
        "\x3B\x74\x69\x6D\x65\x6F\x75\x74\x3D\x34\x30\x30\x30\x0D\x0A\x0D"
        "\x0A";

    uint8_t content[] = "";
    int ret = sts_write_request_with_sequence_number(
        &request,
        url, ARRAY_SIZE(url) - 1,
        1, 4000,
        content, 0);

    ASSERT_NE(ret, 0);
    ASSERT_EQ(request.size, ARRAY_SIZE(expected) - 1);
    ASSERT_TRUE(!memcmp(request.data, expected, request.size));
}

UTEST(sts_parse_reply, parse_request_without_content)
{
    const uint8_t raw[] = "STS/1.0 400 Success\r\ns:1R\r\nl:0\r\n\r\n";
    struct sts_reply reply = {0};
    int ret = sts_parse_reply(&reply, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_SUCCESS);
    ASSERT_EQ(reply.status_code, 400);
    ASSERT_EQ(reply.sequence_number, 1);
    ASSERT_EQ(reply.content_length, 0);
}

UTEST(sts_parse_reply, parse_request_with_unsupported_version)
{
    const uint8_t raw[] = "STS/1.1 400 Success\r\ns:1R\r\nl:0\r\n\r\n";
    struct sts_reply reply = {0};
    int ret = sts_parse_reply(&reply, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_UNSUPPORTED_PROTOCOL);
}

UTEST(sts_parse_reply, parse_request_with_content)
{
    const char expected[] = "Hello World!\0Hello Sailor!";
    const uint8_t raw[] = "STS/1.0 400 Success\r\ns:1R\r\nl:26\r\n\r\nHello World!\0Hello Sailor!";
    struct sts_reply reply = {0};
    int ret = sts_parse_reply(&reply, raw, sizeof(raw) - 1);

    ASSERT_EQ(ret, STSE_SUCCESS);
    ASSERT_EQ(reply.content_length, sizeof(expected) - 1);
    ASSERT_TRUE(!memcmp(reply.content, expected, sizeof(expected) - 1));
}

UTEST(sts_parse_reply, parse_request_with_incomplete_status_line)
{
    const uint8_t raw[] = "STS/1.";
    struct sts_reply reply = {0};
    int ret = sts_parse_reply(&reply, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_INCOMPLETE_HEADER);
}

UTEST(sts_parse_reply, parse_request_with_incomplete_header)
{
    const uint8_t raw[] = "STS/1.0 400 Success\r\n\r";
    struct sts_reply reply = {0};
    int ret = sts_parse_reply(&reply, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_INCOMPLETE_HEADER);
}

#if 0
UTEST(sts_parse_reply, parse_request_with_incomplete_content)
{
    const uint8_t raw[] = "STS/1.0 400 Success\r\ns:1R\r\nl:26\r\n\r\nHello World!\0Hello";
    struct sts_reply reply = {0};
    int ret = sts_parse_reply(&reply, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_INCOMPLETE_CONTENT);
    ASSERT_EQ(reply.status_code, 400);
    ASSERT_EQ(reply.sequence_number, 1);
    ASSERT_EQ(reply.content_length, 26);
}

UTEST(sts_process_server_hello, process_valid_message)
{
    const char message[] = \
        "\x16\x03\x03\x00\x30\x02\x00\x00\x2c\x03\x03\x18\x28\x38\x66\x48"
        "\x51\x4b\xaf\x84\x67\x92\xcc\xed\x7f\x68\x33\x5f\xb0\x91\x1c\x01"
        "\x36\x6a\xb3\x78\xd2\x39\x2c\x4a\xa6\xb2\x6c\x00\xc0\x20\x00\x00"
        "\x04\xad\xae\x00\x00";

    struct server_hello expected = {
        .random_time = 0x18283866,
        .random_bytes = "\x48\x51\x4b\xaf\x84\x67\x92\xcc\xed\x7f\x68\x33\x5f\xb0\x91\x1c"
                        "\x01\x36\x6a\xb3\x78\xd2\x39\x2c\x4a\xa6\xb2\x6c",
    };

    struct server_hello hello = {0};

    int ret = sts_process_server_hello(&hello, message, sizeof(message) - 1);
    ASSERT_EQ(ret, 0);
    ASSERT_TRUE(memcmp(&hello, &expected, sizeof(expected)) == 0);
}

UTEST(sts_process_server_key_exchange, process_valid_message)
{
    const char message[] = \
        "\x16\x03\x03\x01\x14\x0c\x00\x01\x10\x00\x80\xee\xaf\x0a\xb9\xad"
        "\xb3\x8d\xd6\x9c\x33\xf8\x0a\xfa\x8f\xc5\xe8\x60\x72\x61\x87\x75"
        "\xff\x3c\x0b\x9e\xa2\x31\x4c\x9c\x25\x65\x76\xd6\x74\xdf\x74\x96"
        "\xea\x81\xd3\x38\x3b\x48\x13\xd6\x92\xc6\xe0\xe0\xd5\xd8\xe2\x50"
        "\xb9\x8b\xe4\x8e\x49\x5c\x1d\x60\x89\xda\xd1\x5d\xc7\xd7\xb4\x61"
        "\x54\xd6\xb6\xce\x8e\xf4\xad\x69\xb1\x5d\x49\x82\x55\x9b\x29\x7b"
        "\xcf\x18\x85\xc5\x29\xf5\x66\x66\x0e\x57\xec\x68\xed\xbc\x3c\x05"
        "\x72\x6c\xc0\x2f\xd4\xcb\xf4\x97\x6e\xaa\x9a\xfd\x51\x38\xfe\x83"
        "\x76\x43\x5b\x9f\xc6\x1d\x2f\xc0\xeb\x06\xe3\x00\x01\x02\x08\x72"
        "\xad\x14\x6f\x1e\xd0\x57\xdf\x00\x80\xd9\xd9\xbc\x33\xa2\xf1\xfd"
        "\x94\x6f\x08\xcb\x95\x1b\x25\xb8\x4b\x41\x61\x2e\x25\x33\x79\x0a"
        "\x09\xc6\x05\x8c\x6c\x28\x11\x76\xcf\xfe\x55\x9a\x76\x72\x73\x5e"
        "\xe3\x2b\x0d\xa9\xa5\x16\x20\xbe\xa7\xca\x5a\x4b\x3a\x0d\xc1\x4a"
        "\x6d\xfe\xcc\x3a\x19\x25\x11\xd2\xd2\x8b\xd3\xcb\x9d\x21\x0a\xe9"
        "\x85\x3d\x29\xf7\xf6\x17\x2e\x78\x8e\x8e\x93\xfb\xed\xf3\x6d\x28"
        "\x7b\x09\x85\x19\xda\xe6\x77\xa2\x54\x79\x06\x4a\x27\xf4\xdc\x01"
        "\x87\xe7\xef\xcd\x6c\x67\x50\xd9\x25\xee\x1a\xff\x5b\x88\x2e\x4f"
        "\x1f\xd4\x71\x37\x9a\x01\xd6\x35\x6e";

    struct server_key expected = {
        .prime = "\xee\xaf\x0a\xb9\xad\xb3\x8d\xd6\x9c\x33\xf8\x0a\xfa\x8f\xc5\xe8"
                 "\x60\x72\x61\x87\x75\xff\x3c\x0b\x9e\xa2\x31\x4c\x9c\x25\x65\x76"
                 "\xd6\x74\xdf\x74\x96\xea\x81\xd3\x38\x3b\x48\x13\xd6\x92\xc6\xe0"
                 "\xe0\xd5\xd8\xe2\x50\xb9\x8b\xe4\x8e\x49\x5c\x1d\x60\x89\xda\xd1"
                 "\x5d\xc7\xd7\xb4\x61\x54\xd6\xb6\xce\x8e\xf4\xad\x69\xb1\x5d\x49"
                 "\x82\x55\x9b\x29\x7b\xcf\x18\x85\xc5\x29\xf5\x66\x66\x0e\x57\xec"
                 "\x68\xed\xbc\x3c\x05\x72\x6c\xc0\x2f\xd4\xcb\xf4\x97\x6e\xaa\x9a"
                 "\xfd\x51\x38\xfe\x83\x76\x43\x5b\x9f\xc6\x1d\x2f\xc0\xeb\x06\xe3",
        .generator = "\x02",
        .salt = "\x72\xad\x14\x6f\x1e\xd0\x57\xdf",
        .server_public = "\xd9\xd9\xbc\x33\xa2\xf1\xfd\x94\x6f\x08\xcb\x95\x1b\x25\xb8\x4b"
                         "\x41\x61\x2e\x25\x33\x79\x0a\x09\xc6\x05\x8c\x6c\x28\x11\x76\xcf"
                         "\xfe\x55\x9a\x76\x72\x73\x5e\xe3\x2b\x0d\xa9\xa5\x16\x20\xbe\xa7"
                         "\xca\x5a\x4b\x3a\x0d\xc1\x4a\x6d\xfe\xcc\x3a\x19\x25\x11\xd2\xd2"
                         "\x8b\xd3\xcb\x9d\x21\x0a\xe9\x85\x3d\x29\xf7\xf6\x17\x2e\x78\x8e"
                         "\x8e\x93\xfb\xed\xf3\x6d\x28\x7b\x09\x85\x19\xda\xe6\x77\xa2\x54"
                         "\x79\x06\x4a\x27\xf4\xdc\x01\x87\xe7\xef\xcd\x6c\x67\x50\xd9\x25"
                         "\xee\x1a\xff\x5b\x88\x2e\x4f\x1f\xd4\x71\x37\x9a\x01\xd6\x35\x6e"
    };

    struct server_key calculated = {0};
    int ret = sts_process_server_key_exchange(&calculated, message, sizeof(message) - 1);

    ASSERT_EQ(ret, 0);
    ASSERT_TRUE(memcmp(&calculated, &expected, sizeof(expected)) == 0);
}

UTEST(sts_process_server_done, process_valid_message)
{
    const char message[] = "\x16\x03\x03\x00\x04\x0e\x00\x00\x00";
    int ret = sts_process_server_done(message, sizeof(message) - 1);
    ASSERT_EQ(ret, 0);
}
#endif
