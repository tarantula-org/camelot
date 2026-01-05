/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by the Avant Systems Canon (ASC-1.1).
 * Compliance is mandatory for all contributions.
 */

// clang-format off
#include <stdint.h>
#include "camelot.h"
#include "tests.h"
// clang-format on

TEST(test_alignment) {
  Arena a = arena.create(1024);

  void *p1 = arena.alloc(&a, sizeof(u8));
  void *p2 = arena.alloc(&a, sizeof(u64));

  REQUIRE(p1 != NULL);
  REQUIRE(p2 != NULL);

  uintptr_t addr = (uintptr_t)p2;
  REQUIRE((addr % 8) == 0);

  arena.release(&a);
}

TEST(test_oom) {
  Arena a = arena.create(16);

  void *p1 = arena.alloc(&a, 10);
  REQUIRE(p1 != NULL);
  REQUIRE(a.status == OK);

  void *p2 = arena.alloc(&a, 10);
  REQUIRE(p2 == NULL);
  REQUIRE(a.status == OOM);

  arena.release(&a);
}

TEST(test_workspace_macro) {
  // Verifies that the 'Workspace' syntax compiles and runs.
  // If the cleanup logic was broken, this might segfault on scope exit.
  {
    Workspace a = arena.create(256);
    REQUIRE(a.cap == 256);

    void *p = arena.alloc(&a, 10);
    REQUIRE(p != NULL);
    // 'a' is automatically released here via __attribute__((cleanup))
  }

  // If we survived the block exit...
}

void test_memory() {
  RUN(test_alignment);
  RUN(test_oom);
  RUN(test_workspace_macro);
}