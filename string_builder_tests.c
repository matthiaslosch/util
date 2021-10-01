#define STRING_BUILDER_IMPLEMENTATION
#include "string_builder.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

int main(void)
{
    String_Builder sb;
    sb_init(&sb);

    // Test if sb_is_empty() works.
    int empty = sb_is_empty(&sb);
    assert(empty);

    // Test if appending a single char works.
    char c = 'c';
    sb_append_len(&sb, &c, 1);
    assert(sb.first_buffer.data[0] == 'c');

    // Test if printing the string buffer to a pointer works.
    char *buf1;
    sb_to_string(&sb, &buf1);
    assert(strcmp(buf1, "c") == 0);

    // Test if appending a string works.
    sb_append(&sb, ", foo");
    char *buf2;
    sb_to_string(&sb, &buf2);
    assert(strcmp(buf2, "c, foo") == 0);

    // Test if appending via a format string works.
    char *string = "string";
    int positive = 123;
    int negative = -123;
    sb_appendf(&sb, ", %c, %s, %d, %d", c, string, positive, negative);
    char *buf3;
    sb_to_string(&sb, &buf3);
    assert(strcmp(buf3, "c, foo, c, string, 123, -123") == 0);

    // Test if dynamically allocating a new buffer works.
    for (int i = 0; i < SB_BUFFER_CAPACITY; ++i)
        sb_append_len(&sb, "c", 1);
    assert(sb.number_of_buffers > 1);
    assert(&sb.first_buffer != sb.last_buffer);

    // Test if freeing all dynamically allocated buffers works.
    sb_free(&sb);
    assert(sb.first_buffer.next == NULL);

    // C++-specific tests
#ifdef __cplusplus
    // Test if the overload of sb_append() works.
    String_Builder sb_cpp;
    sb_init(&sb_cpp);
    sb_append(&sb_cpp, "This is a test", sizeof("This is a test")-1);
    char *buf4;
    sb_to_string(&sb_cpp, &buf4);
    assert(strcmp(buf4, "This is a test") == 0);

    puts("Tests passed for C++.");
    return 0;
#else
    puts("Tests passed for C.");
    return 0;
#endif
}
