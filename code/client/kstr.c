#ifdef CORE_KSTR_C
#error "kstr.c included more than once"
#endif
#define CORE_KSTR_C

size_t u16len(const uint16_t *src, size_t size)
{
    size_t length;
    for (length = 0; length < size; length++) {
        if (!src[length])
            break;
    }
    return length;
}

void kstr_init(struct kstr *str, uint16_t *buffer, size_t length, size_t capacity)
{
    str->length = length;
    str->capacity = capacity;
    str->buffer = buffer;
}

bool kstr_copy(struct kstr *dest, const struct kstr *src)
{
    if (dest->capacity < src->length)
        return false;
    memcpy(dest->buffer, src->buffer, src->length * sizeof(uint16_t));
    dest->length = src->length;
    return true;
}

int kstr_compare(const struct kstr *s1, const struct kstr *s2)
{
    if (s1->length < s2->length)
        return -1;
    else if (s2->length < s1->length)
        return 1;
    else
        return memcmp(s1->buffer, s2->buffer, s1->length * 2);
}

size_t kstr_find_codepoint(const struct kstr *str, uint16_t codepoint)
{
    for (size_t idx = 0; idx < str->length; ++idx) {
        if (str->buffer[idx] == codepoint)
            return idx;
    }
    return (size_t)-1;
}

struct kstr kstr_substr(const struct kstr *str, size_t pos, size_t length)
{
    if (str->length < pos)
        pos = str->length;
    if ((str->length - pos) < length)
        length = str->length - pos;
    size_t capacity = str->capacity - pos;
    return (struct kstr){ .length = length, .capacity = capacity, .buffer = str->buffer + pos };
}

bool kstr_read(struct kstr *str, const uint16_t *src, size_t size)
{
    struct kstr source;
    kstr_init(&source, (uint16_t *)src, size, size);

    size_t trim;
    if ((trim = kstr_find_codepoint(&source, 0)) != (size_t)-1) {
        source = kstr_substr(&source, 0, trim);
    }

    return kstr_copy(str, &source);
}

bool kstr_write(struct kstr *str, uint16_t *buffer, size_t size)
{
    if (str->length >= size)
        return false;
    memcpy(buffer, str->buffer, str->length * sizeof(uint16_t));
    buffer[str->length] = 0;
    return true;
}

bool kstr_read_ascii(struct kstr *str, const char *src, size_t size)
{
    str->length = 0;
    size_t length = strnlen(src, size);
    if (str->capacity < length)
        return false;
    for (size_t i = 0; i < length; i++) {
        if (src[i] & 0x80)
            return false;
        str->buffer[i] = src[i];
    }
    str->length = length;
    return true;
}

bool kstr_write_ascii(struct kstr *str, char *buffer, size_t size)
{
    if (str->length >= size)
        return false;
    for (size_t i = 0; i < str->length; i++) {
        if (str->buffer[i] & ~0x7F)
            return false;
        buffer[i] = str->buffer[i] & 0x7F;
    }
    buffer[str->length] = 0;
    return true;
}
