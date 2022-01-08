#include "../unit_test.h"

#define STRING_BUILDER_IMPLEMENTATION
#include "../string_builder.h"

#include <string.h>

TEST_CASE(empty)
{
    String_Builder test_string_builder;
    sb_init(&test_string_builder);

    EXPECT(sb_is_empty(&test_string_builder));
}

TEST_CASE(append)
{
    String_Builder test_string_builder;
    sb_init(&test_string_builder);

    char c = 'A';
    const char *buffer = "ABCDEF";
    sb_append_len(&test_string_builder, &c, 1);
    sb_append(&test_string_builder, "BCDEF");

    EXPECT_EQ(test_string_builder.first_buffer.data[0], 'A');
    EXPECT(strcmp(buffer, "ABCDEF") == 0);
}

TEST_CASE(to_string)
{
    String_Builder test_string_builder;
    sb_init(&test_string_builder);

    char *buffer;
    sb_append(&test_string_builder, "ABCDEF");
    sb_to_string(&test_string_builder, &buffer);

    EXPECT(strcmp(buffer, "ABCDEF") == 0);
}

int main(void)
{
    REGISTER_TEST_CASE(empty);
    REGISTER_TEST_CASE(append);
    REGISTER_TEST_CASE(to_string);

    EXECUTE_TESTS();

    return 0;
}
