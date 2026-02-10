/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by the Avant Systems Canon (ASC-1.3).
 * Compliance is mandatory for all contributions.
 */

// clang-format off
#include <fcntl.h> // open, O_RDONLY
#include <stdio.h>
#include <string.h>
#include <unistd.h> // close
#include "camelot.h"
#include "tests.h"
// clang-format on

// --- HELPERS (For Scan Tests) ---

static void mock_stdin(const char *content) {
	FILE *f = fopen("test_stdin.tmp", "wb");
	if (f) {
		fwrite(content, 1, strlen(content), f);
		fclose(f);
	}

	fflush(stdout);
	close(0);
	int fd = open("test_stdin.tmp", O_RDONLY);

	if (fd != 0 && fd != -1) {
		dup2(fd, 0);
		close(fd);
	}
}

static void cleanup_stdin() {
	remove("test_stdin.tmp");
}

// --- UNIT TESTS (Logic) ---

TEST(test_scan_basic) {
	mock_stdin("CamelotUser\n");

	// Arena > 4096 (Default Scan Buffer)
	Arena a = arena.create(8192);

	String s = io.scan(&a, 0);

	REQUIRE(s.len == 11);
	REQUIRE(string.equal(s, string.from("CamelotUser")));

	arena.release(&a);
	cleanup_stdin();
}

TEST(test_scan_limited) {
	mock_stdin("OverflowMe");

	Arena a = arena.create(1024);

	// Explicit limit (5 bytes)
	String s = io.scan(&a, 5);

	REQUIRE(s.len == 4);
	REQUIRE(memcmp(s.ptr, "Over", 4) == 0);

	arena.release(&a);
	cleanup_stdin();
}

// --- VISUAL CHECK ---

TEST(test_io_visual) {
	io.print("\n================ [ VISUAL SYSTEM CHECK ] ================\n");

	// 1. PRIMITIVES
	io.print("\n[1] PRIMITIVES\n");
	io.print("    Integer (42)       : %i\n", 42);
	io.print("    Negative (-999)    : %i\n", -999);
	io.print("    Float (PI)         : %f\n", 3.14159265);

	// 2. STRINGS
	io.print("\n[2] STRINGS\n");
	String s = string.from("Camelot String View");
	io.print("    %%S (Camelot)       : %S\n", s);
	io.print("    %%s (Standard C)    : %s\n", "Legacy C String");

	// 3. MEMORY
	io.print("\n[3] MEMORY\n");
	Arena scratch = arena.create(4096);
	io.print("    Arena Created      : Cap %i bytes\n", (int)scratch.cap);
	int *num = arena.alloc(&scratch, sizeof(int));
	*num = 12345;
	io.print("    Allocated int      : %i (Used %i bytes)\n", *num, (int)scratch.len);

	// 4. DATA STRUCTURES (List)
	io.print("\n[4] PAGED LIST\n");
	List l = list.create(&scratch, sizeof(int));
	for (int i = 1; i <= 3; i++) {
		int val = i * 10;
		list.push(&l, &val);
	}

	for (u64 i = 0; i < l.count; i++) {
		int *val = list.get(&l, i);
		io.print("    List[%i]            : %i\n", (int)i, *val);
	}

	// 5. DATA STRUCTURES (Hash Table)
	io.print("\n[5] HASH TABLE\n");
	Table t = table.create(&scratch, 16);

	int health = 100;
	int mana = 50;

	table.put(&t, string.from("Health"), &health);
	table.put(&t, string.from("Mana"), &mana);

	int *h_ptr = table.get(&t, string.from("Health"));
	int *m_ptr = table.get(&t, string.from("Mana"));

	io.print("    Map['Health']      : %i\n", h_ptr ? *h_ptr : -1);
	io.print("    Map['Mana']        : %i\n", m_ptr ? *m_ptr : -1);

	arena.release(&scratch);
	io.print("\n================ [ CHECK COMPLETE ] ================\n");
}

void test_io() {
	RUN(test_scan_basic);
	RUN(test_scan_limited);
	// RUN(test_io_visual); // Optional: Uncomment to see output
}