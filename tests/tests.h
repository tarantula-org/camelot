/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>

#include "camelot.h"

#define ALLOW_UNSAFE

// Global Test State
static int tests_run = 0;
static int tests_failed = 0;

#define TEST(name) static void name()

/*
 * MACRO: REQUIRE
 * Usage: REQUIRE(x == 1);
 * Effect: Prints failure and increments counter if condition is false.
 */
#define REQUIRE(condition)                                                                         \
	do {                                                                                           \
		if (!(condition)) {                                                                        \
			printf("FAILED: [%s] at %s:%d\n", #condition, __FILE__, __LINE__);                     \
			tests_failed++;                                                                        \
		}                                                                                          \
	} while (0)

/*
 * MACRO: RUN
 * Usage: RUN(test_name);
 * Effect: Executes the test function and tracks stats.
 */
#define RUN(test)                                                                                  \
	do {                                                                                           \
		printf("Running %s...\n", #test);                                                          \
		test();                                                                                    \
		tests_run++;                                                                               \
	} while (0)

#endif