#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <stdlib.h>
#include <stdio.h>

struct Test_Case_List {
    const char *test_case_name;
    void (*test_case_function)(void);
    struct Test_Case_List *next;
};

static struct Test_Case_List *list_head = NULL;
static struct Test_Case_List *list_tail = NULL;

void add_test_case_to_list(const char *name, void (*function)(void))
{
    if (!list_tail) {
        list_tail = (struct Test_Case_List *)malloc(sizeof(struct Test_Case_List)); // FIXME: Check for malloc result.
    } else {
        list_tail->next = (struct Test_Case_List *)malloc(sizeof(struct Test_Case_List)); // FIXME: Check for malloc result.
        list_tail = list_tail->next;
    }
    list_tail->test_case_name = name;
    list_tail->test_case_function = function;
    list_tail->next = NULL;

    if (!list_head)
        list_head = list_tail;
}

#define TEST_CASE_FUNCTION(x) __test_##x

#define TEST_CASE(x) \
    static void TEST_CASE_FUNCTION(x)(void)

#define REGISTER_TEST_CASE(x) \
    add_test_case_to_list(#x, TEST_CASE_FUNCTION(x));

#define EXPECT(x)                                                                                          \
    do {                                                                                                   \
        if (!(x)) {                                                                                        \
            fprintf(stderr, "\033[31;1mFAIL\033[0m: %s:%d: EXPECT(%s) failed.\n", __FILE__, __LINE__, #x); \
            abort();                                                                                       \
        }                                                                                                  \
    } while (0)

#define EXPECT_NE(a, b)                                                                                               \
    do {                                                                                                              \
        if ((a) == (b)) {                                                                                             \
            fprintf(stderr, "\033[31;1mFAIL\033[0m: %s:%d: EXPECT_NE(%s, %s) failed.\n", __FILE__, __LINE__, #a, #b); \
            abort();                                                                                                  \
        }                                                                                                             \
    } while (0)

#define EXPECT_EQ(a, b)                                                                                               \
    do {                                                                                                              \
        if ((a) != (b)) {                                                                                             \
            fprintf(stderr, "\033[31;1mFAIL\033[0m: %s:%d: EXPECT_EQ(%s, %s) failed.\n", __FILE__, __LINE__, #a, #b); \
            abort();                                                                                                  \
        }                                                                                                             \
    } while (0)

#define EXECUTE_TESTS()                                             \
    while (list_head) {                                             \
        list_head->test_case_function();                            \
        printf("%s %s.\n", "Completed", list_head->test_case_name); \
        list_head = list_head->next;                                \
    }

#endif // !UNIT_TEST_H
