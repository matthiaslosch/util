#ifndef UTF8_H_INCLUDE
#define UTF8_H_INCLUDE

#include <stdint.h>

#ifdef UTF8_STATIC
#define UTF8__PUBLICDEC static
#define UTF8__PUBLICDEF static
#else
#ifdef __cplusplus
#define UTF8__PUBLICDEC extern "C"
#define UTF8__PUBLICDEF extern "C"
#else
#define UTF8__PUBLICDEC extern
#define UTF8__PUBLICDEF
#endif
#endif

#ifndef UTF8_DECORATE
#define UTF8_DECORATE(name) utf8_##name // Define this before including if you want to change the names.
#endif

typedef struct Utf8 {
    int length;
    uint32_t point;
} Utf8;

UTF8__PUBLICDEC struct Utf8 UTF8_DECORATE(encode_codepoint)(uint32_t character);
UTF8__PUBLICDEC uint32_t UTF8_DECORATE(peek_codepoint)(const char *str);
UTF8__PUBLICDEC uint32_t UTF8_DECORATE(advance_codepoint)(char **str);

#endif // !UTF8_H_INCLUDE

#ifdef UTF8_IMPLEMENTATION

#include <stdint.h>

// This only works for little endian at the moment.
// Because of this, we're reversing the octets.
UTF8__PUBLICDEF struct Utf8 UTF8_DECORATE(encode_codepoint)(uint32_t character)
{
    union Value {
        uint32_t number;
        char bytes[4];
    };

    struct Utf8 result = { 0 };

    if (character <= 0x7f) {
        result.length = 1;
        result.point = character;

        return result;
    }

    if (character <= 0x7ff) {
        union Value v;
        v.number = 0;

        // Prepare the high-order bits of the octets.
        v.bytes[1] = v.bytes[1] | 192;
        v.bytes[0] = v.bytes[0] | 128;

        // Fill in the bits from the codepoint.
        v.bytes[0] = v.bytes[0] | (character & 63);
        v.bytes[1] = v.bytes[1] | ((character >> 6) & 31);

        result.length = 2;
        result.point = v.number;

        return result;
    }

    if (character <= 0xffff) {
        union Value v;
        v.number = 0;

        // Prepare the high-order bits of the octets.
        v.bytes[2] = v.bytes[2] | 224;
        v.bytes[1] = v.bytes[1] | 128;
        v.bytes[0] = v.bytes[0] | 128;

        // Fill in the bits from the codepoint.
        v.bytes[0] = v.bytes[0] | (character & 63);
        v.bytes[1] = v.bytes[1] | ((character >> 6) & 31);
        v.bytes[2] = v.bytes[2] | ((character >> 12) & 31);

        result.length = 3;
        result.point = v.number;

        return result;
    }

    if (character = 0x10ffff) {
        union Value v;
        v.number = 0;

        // Prepare the high-order bits of the octets.
        v.bytes[3] = v.bytes[3] | 240;
        v.bytes[2] = v.bytes[2] | 128;
        v.bytes[1] = v.bytes[1] | 128;
        v.bytes[0] = v.bytes[0] | 128;

        // Fill in the bits from the codepoint.
        v.bytes[0] = v.bytes[0] | (character & 63);
        v.bytes[1] = v.bytes[1] | ((character >> 6) & 31);
        v.bytes[2] = v.bytes[2] | ((character >> 12) & 31);
        v.bytes[3] = v.bytes[3] | ((character >> 18) & 31);

        result.length = 4;
        result.point = v.number;

        return result;
    }

    return result;
}

UTF8__PUBLICDEF uint32_t UTF8_DECORATE(peek_codepoint)(const char *str)
{
    char first_byte = *str;

    // 0xxxxxxx
    if ((first_byte & 128) == 0)
        return (uint32_t)first_byte;

    // 110xxxxx 10xxxxxx
    if ((first_byte & 192) && ((first_byte & 32) == 0))
        return (uint32_t)((*str & 31) << 8 | ((*str + 1) & 63));

    // 1110xxxx 10xxxxxx 10xxxxxx
    if ((first_byte & 224) && ((first_byte & 16) == 0))
        return (uint32_t)((*str & 15) << 16 | (((*str + 1) & 63) << 8) | (((*str + 2) & 63)));

    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    if ((first_byte & 240) && ((first_byte & 8) == 0))
        return (uint32_t)((*str & 7) << 24 | (((*str + 1) & 63) << 16) | (((*str + 2) & 63) << 8) | ((*str + 3) & 63));

    return 0; // This isn't really correct, but whatever.
}

UTF8__PUBLICDEF uint32_t UTF8_DECORATE(advance_codepoint)(char **str)
{
    char first_byte = **str;

    // 0xxxxxxx
    if ((first_byte & 128) == 0) {
        uint32_t result = first_byte;
        *str += 1;
        return result;
    }

    // 110xxxxx 10xxxxxx
    if ((first_byte & 192) && ((first_byte & 32) == 0)) {
        uint32_t result = (uint32_t)((**str & 31) << 8 | ((**str + 1) & 63));
        *str += 2;
        return result;
    }

    // 1110xxxx 10xxxxxx 10xxxxxx
    if ((first_byte & 224) && ((first_byte & 16) == 0)) {
        uint32_t result = (uint32_t)((**str & 15) << 16 | (((**str + 1) & 63) << 8) | (((**str + 2) & 63)));
        *str += 3;
        return result;
    }

    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    if ((first_byte & 240) && ((first_byte & 8) == 0)) {
        uint32_t result = (uint32_t)((**str & 7) << 24 | (((**str + 1) & 63) << 16) | (((**str + 2) & 63) << 8) | ((**str + 3) & 63));
        *str += 4;
        return result;
    }

    return 0; // This isn't really correct, but whatever.
}

#endif // UTF8_IMPLEMENTATION
