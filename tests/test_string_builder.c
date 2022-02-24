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

TEST_CASE(append_number)
{
    String_Builder test_string_builder;
    sb_init(&test_string_builder);

    // Test sb_append_u*

    unsigned char int_8_bit_unsigned  = 255; // 2^8 - 1
    unsigned short int_16_bit_unsigned = 65535; // 2^16 - 1
    unsigned int int_32_bit_unsigned = 4294967295; // 2^32 - 1
    unsigned long long int_64_bit_unsigned = 18446744073709551615; // 2^64 - 1

    sb_append_u8(&test_string_builder, int_8_bit_unsigned);
    sb_append_u16(&test_string_builder, int_16_bit_unsigned);
    sb_append_u32(&test_string_builder, int_32_bit_unsigned);
    sb_append_u64(&test_string_builder, int_64_bit_unsigned);

    unsigned char sb_8_bit_unsigned  = *(unsigned char*)&test_string_builder.first_buffer.data[0];
    unsigned short sb_16_bit_unsigned = *(unsigned short*)&test_string_builder.first_buffer.data[1];
    unsigned int sb_32_bit_unsigned = *(unsigned int*)&test_string_builder.first_buffer.data[3];
    unsigned long long sb_64_bit_unsigned = *(unsigned long long*)&test_string_builder.first_buffer.data[7];

    EXPECT_EQ(sb_8_bit_unsigned, int_8_bit_unsigned);
    EXPECT_EQ(sb_16_bit_unsigned, int_16_bit_unsigned);
    EXPECT_EQ(sb_32_bit_unsigned, int_32_bit_unsigned);
    EXPECT_EQ(sb_64_bit_unsigned, int_64_bit_unsigned);

    // Test sb_append_i* min values

    char int_8_bit_signed_min  = -128; // -(2^7)
    short int_16_bit_signed_min = -32768; // -(2^15)
    int int_32_bit_signed_min = -2147483647 - 1; // -(2^31)
    long long int_64_bit_signed_min = -9223372036854775807 - 1; // -(2^63)

    sb_append_i8(&test_string_builder, int_8_bit_signed_min);
    sb_append_i16(&test_string_builder, int_16_bit_signed_min);
    sb_append_i32(&test_string_builder, int_32_bit_signed_min);
    sb_append_i64(&test_string_builder, int_64_bit_signed_min);

    char sb_8_bit_signed_min  = *(char*)&test_string_builder.first_buffer.data[15];
    short sb_16_bit_signed_min = *(short*)&test_string_builder.first_buffer.data[16];
    int sb_32_bit_signed_min = *(int*)&test_string_builder.first_buffer.data[18];
    long long sb_64_bit_signed_min = *(long long*)&test_string_builder.first_buffer.data[22];

    EXPECT_EQ(sb_8_bit_signed_min, int_8_bit_signed_min);
    EXPECT_EQ(sb_16_bit_signed_min, int_16_bit_signed_min);
    EXPECT_EQ(sb_32_bit_signed_min, int_32_bit_signed_min);
    EXPECT_EQ(sb_64_bit_signed_min, int_64_bit_signed_min);

    // Test sb_append_i* max values

    char int_8_bit_signed_max  = 127; // 2^7 - 1
    short int_16_bit_signed_max = 32767; // 2^15 - 1
    int int_32_bit_signed_max = 2147483647; // 2^31 - 1
    long long int_64_bit_signed_max = 9223372036854775807; // 2^63 - 1

    sb_append_i8(&test_string_builder, int_8_bit_signed_max);
    sb_append_i16(&test_string_builder, int_16_bit_signed_max);
    sb_append_i32(&test_string_builder, int_32_bit_signed_max);
    sb_append_i64(&test_string_builder, int_64_bit_signed_max);

    char sb_8_bit_signed_max = *(char*)&test_string_builder.first_buffer.data[30];
    short sb_16_bit_signed_max = *(short*)&test_string_builder.first_buffer.data[31];
    int sb_32_bit_signed_max = *(int*)&test_string_builder.first_buffer.data[33];
    long long sb_64_bit_signed_max = *(long long*)&test_string_builder.first_buffer.data[37];

    EXPECT_EQ(sb_8_bit_signed_max, int_8_bit_signed_max);
    EXPECT_EQ(sb_16_bit_signed_max, int_16_bit_signed_max);
    EXPECT_EQ(sb_32_bit_signed_max, int_32_bit_signed_max);
    EXPECT_EQ(sb_64_bit_signed_max, int_64_bit_signed_max);
}

TEST_CASE(append_string)
{
    String_Builder test_string_builder;
    sb_init(&test_string_builder);

    char c = 'A';
    const char *buffer = "ABCDEF";
    sb_append_len(&test_string_builder, &c, 1);
    sb_append_string(&test_string_builder, "BCDEF");

    EXPECT_EQ(test_string_builder.first_buffer.data[0], 'A');
    EXPECT(strcmp(buffer, "ABCDEF") == 0);
}

TEST_CASE(append_bytes)
{
    String_Builder test_string_builder;
    sb_init(&test_string_builder);

    struct Test_Struct {
        int a;
        char b;
    };

    struct Test_Struct test_struct;
    test_struct.a = 12345;
    test_struct.b = 2;

    sb_append_len(&test_string_builder, &test_struct, sizeof(struct Test_Struct));

    EXPECT(memcmp(test_string_builder.first_buffer.data, &test_struct, sizeof(struct Test_Struct)) == 0);
}

TEST_CASE(to_string)
{
    String_Builder test_string_builder;
    sb_init(&test_string_builder);

    char *buffer;
    sb_append_string(&test_string_builder, "ABCDEF");
    sb_to_string(&test_string_builder, &buffer);

    EXPECT(strcmp(buffer, "ABCDEF") == 0);
}

int main(void)
{
    REGISTER_TEST_CASE(empty);
    REGISTER_TEST_CASE(append_number);
    REGISTER_TEST_CASE(append_string);
    REGISTER_TEST_CASE(append_bytes);
    REGISTER_TEST_CASE(to_string);

    EXECUTE_TESTS();

    return 0;
}
