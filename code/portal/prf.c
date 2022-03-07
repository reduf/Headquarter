#ifdef PORTAL_PRF_C
#error "prf.c included more than once"
#endif
#define PORTAL_PRF_C

// This doesn't have to be the limit, but we currently pass in (2 * 32) bytes
// of random data (64 bytes) in addition to the label which is one of
// ["key expansion", "client finished", "master secret"]. So, (64 + 15) bytes
// is exactly what we need for the user given data.
//
// In addition to that, we later need the concat digest + label + random, so
// we reserve an additional 32 bytes for it.
#define MAX_CONCAT_SIZE 111

#define SHA256_DIGEST_SIZE 32

int tls_prf_sha256(
    const uint8_t *secret, size_t secret_len,
    const char *label, size_t label_len,
    const uint8_t *random, size_t random_len,
    uint8_t *output, size_t output_len)
{
    int ret;

    uint8_t digest[SHA256_DIGEST_SIZE];
    uint8_t concat[MAX_CONCAT_SIZE];

    size_t concat_len = SHA256_DIGEST_SIZE + label_len + random_len;
    if (sizeof(concat) < concat_len) {
        fprintf(stderr, "Concat buffer isn't large enough, need %zu bytes\n", concat_len);
        return 1;
    }

    memset(concat, 0, SHA256_DIGEST_SIZE);
    memcpy(concat + SHA256_DIGEST_SIZE, label, label_len);
    memcpy(concat + SHA256_DIGEST_SIZE + label_len, random, random_len);

    const mbedtls_md_info_t *md_info;
    if ((md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256)) == NULL)
        return 1;

    size_t md_len = mbedtls_md_get_size(md_info);
    if (md_len != sizeof(digest))
        return 1;

    mbedtls_md_context_t ctx;
    mbedtls_md_init(&ctx);

    if ((ret = mbedtls_md_setup(&ctx, md_info, 1)) != 0)
        goto cleanup;
    if ((ret = mbedtls_md_hmac_starts(&ctx, secret, secret_len)) != 0)
        goto cleanup;
    if ((ret = mbedtls_md_hmac_update(&ctx, concat + SHA256_DIGEST_SIZE, concat_len - SHA256_DIGEST_SIZE)) != 0)
        goto cleanup;
    if ((ret = mbedtls_md_hmac_finish(&ctx, concat)) != 0)
        goto cleanup;

    for (size_t i = 0; i < output_len; i += md_len)
    {
        if ((ret = mbedtls_md_hmac_reset(&ctx)) != 0)
            goto cleanup;
        if ((ret = mbedtls_md_hmac_update(&ctx, concat, concat_len)) != 0)
            goto cleanup;
        if ((ret = mbedtls_md_hmac_finish(&ctx, digest)) != 0)
            goto cleanup;

        if ((ret = mbedtls_md_hmac_reset(&ctx)) != 0)
            goto cleanup;
        if ((ret = mbedtls_md_hmac_update(&ctx, concat, md_len)) != 0)
            goto cleanup;
        if ((ret = mbedtls_md_hmac_finish(&ctx, concat)) != 0)
            goto cleanup;

        size_t k = (output_len < i + md_len) ? output_len % md_len : md_len;

        for (size_t j = 0; j < k; ++j)
            output[i + j]  = digest[j];
    }

cleanup:
    mbedtls_md_free(&ctx);
    if (ret != 0)
        ret = 1;
    return ret;
}
