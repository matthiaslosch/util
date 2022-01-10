#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h> // PRId64

#if defined(_WIN32) || defined(WIN32)
#include <windows.h> // QueryPerformanceFrequency(), QueryPerformanceCounter()
#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__)) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#include <sys/time.h> // struct timeval, gettimeofday(), timersub()
#else
#error "The OS you're using is currently not supported by Unit Test."
#endif

struct Test_Case_List {
    const char *test_case_name;
    void (*test_case_function)(void);
    struct Test_Case_List *next;
};

static struct Test_Case_List *list_head = NULL;
static struct Test_Case_List *list_tail = NULL;
static int number_of_test_cases = 0;

int failed_current_test_case = 0;

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
    ++number_of_test_cases;

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
            failed_current_test_case = 1;                                                                  \
        }                                                                                                  \
    } while (0)

#define EXPECT_NE(a, b)                                                                                               \
    do {                                                                                                              \
        if ((a) == (b)) {                                                                                             \
            fprintf(stderr, "\033[31;1mFAIL\033[0m: %s:%d: EXPECT_NE(%s, %s) failed.\n", __FILE__, __LINE__, #a, #b); \
            failed_current_test_case = 1;                                                                             \
        }                                                                                                             \
    } while (0)

#define EXPECT_EQ(a, b)                                                                                               \
    do {                                                                                                              \
        if ((a) != (b)) {                                                                                             \
            fprintf(stderr, "\033[31;1mFAIL\033[0m: %s:%d: EXPECT_EQ(%s, %s) failed.\n", __FILE__, __LINE__, #a, #b); \
            failed_current_test_case = 1;                                                                             \
        }                                                                                                             \
    } while (0)

struct Test_Timer {
#if defined(_WIN32) || defined(WIN32)
    int64_t start_counter;
#else
    struct timeval start_counter;
#endif
};

struct Test_Timer start_timer()
{
    struct Test_Timer result;
#if defined(_WIN32) || defined(WIN32)
    LARGE_INTEGER start_counter;
    QueryPerformanceCounter(&start_counter);
    result.start_counter = start_counter.QuadPart;
    return result;
#else
    struct timeval start_counter;
    gettimeofday(&start_counter, NULL);
    result.start_counter = start_counter;
    return result;
#endif
}

int64_t elapsed_milliseconds(struct Test_Timer timer)
{
#if defined(_WIN32) || defined(WIN32)
    LARGE_INTEGER perf_counter_frequency;
    if (!QueryPerformanceFrequency(&perf_counter_frequency))
        fprintf(stderr, "QueryPerformanceFrequency failed.\n");
    LARGE_INTEGER end_counter;
    QueryPerformanceCounter(&end_counter);
    int64_t counter_elapsed = (end_counter.QuadPart - timer.start_counter) / perf_counter_frequency.QuadPart * 1000;
#else
    struct timeval end_counter;
    gettimeofday(&end_counter, NULL);
    struct timeval counter_delta;
    timersub(&end_counter, &timer.start_counter, &counter_delta);
    int64_t counter_elapsed = counter_delta.tv_sec * 1000 + counter_delta.tv_usec / 1000;
#endif
    return counter_elapsed;
}

void execute_test_case(struct Test_Case_List *current)
{
    printf("Running test '%s'.\n", current->test_case_name);
    struct Test_Timer timer = start_timer();
    current->test_case_function();
    int64_t counter_elapsed = elapsed_milliseconds(timer);
    printf("%s test '%s' in %" PRId64 " ms.\n", failed_current_test_case ? "\033[31;1mFailed\033[0m" : "\033[32;1mCompleted\033[0m", current->test_case_name, counter_elapsed);
}

void execute_all_test_cases()
{
    int failed = 0;
    struct Test_Timer global_timer = start_timer();
    while (list_head) {
        execute_test_case(list_head);
        list_head = list_head->next;
        if (failed_current_test_case)
            ++failed;
        failed_current_test_case = 0;
    }
    int64_t global_time_elapsed = elapsed_milliseconds(global_timer);
    printf("Finished %d tests in %" PRId64 " ms.\n", number_of_test_cases, global_time_elapsed);
    printf("Out of %d tests, %d %s and %d %s.\n", number_of_test_cases, number_of_test_cases - failed, number_of_test_cases - failed ? "\033[32;1mpassed\033[0m" : "passed", failed, failed ? "\033[31;1mfailed\033[0m" : "failed");
}

#define EXECUTE_TESTS() \
    execute_all_test_cases()

#endif // !UNIT_TEST_H
