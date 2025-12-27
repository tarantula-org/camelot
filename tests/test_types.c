/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by The Architectural Rigor Standard (ARS-1.0).
 * Compliance is mandatory for all contributions.
*/

#include "tests.h"
#include "camelot.h"
#include <string.h>

TEST(test_string_construction) {
      // 1. Manual
      u8 raw[] = "Hello";
      String s = { raw, 5 };
      REQUIRE(s.len == 5);

      // 2. Namespace Construction
      String s2 = string.from("Camelot");
      REQUIRE(s2.len == 7);
      REQUIRE(memcmp(s2.ptr, "Camelot", 7) == 0);
}