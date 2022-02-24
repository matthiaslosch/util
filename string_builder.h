#ifndef STRING_BUILDER_H_INCLUDE
#define STRING_BUILDER_H_INCLUDE

#include <stdarg.h> // va_list

#if defined(_WIN32) || defined(WIN32)
#define SB__WINDOWS
#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__)) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#define SB__UNIX
#endif

#if defined(__has_attribute)
#if __has_attribute(format)
#define SB__ATTRIBUTE_FORMAT(fmt, va) __attribute__((format(printf, fmt, va)))
#endif
#endif
#ifndef SB__ATTRIBUTE_FORMAT
#define SB__ATTRIBUTE_FORMAT(...)
#endif

#ifndef SB_BUFFER_CAPACITY
#define SB_BUFFER_CAPACITY 16384 // Define this before including to change the buffer capacity.
#endif

#ifdef SB_STATIC
#define SB__PUBLICDEC static
#define SB__PUBLICDEF static
#else
#ifdef __cplusplus
#define SB__PUBLICDEC extern "C"
#define SB__PUBLICDEF extern "C"
#else
#define SB__PUBLICDEC extern
#define SB__PUBLICDEF
#endif
#endif

#ifndef SB_DECORATE
#define SB_DECORATE(name) sb_##name // Define this before including if you want to change the names.
#endif

#ifdef _MSC_VER
#define sb__u8  unsigned __int8
#define sb__i8  __int8
#define sb__u16 unsigned __int16
#define sb__i16 __int16
#define sb__u32 unsigned __int32
#define sb__i32 __int32
#define sb__u64 unsigned __int64
#define sb__i64 __int64
#else
#define sb__u8  unsigned char
#define sb__i8  char
#define sb__u16 unsigned short
#define sb__i16 short
#define sb__u32 unsigned int
#define sb__i32 int
#define sb__u64 unsigned long long
#define sb__i64 long long
#endif

typedef struct Sb_Buffer {
    unsigned char data[SB_BUFFER_CAPACITY];
    size_t length;
    struct Sb_Buffer *next;
} Sb_Buffer;

typedef struct String_Builder {
    Sb_Buffer first_buffer;
    Sb_Buffer *last_buffer;
    int number_of_buffers;
} String_Builder;

SB__PUBLICDEC void SB_DECORATE(init)(String_Builder *sb);
SB__PUBLICDEC void SB_DECORATE(free)(String_Builder *sb);
SB__PUBLICDEC int SB_DECORATE(is_empty)(String_Builder *sb);
SB__PUBLICDEC void SB_DECORATE(append_len)(String_Builder *sb, const char *string, size_t length);
SB__PUBLICDEC void SB_DECORATE(unchecked_append_len)(String_Builder *sb, const char *string, size_t length);
SB__PUBLICDEC void SB_DECORATE(append_string)(String_Builder *sb, const char *string);
SB__PUBLICDEC void SB_DECORATE(append_u8)(String_Builder *sb, sb__u8 val);
SB__PUBLICDEC void SB_DECORATE(append_u16)(String_Builder *sb, sb__u16 val);
SB__PUBLICDEC void SB_DECORATE(append_u32)(String_Builder *sb, sb__u32 val);
SB__PUBLICDEC void SB_DECORATE(append_u64)(String_Builder *sb, sb__u64 val);
SB__PUBLICDEC void SB_DECORATE(append_i8)(String_Builder *sb, sb__i8 val);
SB__PUBLICDEC void SB_DECORATE(append_i16)(String_Builder *sb, sb__i16 val);
SB__PUBLICDEC void SB_DECORATE(append_i32)(String_Builder *sb, sb__i32 val);
SB__PUBLICDEC void SB_DECORATE(append_i64)(String_Builder *sb, sb__i64 val);

