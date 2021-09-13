#ifndef STRING_BUILDER_H_INCLUDE
#define STRING_BUILDER_H_INCLUDE

#include <stdarg.h> // va_list

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

typedef struct Sb_Buffer {
    char data[SB_BUFFER_CAPACITY];
    int length;
    struct Sb_Buffer *next;
} Sb_Buffer;

typedef struct String_Builder {
    Sb_Buffer first_buffer;
    Sb_Buffer *last_buffer;
    int number_of_buffers;
} String_Builder;

SB__PUBLICDEC void SB_DECORATE(init)(String_Builder *sb);
SB__PUBLICDEC void SB_DECORATE(free)(String_Builder *sb);
SB__PUBLICDEC void SB_DECORATE(append_len)(String_Builder *sb, char *string, int length);
#ifdef __cplusplus // If compiled as C++, provide an overload for append_len() as append(). Disable 'extern "C"' linkage for this to work.
#ifdef SB_STATIC
static void SB_DECORATE(append)(String_Builder *sb, char *string, int length);
static void SB_DECORATE(append)(String_Builder *sb, char *string);
#else
extern void SB_DECORATE(append)(String_Builder *sb, char *string, int length);
extern void SB_DECORATE(append)(String_Builder *sb, char *string);
#endif
#else // Compiling as C, so link with 'extern "C"'.
SB__PUBLICDEC void SB_DECORATE(append)(String_Builder *sb, char *string);
#endif
SB__PUBLICDEC void SB_DECORATE(vappendf)(String_Builder *sb, char *format, va_list va);
SB__PUBLICDEC void SB_DECORATE(appendf)(String_Builder *sb, char *format, ...);
SB__PUBLICDEC int SB_DECORATE(to_string)(String_Builder *sb, char **string);

#endif // !STRING_BUILDER_H_INCLUDE

#ifdef STRING_BUILDER_IMPLEMENTATION

#include <stdarg.h> // va_list, va_start(), va_arg(), va_end()
#include <string.h> // memcpy(), strlen()
#include <stddef.h> // size_t, NULL

#if _WIN32
#include <windows.h> // VirtualAlloc(), VirtualFree()

void *sb__malloc(size_t size)
{
    return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void sb__free(void *ptr)
{
    VirtualFree(ptr, sizeof(ptr), MEM_RELEASE);
}

#else
#include <stdlib.h> // malloc(), free()

#define sb__malloc malloc
#define sb__free   free
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
    if (!sb)
        return;

    while (sb->first_buffer.next) {
        Sb_Buffer *last = sb->first_buffer.next;
        Sb_Buffer *prev = sb->first_buffer.next;
        while (last->next) {
            prev = last;
            last = last->next;
        }
        sb__free(last);
        prev->next = NULL;
    }
}

static int sb__expand(String_Builder *sb) {
    if (!sb)
        return 0;

    sb->last_buffer->next = (Sb_Buffer *)sb__malloc(sizeof(Sb_Buffer));
    if (!sb->last_buffer->next)
        return 0;

    sb->last_buffer = sb->last_buffer->next;
    sb->last_buffer->length = 0;
    sb->last_buffer->next = NULL;
    sb->number_of_buffers++;

    return 1;
}

SB__PUBLICDEF void SB_DECORATE(append_len)(String_Builder *sb, char *string, int length)
{
    if (!sb)
        return;

    char *cursor = string;

    // We might be given a string that is bigger than a single remaining or even empty bucket.
    // Check if that is the case. If it is, cut up the string into the biggest piece
    // that can fit into the last bucket. Create a new bucket after that and repeat
    // until the remaining string is smaller than the capacity of the last bucket.
    int remaining_space = SB_BUFFER_CAPACITY - sb->last_buffer->length;
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

// See the function delarations for an explanation for this.
#ifdef __cplusplus
#ifdef SB_STATIC
static void SB_DECORATE(append)(String_Builder *sb, char *string, int length)
{
    SB_DECORATE(append_len)(sb, string, length);
}
static void SB_DECORATE(append)(String_Builder *sb, char *string)
#else
void SB_DECORATE(append)(String_Builder *sb, char *string, int length)
{
    SB_DECORATE(append_len)(sb, string, length);
}
void SB_DECORATE(append)(String_Builder *sb, char *string)
#endif
#else
SB__PUBLICDEF void SB_DECORATE(append)(String_Builder *sb, char *string)
#endif
{
    int length = strlen(string);
    SB_DECORATE(append_len)(sb, string, length);
}

SB__PUBLICDEF void SB_DECORATE(vappendf)(String_Builder *sb, char *format, va_list va)
{
    while (*format) {
        int pos = 0;
        while (format[pos] != '%' && format[pos] != '\0') {
            ++pos;
        }
        SB_DECORATE(append_len)(sb, format, pos);
        format += pos + 1;

        switch (*format) {
        case 's':
            SB_DECORATE(append)(sb, va_arg(va, char *));
            break;
        case 'd':
            char val[20];
            itoa(va_arg(va, int), val, 10);
            SB_DECORATE(append)(sb, val);
            break;
        case '%':
            SB_DECORATE(append_len)(sb, "%", 1);
        }
        ++format;
    }
}

SB__PUBLICDEF void SB_DECORATE(appendf)(String_Builder *sb, char *format, ...)
{
    va_list va;
    va_start(va, format);
    SB_DECORATE(vappendf)(sb, format, va);
    va_end(va);
}

SB__PUBLICDEF int SB_DECORATE(to_string)(String_Builder *sb, char **string)
{
    if (!sb)
        return 0;

    int total_length = sb->first_buffer.length;
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

    return total_length;
}

#endif // STRING_BUILDER_IMPLEMENTATION
