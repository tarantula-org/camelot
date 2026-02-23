/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "tests.h"

extern void test_types();
extern void test_memory();
extern void test_io();
extern void test_files();
extern void test_ds();

int main() {
	printf("\n[CAMELOT] RUNNING VERIFICATION SUITE\n");
	printf("--------------------------------------\n");

	test_types();
	test_memory();
	test_io();
	test_files();
	test_ds();

	printf("--------------------------------------\n");
	printf("Tests Run:    %d\n", tests_run);
	printf("Tests Failed: %d\n", tests_failed);

	if (tests_failed == 0) {
		printf("[V] SYSTEM INTEGRITY VERIFIED.\n");
		return 0;
	} else {
		printf("[X] INTEGRITY VIOLATION DETECTED.\n");
		return 1;
	}
}