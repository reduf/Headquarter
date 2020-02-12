#ifdef ARRAY_C
#error "array.c included more than once"
#endif
#define ARRAY_C

#include "array.h"

#ifndef ARRAY_MIN_CAPACITY
# define ARRAY_MIN_CAPACITY 8
#endif

#define ARRAY_ALLOC_ZERO 1

#define array_max(a, b) ((b) < (a) ? (a) : (b))
#define array_min(a, b) ((b) > (a) ? (a) : (b))

static void *
msdn_realloc(void *addr, size_t size)
{
    if (addr != NULL) {
        return realloc(addr, size);
    } else {
        return malloc(size);
    }
}

int
_array_init(array_void_t *a, size_t capacity, const size_t elem_size)
{
    assert(a && elem_size > 0);

    capacity = array_max(capacity, ARRAY_MIN_CAPACITY);
    a->size = 0;
    a->data = msdn_realloc(NULL, capacity * elem_size);
    if (!a->data)
        return 0;
#ifdef ARRAY_ALLOC_ZERO
    memset(a->data, 0, capacity * elem_size);
#endif
    a->capacity = capacity;
    return 1;
}

int
_array_init2(array_void_t *a, size_t capacity, const size_t elem_size)
{
    if (!_array_init(a, capacity, elem_size))
        return 0;
    a->size = a->capacity;
    return 1;
}

void
_array_reset(array_void_t *a)
{
    a->size = 0;
    a->capacity = 0;
    if (a->data) free(a->data);
#ifndef NDEBUG
    a->data = NULL;
#endif
}

int
_array_resize(array_void_t *a, size_t new_capacity, const size_t elem_size)
{
    assert(a && elem_size > 0);

    new_capacity = array_max(new_capacity, ARRAY_MIN_CAPACITY);
    size_t new_size = new_capacity * elem_size;
    assert((new_size / elem_size) == new_capacity);

    void *data = msdn_realloc(a->data, new_size);
    if (!data) return 0;

    a->data = data;
#ifdef ARRAY_ALLOC_ZERO
    if (new_capacity >= a->capacity) {
        char *b = (char *)a->data + (a->capacity * elem_size);
        memset(b, 0, (new_capacity - a->capacity) * elem_size);
    }
#endif

    a->size = (a->size > new_capacity) ? new_capacity : a->size;
    a->capacity = new_capacity;
    return 1;
}

int
_array_grow_to(array_void_t *a, size_t min, const size_t elem_size)
{
    assert(a && elem_size > 0);
    size_t new_capacity = (a->capacity * 2) + 1;
    if (new_capacity < min)
        new_capacity = min;
    return _array_resize(a, new_capacity, elem_size);
}

int
_array_reserve(array_void_t *a, size_t count, const size_t elem_size)
{
    assert(a && elem_size > 0);
    if (a->capacity - a->size < count) {
        size_t new_capacity = a->capacity * 2;
        if (new_capacity < a->size + count)
            new_capacity = a->size + count;
        return _array_resize(a, new_capacity, elem_size);
    }
    return 1;
}

void
_array_remove(array_void_t *a, size_t index, const size_t elem_size)
{
    assert(a && elem_size > 0);

    if (index >= a->size)
        return;

    size_t new_size = a->size - 1;
    if (index < new_size) {
        // if it's not the last element, we swap with the last.
        char *dst = (char *)a->data + (index * elem_size);
        char *src = (char *)a->data + (new_size * elem_size);
        memcpy(dst, src, elem_size);
    }
    a->size = new_size;
}

int
_array_insert(array_void_t *a, size_t count, void *ptr, const size_t elem_size)
{
    assert(a && elem_size > 0);
    if (!count) return 1;

    if (!_array_reserve(a, count, elem_size))
        return 0;
    assert(a->size + count <= a->capacity);

    char *buff = (char *)a->data + (a->size * elem_size);
    memcpy(buff, ptr, count * elem_size);
    a->size += count;
    return 1;
}

int
_array_copy(array_void_t *dest, array_void_t *src, const size_t elem_size)
{
    array_clear(*dest);
    return _array_insert(dest, src->size, src->data, elem_size);
}

void *
_array_push(array_void_t *a, size_t n, const size_t elem_size)
{
    if (!_array_reserve(a, n, elem_size))
        return NULL;
    void *ptr = (char *)a->data + (a->size * elem_size);
    a->size += n;
    assert(a->size <= a->capacity);
    return ptr;
}

void
_array_remove_ordered(array_void_t *a, size_t i, const size_t elem_size)
{
    assert(a && elem_size > 0);
    if (i >= a->size)
        return;
    size_t rem = a->size - i;
    if (rem) {
        char *data = (char *)a->data + (i * elem_size);
        memmove(data, data + elem_size, rem * elem_size);
    }
    a->size -= 1;
}
