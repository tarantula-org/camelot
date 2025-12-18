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