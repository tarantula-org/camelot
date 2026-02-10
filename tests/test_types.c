/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by the Avant Systems Canon (ASC-1.3).
 * Compliance is mandatory for all contributions.
 */

// clang-format off
#include <string.h>
#include "camelot.h"
#include "tests.h"
// clang-format on

TEST(test_string_construction) {
	// 1. Manual
	u8 raw[] = "Hello";
	String s = {raw, 5};
	REQUIRE(s.len == 5);

	// 2. Namespace Construction
	String s2 = string.from("Camelot");
	REQUIRE(s2.len == 7);
	REQUIRE(memcmp(s2.ptr, "Camelot", 7) == 0);

	// 3. Joining
	Arena a = arena.create(1024);
	String joined = string.join(&a, s, s2);
	REQUIRE(joined.len == 12);
	REQUIRE(memcmp(joined.ptr, "HelloCamelot", 12) == 0);

	arena.release(&a);
}

void test_types() {
	RUN(test_string_construction);
}