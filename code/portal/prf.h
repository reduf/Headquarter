#ifdef PORTAL_PRF_H
#error "prf.h included more than once"
#endif
#define PORTAL_PRF_H

int tls_prf_sha256(
    const uint8_t *secret, size_t secret_len,
    const char *label, size_t label_len,
    const uint8_t *random, size_t random_len,
    uint8_t *output, size_t output_len);
