#pragma once

struct kstr {
    size_t    length;
    size_t    capacity;
    uint16_t *buffer;
};

#define DECLARE_KSTR(name, size) \
    uint16_t name ## _buffer[size]; \
    struct kstr name = {0, size, name ## _buffer};

size_t u16len(const uint16_t* src, size_t size);

void kstr_init(struct kstr *str, uint16_t *buffer, size_t length, size_t capacity);

bool kstr_copy(struct kstr *dest, const struct kstr *src);

int kstr_compare(const struct kstr *s1, const struct kstr *s2);

bool kstr_read(struct kstr *str, const uint16_t *src, size_t size);
bool kstr_write(struct kstr *str, uint16_t *buffer, size_t size);

bool kstr_read_ascii(struct kstr *str, const char *src, size_t size);
bool kstr_write_ascii(struct kstr *str, char *buffer, size_t size);
