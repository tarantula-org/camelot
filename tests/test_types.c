#include "tests.h"
#include "types/string.h"
#include <string.h> // for memcmp

TEST(test_string_construction) {
      // 1. Manual Construction
      u8 raw[] = "Hello";
      String s = { raw, 5 };

      REQUIRE(s.len == 5);
      REQUIRE(s.ptr[0] == 'H');
      REQUIRE(s.ptr[4] == 'o');
}

TEST(test_stringify_macro) {
      // 2. Macro Construction (compile-time length)
      String s = stringify("Camelot");

      REQUIRE(s.len == 7);
      REQUIRE(s.ptr != NULL);
      
      // Verify content matches
      REQUIRE(memcmp(s.ptr, "Camelot", 7) == 0);
}