// Provide overloads for C++.
#ifdef __cplusplus
#ifdef SB_STATIC
static void SB_DECORATE(append)(String_Builder *sb, const char *string, size_t length);
static void SB_DECORATE(append)(String_Builder *sb, const char *string);
static void SB_DECORATE(append)(String_Builder *sb, sb__u8 val);
static void SB_DECORATE(append)(String_Builder *sb, sb__u16 val);
static void SB_DECORATE(append)(String_Builder *sb, sb__u32 val);
static void SB_DECORATE(append)(String_Builder *sb, sb__u64 val);
static void SB_DECORATE(append)(String_Builder *sb, sb__i8 val);
static void SB_DECORATE(append)(String_Builder *sb, sb__i16 val);
static void SB_DECORATE(append)(String_Builder *sb, sb__i32 val);
static void SB_DECORATE(append)(String_Builder *sb, sb__i64 val);

static void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string, size_t length);
static void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string);
#else
extern void SB_DECORATE(append)(String_Builder *sb, const char *string, size_t length);
extern void SB_DECORATE(append)(String_Builder *sb, const char *string);
extern void SB_DECORATE(append)(String_Builder *sb, sb__u8 val);
extern void SB_DECORATE(append)(String_Builder *sb, sb__u16 val);
extern void SB_DECORATE(append)(String_Builder *sb, sb__u32 val);
extern void SB_DECORATE(append)(String_Builder *sb, sb__u64 val);
extern void SB_DECORATE(append)(String_Builder *sb, sb__i8 val);
extern void SB_DECORATE(append)(String_Builder *sb, sb__i16 val);
extern void SB_DECORATE(append)(String_Builder *sb, sb__i32 val);
extern void SB_DECORATE(append)(String_Builder *sb, sb__i64 val);
extern void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string, size_t length);
extern void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string);
#endif
#else // Compiling as C, so link with 'extern "C"'.
SB__PUBLICDEC void SB_DECORATE(append_string)(String_Builder *sb, const char *string);
SB__PUBLICDEC void SB_DECORATE(append)(String_Builder *sb, const char *string);
SB__PUBLICDEC void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string);
#endif
SB__PUBLICDEC void SB_DECORATE(vappendf)(String_Builder *sb, const char *format, va_list va);
SB__PUBLICDEC void SB_DECORATE(appendf)(String_Builder *sb, const char *format, ...) SB__ATTRIBUTE_FORMAT(2, 3);
SB__PUBLICDEC int SB_DECORATE(to_string)(String_Builder *sb, char **string);

#endif // !STRING_BUILDER_H_INCLUDE

#ifdef STRING_BUILDER_IMPLEMENTATION

#include <assert.h> // assert()
#include <stdarg.h> // va_list, va_start(), va_arg(), va_end()
#include <string.h> // memcpy(), strlen()
#include <stddef.h> // size_t, NULL

#ifdef SB_SYSTEM_ALLOCATOR
#if defined(SB__WINDOWS)
#include <windows.h> // VirtualAlloc(), VirtualFree()

