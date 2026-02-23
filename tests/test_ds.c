/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "tests.h"

// --- LIST TESTS ---

TEST(test_paged_list) {
	Arena a = arena.create(1024 * 10);

	List nums = list.create(&a, sizeof(int));

	for (int i = 0; i < 300; i++) {
		list.push(&nums, &i);
	}

	REQUIRE(nums.count == 300);

	int *val1 = list.get(&nums, 10);
	REQUIRE(val1 != NULL);
	if (val1) {
		REQUIRE(*val1 == 10);
	}

	int *val2 = list.get(&nums, 260);
	REQUIRE(val2 != NULL);
	if (val2) {
		REQUIRE(*val2 == 260);
	}

	list.remove(&nums, 10);
	int *new_val1 = list.get(&nums, 10);

	REQUIRE(new_val1 != NULL);
	if (new_val1) {
		REQUIRE(*new_val1 == 299);
	}
	REQUIRE(nums.count == 299);

	arena.release(&a);
}

// --- TABLE TESTS ---

TEST(test_hash_table) {
	Arena a = arena.create(4096);

	Table cfg = table.create(&a, 16);

	int val1 = 100;
	table.put(&cfg, string.from("Width"), &val1);

	int *got1 = table.get(&cfg, string.from("Width"));
	REQUIRE(got1 != NULL);
	if (got1) {
		REQUIRE(*got1 == 100);
	}

	int *missing = table.get(&cfg, string.from("Depth"));
	REQUIRE(missing == NULL);

	arena.release(&a);
}

void test_ds() {
	RUN(test_paged_list);
	RUN(test_hash_table);
}