#pragma once

struct kstr {
    size_t    length;
    size_t    capacity;
    uint16_t *buffer;
};

// `struct kstr_hdr` is a helper struct that can be used in order to define
// an analog of a `struct kstr` that can be used in movable struct that has
// a buffer for the string. For example:
// struct Agent {
//     struct kstr_hdr name;
//     uint16_t name_buffer[20];
// }
// In this case, the struct can be moved with `memcpy` without worrying
// invalidating pointers.
struct kstr_hdr {
    size_t length;
    size_t capacity;
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

void kstr_init_from_kstr_hdr(struct kstr *str, struct kstr_hdr *hdr);
struct kstr kstr_get_from_kstr_hdr(struct kstr_hdr *hdr);

void kstr_hdr_init(struct kstr_hdr *hdr, uint16_t *buffer, size_t capacity);
bool kstr_hdr_copy(struct kstr_hdr *dest, const struct kstr_hdr *src);
bool kstr_hdr_copy_from_kstr(struct kstr_hdr *dest, const struct kstr *src);
bool kstr_hdr_read(struct kstr_hdr *hdr, const uint16_t *src, size_t size);
bool kstr_hdr_write(struct kstr_hdr *hdr, uint16_t *buffer, size_t size);
bool kstr_hdr_read_ascii(struct kstr_hdr *hdr, const char *src, size_t size);
bool kstr_hdr_write_ascii(struct kstr_hdr *hdr, char *buffer, size_t size);
int  kstr_hdr_compare_kstr(const struct kstr_hdr *s1, const struct kstr *s2);