void *sb__malloc(size_t size)
{
    return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void sb__free(void *ptr, size_t size)
{
    VirtualFree(ptr, size, MEM_RELEASE);
}

#elif defined(SB__UNIX)
#include <sys/mman.h> // mmap(), munmap()

void *sb__malloc(size_t size)
{
    return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void sb__free(void *ptr, size_t size)
{
    munmap(ptr, size);
}

#else
#error "The OS you're using is currently not supported by String Builder."
#endif
#else // SB_SYSTEM_ALLOCATOR
#include <stdlib.h>

void *sb__malloc(size_t size)
{
    return malloc(size);
}

void sb__free(void *ptr, size_t size)
{
    (void)size;
    free(ptr);
}
#endif

SB__PUBLICDEF void SB_DECORATE(init)(String_Builder *sb)
{
    sb->last_buffer = &sb->first_buffer;
    sb->last_buffer->length = 0;
    sb->last_buffer->next = NULL;

    sb->number_of_buffers = 1;
}

SB__PUBLICDEF void SB_DECORATE(free)(String_Builder *sb)
{
    assert(sb);

    while (sb->number_of_buffers-- > 1) {
        // Get the second to last buffer.
        Sb_Buffer *new_last = &sb->first_buffer;
        while (new_last->next != sb->last_buffer)
            new_last = new_last->next;

        sb__free(sb->last_buffer, sizeof(Sb_Buffer));
        new_last->next = NULL;
        sb->last_buffer = new_last;
    }
}

static char *sb__itoa(int value, char *result, int base)
{
    if (base < 2 || base > 36) {
        *result = 0;
        return result;
    }

    char *cursor = result;
    char *start = result;
    int old_value;

    do {
        old_value = value;
        value /= base;
        int index = (old_value - (value * base)) + 35;
        *cursor++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[index];
    } while (value);

    // Apppy negative sign.
    if (old_value < 0)
        *cursor++ = '-';

    char *end = cursor;
    --cursor;

    // Reverse the result string.
    while (cursor > start) {
        char c = *start;
        *start++ = *cursor;
        *cursor-- = c;
    }

    *end = 0;

    return result;
}

static int sb__expand(String_Builder *sb)
{
    assert(sb);

    sb->last_buffer->next = (Sb_Buffer *)sb__malloc(sizeof(Sb_Buffer));
    if (!sb->last_buffer->next)
        return 0;

    sb->last_buffer = sb->last_buffer->next;
    sb->last_buffer->length = 0;
    sb->last_buffer->next = NULL;
    sb->number_of_buffers++;

    return 1;
}

SB__PUBLICDEF int SB_DECORATE(is_empty)(String_Builder *sb)
{
    if (!sb->first_buffer.length)
        return 1;

    return 0;
}

SB__PUBLICDEF void SB_DECORATE(append_len)(String_Builder *sb, const char *string, size_t length)
{
    assert(sb);

    const char *cursor = string;

    // We might be given a string that is bigger than a single remaining or even empty bucket.
    // Check if that is the case. If it is, cut up the string into the biggest piece
    // that can fit into the last bucket. Create a new bucket after that and repeat
    // until the remaining string is smaller than the capacity of the last bucket.
    size_t remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
    while ((remaining_space - length) <= 0) {
        memcpy(sb->last_buffer->data + sb->last_buffer->length, cursor, remaining_space);
        cursor += remaining_space;
        sb->last_buffer->length = SB_BUFFER_CAPACITY;
        if (!sb__expand(sb))
            return;
        length -= remaining_space;
        remaining_space = SB_BUFFER_CAPACITY;
    }

    memcpy(sb->last_buffer->data + sb->last_buffer->length, cursor, length);
    sb->last_buffer->length += length;
}

SB__PUBLICDEF void SB_DECORATE(append_string)(String_Builder *sb, const char *string)
{
    size_t length = strlen(string);
    SB_DECORATE(append_len)(sb, string, length);
}

SB__PUBLICDEF void SB_DECORATE(append_u8)(String_Builder *sb, sb__u8 val)
{
    assert(sb);

    size_t remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
    if (remaining_space <= 0) {
        if (!sb__expand(sb))
            return;
    }

    sb->last_buffer->data[0] = val;
    sb->last_buffer->length++;
}

SB__PUBLICDEF void SB_DECORATE(append_u16)(String_Builder *sb, sb__u16 val)
{
    assert(sb);

    size_t remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
    if (remaining_space <= 1) {
        if (!sb__expand(sb))
            return;
    }

    memcpy(sb->last_buffer->data + sb->last_buffer->length, &val, 2);
    sb->last_buffer->length += 2;
}

SB__PUBLICDEF void SB_DECORATE(append_u32)(String_Builder *sb, sb__u32 val)
{
    assert(sb);

    size_t remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
    if (remaining_space <= 3) {
        if (!sb__expand(sb))
            return;
    }

    memcpy(sb->last_buffer->data + sb->last_buffer->length, &val, 4);
    sb->last_buffer->length += 4;
}

SB__PUBLICDEF void SB_DECORATE(append_u64)(String_Builder *sb, sb__u64 val)
{
    assert(sb);

    size_t remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
    if (remaining_space <= 7) {
        if (!sb__expand(sb))
            return;
    }

    memcpy(sb->last_buffer->data + sb->last_buffer->length, &val, 8);
    sb->last_buffer->length += 8;
}

SB__PUBLICDEF void SB_DECORATE(append_i8)(String_Builder *sb, sb__i8 val)
{
    assert(sb);

    size_t remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
    if (remaining_space <= 0) {
        if (!sb__expand(sb))
            return;
    }

    memcpy(sb->last_buffer->data + sb->last_buffer->length, (sb__u8 *)&val, 1);
    sb->last_buffer->length++;
}

SB__PUBLICDEF void SB_DECORATE(append_i16)(String_Builder *sb, sb__i16 val)
{
    assert(sb);

    size_t remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
    if (remaining_space <= 1) {
        if (!sb__expand(sb))
            return;
    }

    memcpy(sb->last_buffer->data + sb->last_buffer->length, (sb__u8 *)&val, 2);
    sb->last_buffer->length += 2;
}

SB__PUBLICDEF void SB_DECORATE(append_i32)(String_Builder *sb, sb__i32 val)
{
    assert(sb);

    size_t remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
    if (remaining_space <= 3) {
        if (!sb__expand(sb))
            return;
    }

    memcpy((sb__i32 *)(sb->last_buffer->data + sb->last_buffer->length), &val, 4);
    sb->last_buffer->length += 4;
}

SB__PUBLICDEF void SB_DECORATE(append_i64)(String_Builder *sb, sb__i64 val)
{
    assert(sb);

    size_t remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
    if (remaining_space <= 7) {
        if (!sb__expand(sb))
            return;
    }

    memcpy((sb__i64 *)(sb->last_buffer->data + sb->last_buffer->length), &val, 8);
    sb->last_buffer->length += 8;
}

SB__PUBLICDEF void SB_DECORATE(unchecked_append_len)(String_Builder *sb, const char *string, size_t length)
{
    assert(sb);

    memcpy(sb->last_buffer->data + sb->last_buffer->length, string, length);
    sb->last_buffer->length += length;
}

#ifdef __cplusplus
#ifdef SB_STATIC
static void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string, size_t length)
{
    SB_DECORATE(unchecked_append_len)(sb, string, length);
}
static void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string)
#endif
void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string, size_t length)
{
    SB_DECORATE(unchecked_append_len)(sb, string, length);
}
void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string)
#else
SB__PUBLICDEF void SB_DECORATE(unchecked_append)(String_Builder *sb, const char *string)
#endif
{
    size_t length = strlen(string);
    SB_DECORATE(unchecked_append_len)(sb, string, length);
}

