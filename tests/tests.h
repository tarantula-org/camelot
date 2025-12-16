#ifndef CAMELOT_TESTS_H
#define CAMELOT_TESTS_H

// 1. UNLOCK SAFETY
// We define this so the tests are allowed to use 'printf'
// to report success/failure.
#define ALLOW_UNSAFE 

#include <stdio.h>
#include <stdbool.h>

// Global counters
static int tests_run = 0;
static int tests_failed = 0;

// --- MACROS ---

#define TEST(name) void name()

#define REQUIRE(condition) \
    do { \
        if (!(condition)) { \
            printf("FAILED: [%s] at %s:%d\n", #condition, __FILE__, __LINE__); \
            tests_failed++; \
        } \
    } while(0)

#define RUN(test) \
    do { \
        printf("Running %s...\n", #test); \
        test(); \
        tests_run++; \
    } while(0)

#endif