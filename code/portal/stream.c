#ifdef PORTAL_STREAM_C
#error "stream.c included more than once"
#endif
#define PORTAL_STREAM_C

void appendv(array_uint8_t *buffer, const char *fmt, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);

    int ret = vsnprintf(NULL, 0, fmt, args);
    if (ret < 0) {
        va_end(args_copy);
        abort();
    }

    // We need to allocate one more bytes, because  of `vsnprintf`.
    // We will pop this "\0" byte later.
    uint8_t *write_ptr = array_push(buffer, (size_t)ret + 1);
    vsnprintf((char *)write_ptr, (size_t)ret + 1, fmt, args_copy);
    (void)array_pop(buffer);
    va_end(args_copy);
}

void appendf(array_uint8_t *buffer, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    appendv(buffer, fmt, args);
    va_end(args);
}

void array_add_be_uint16(array_uint8_t *buffer, uint16_t value)
{
    uint8_t *ptr = array_push(buffer, sizeof(value));
    be16enc(ptr, value);
}

uint32_t be24dec(const void *pp)
{
    uint8_t const *p = (uint8_t const *)pp;
    return (((unsigned)p[0] << 16) | (p[1] << 8) | p[2]);
}

int chk_stream_read8(const uint8_t **data, size_t *length, uint8_t *out)
{
    if (*length < sizeof(*out))
        return ERR_SSL_BAD_INPUT_DATA;

    *out = **data;
    *data += sizeof(*out);
    *length -= sizeof(*out);
    return 0;
}

int chk_stream_read16(const uint8_t **data, size_t *length, uint16_t *out)
{
    if (*length < sizeof(*out))
        return ERR_SSL_BAD_INPUT_DATA;

    *out = be16dec(*data);
    *data += sizeof(*out);
    *length -= sizeof(*out);
    return 0;
}

int chk_stream_read32(const uint8_t **data, size_t *length, uint32_t *out)
{
    if (*length < sizeof(*out))
        return ERR_SSL_BAD_INPUT_DATA;

    *out = be32dec(*data);
    *data += sizeof(*out);
    *length -= sizeof(*out);
    return 0;
}

int chk_stream_read(const uint8_t **data, size_t *length, uint8_t *out, size_t out_len)
{
    if (*length < out_len)
        return ERR_SSL_BAD_INPUT_DATA;

    memcpy(out, *data, out_len);
    *data += out_len;
    *length -= out_len;
    return 0;
}