SB__PUBLICDEF void SB_DECORATE(vappendf)(String_Builder *sb, const char *format, va_list va)
{
    while (*format) {
        int pos = 0;
        while (format[pos] != '%' && format[pos] != '\0') {
            ++pos;
        }
        SB_DECORATE(append_len)(sb, format, pos);

        if (!format[pos])
            return;

        format += pos + 1;

        switch (*format) {
        case 's': {
            SB_DECORATE(append)(sb, va_arg(va, char *));
            break;
        }
        case 'd': {
            char val[20];
            sb__itoa(va_arg(va, int), val, 10);
            SB_DECORATE(append)(sb, val);
            break;
        }
        case 'c': {
            char val = (char)va_arg(va, int);
            SB_DECORATE(append_len)(sb, &val, 1);
            break;
        }
        case '%': {
            SB_DECORATE(append_len)(sb, "%", 1);
        }
        }
        ++format;
    }
}

SB__PUBLICDEF void SB_DECORATE(appendf)(String_Builder *sb, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    SB_DECORATE(vappendf)(sb, format, va);
    va_end(va);
}

SB__PUBLICDEF int SB_DECORATE(to_string)(String_Builder *sb, char **string)
{
    if (!sb) {
        *string = NULL;
        return -1;
    }

    size_t total_length = sb->first_buffer.length;
    Sb_Buffer *buf = &sb->first_buffer;
    while (buf->next) {
        buf = buf->next;
        total_length += buf->length;
    }
    char *result = (char *)sb__malloc(total_length+1);
    result[total_length] = '\0';

    char *cursor = result;
    buf = &sb->first_buffer;

    do {
        memcpy(cursor, buf->data, buf->length);
        cursor += buf->length;
        buf = buf->next;
    } while (buf);

    *string = result;

    return (int)total_length;
}

