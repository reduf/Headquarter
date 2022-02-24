#include "utest.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <common/array.h>
#include <common/macro.h>
#include <portal/sts.h>
#include <portal/login.h>

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

UTEST(sts, sts_write_request)
{
    array_uint8_t request;
    array_init(request, 1024);

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

    sts_write_request(&request, url, ARRAY_SIZE(url) - 1, content, ARRAY_SIZE(content) - 1);
    ASSERT_EQ(request.size, ARRAY_SIZE(expected) - 1);
    ASSERT_TRUE(!memcmp(request.data, expected, request.size));
}

UTEST(sts, sts_write_sequenced_request)
{
    array_uint8_t request;
    array_init(request, 1024);

    const char url[] = "/Sts/Connect";
    const char expected[] = \
        "\x50\x20\x2F\x53\x74\x73\x2F\x43\x6F\x6E\x6E\x65\x63\x74\x20\x53"
        "\x54\x53\x2F\x31\x2E\x30\x0D\x0A\x6C\x3A\x30\x0D\x0A\x73\x3A\x31"
        "\x3B\x74\x69\x6D\x65\x6F\x75\x74\x3D\x34\x30\x30\x30\x0D\x0A\x0D"
        "\x0A";

    sts_write_sequenced_request(&request, 1, 4000, url, ARRAY_SIZE(url) - 1, "", 0);
    ASSERT_EQ(request.size, ARRAY_SIZE(expected) - 1);
    ASSERT_TRUE(!memcmp(request.data, expected, request.size));
}

UTEST(sts, portal_login)
{
    bool result = portal_login("toto", "otot");
    ASSERT_TRUE(result);
}

UTEST(parse_sts_request, parse_request_without_content)
{
    const uint8_t raw[] = "STS/1.0 400 Success\r\ns:1R\r\nl:0\r\n\r\n";
    struct sts_request request = {0};
    int ret = parse_sts_request(&request, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_SUCCESS);
    ASSERT_EQ(request.status_code, 400);
    ASSERT_EQ(request.sequence_number, 1);
    ASSERT_EQ(request.content_length, 0);
}

UTEST(parse_sts_request, parse_request_with_unsupported_version)
{
    const uint8_t raw[] = "STS/1.1 400 Success\r\ns:1R\r\nl:0\r\n\r\n";
    struct sts_request request = {0};
    int ret = parse_sts_request(&request, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_UNSUPPORTED_PROTOCOL);
}

UTEST(parse_sts_request, parse_request_with_content)
{
    const char expected[] = "Hello World!\0Hello Sailor!";
    const uint8_t raw[] = "STS/1.0 400 Success\r\ns:1R\r\nl:26\r\n\r\nHello World!\0Hello Sailor!";
    struct sts_request request = {0};
    int ret = parse_sts_request(&request, raw, sizeof(raw) - 1);

    ASSERT_EQ(ret, STSE_SUCCESS);
    ASSERT_EQ(request.content_length, sizeof(expected) - 1);
    ASSERT_TRUE(!memcmp(request.content, expected, sizeof(expected) - 1));
}

UTEST(parse_sts_request, parse_request_with_incomplete_status_line)
{
    const uint8_t raw[] = "STS/1.";
    struct sts_request request = {0};
    int ret = parse_sts_request(&request, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_INCOMPLETE_HEADER);
}

UTEST(parse_sts_request, parse_request_with_incomplete_header)
{
    const uint8_t raw[] = "STS/1.0 400 Success\r\n\r";
    struct sts_request request = {0};
    int ret = parse_sts_request(&request, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_INCOMPLETE_HEADER);
}

UTEST(parse_sts_request, parse_request_with_incomplete_content)
{
    const uint8_t raw[] = "STS/1.0 400 Success\r\ns:1R\r\nl:26\r\n\r\nHello World!\0Hello";
    struct sts_request request = {0};
    int ret = parse_sts_request(&request, raw, sizeof(raw) - 1);
    ASSERT_EQ(ret, STSE_INCOMPLETE_CONTENT);
    ASSERT_EQ(request.status_code, 400);
    ASSERT_EQ(request.sequence_number, 1);
    ASSERT_EQ(request.content_length, 26);
}