// C++ append() overload implementations.

#ifdef __cplusplus
#ifdef SB_STATIC
static void SB_DECORATE(append)(String_Builder *sb, const char *string, size_t length)
{
    SB_DECORATE(append_len)(sb, string, length);
}

static void SB_DECORATE(append)(String_Builder *sb, const char *string)
{
    SB_DECORATE(append_string)(sb, string);
}

static void SB_DECORATE(append)(String_Builder *sb, sb__u8 val)
{
    SB_DECORATE(append_u8)(sb, val);
}

static void SB_DECORATE(append)(String_Builder *sb, sb__u16 val)
{
    SB_DECORATE(append_u16)(sb, val);
}

static void SB_DECORATE(append)(String_Builder *sb, sb__u32 val)
{
    SB_DECORATE(append_u32)(sb, val);
}

static void SB_DECORATE(append)(String_Builder *sb, sb__u64 val)
{
    SB_DECORATE(append_u64)(sb, val);
}

static void SB_DECORATE(append)(String_Builder *sb, sb__i8 val)
{
    SB_DECORATE(append_i8)(sb, val);
}

static void SB_DECORATE(append)(String_Builder *sb, sb__16 val)
{
    SB_DECORATE(append_i16)(sb, val);
}

static void SB_DECORATE(append)(String_Builder *sb, sb__i32 val)
{
    SB_DECORATE(append_i32)(sb, val);
}

static void SB_DECORATE(append)(String_Builder *sb, sb__i64 val)
{
    SB_DECORATE(append_i64)(sb, val);
}

#else
void SB_DECORATE(append)(String_Builder *sb, const char *string, size_t length)
{
    SB_DECORATE(append_len)(sb, string, length);
}
void SB_DECORATE(append)(String_Builder *sb, const char *string)
{
    SB_DECORATE(append_string)(sb, string);
}

void SB_DECORATE(append)(String_Builder *sb, sb__u8 val)
{
    SB_DECORATE(append_u8)(sb, val);
}

void SB_DECORATE(append)(String_Builder *sb, sb__u16 val)
{
    SB_DECORATE(append_u16)(sb, val);
}

void SB_DECORATE(append)(String_Builder *sb, sb__u32 val)
{
    SB_DECORATE(append_u32)(sb, val);
}

void SB_DECORATE(append)(String_Builder *sb, sb__u64 val)
{
    SB_DECORATE(append_u64)(sb, val);
}

void SB_DECORATE(append)(String_Builder *sb, sb__i8 val)
{
    SB_DECORATE(append_i8)(sb, val);
}

void SB_DECORATE(append)(String_Builder *sb, sb__i16 val)
{
    SB_DECORATE(append_i16)(sb, val);
}

void SB_DECORATE(append)(String_Builder *sb, sb__i32 val)
{
    SB_DECORATE(append_i32)(sb, val);
}

void SB_DECORATE(append)(String_Builder *sb, sb__i64 val)
{
    SB_DECORATE(append_i64)(sb, val);
}
#endif
#else
SB__PUBLICDEF void SB_DECORATE(append)(String_Builder *sb, const char *string)
{
    SB_DECORATE(append_string)(sb, string);
}
#endif

#endif // STRING_BUILDER_IMPLEMENTATION
