#include "tests.h"
#include <camelot/memory.h>
#include <stdint.h> // for uintptr_t

TEST(test_alignment) {
      u8 buffer[1024];
      Arena arena; // Stack allocation
      hook(&arena, buffer, 1024); // Mutation pass-by-reference

      void *p1 = allocate(&arena, sizeof(u8));
      void *p2 = allocate(&arena, sizeof(u64));

      // 1. Use REQUIRE to check for NULL
      REQUIRE(p1 != NULL);
      REQUIRE(p2 != NULL);

      // 2. The Math
      // We cast the pointer 'p2' to a number 'addr' so we can do math on it.
      uintptr_t addr = (uintptr_t)p2;
      
      // We REQUIRE the remainder to be 0 (perfectly divisible by 8)
      REQUIRE((addr % 8) == 0);
}

TEST(test_oom) {
      u8 buffer[16]; // A tiny buffer (16 bytes total)
      Arena a;
      hook(&a, buffer, 16);

      // 1. Allocate 10 bytes.
      // This fits easily.
      void *p1 = allocate(&a, 10);
      REQUIRE(p1 != NULL);
      REQUIRE(a.status == OK);

      // 2. Allocate 10 more bytes.
      // 10 used + 10 new = 20. This is > 16.
      // Should fail and return NULL.
      void *p2 = allocate(&a, 10);
      REQUIRE(p2 == NULL);

      // 3. Verify the Sticky Error
      // The arena should now be in a poisoned state.
      REQUIRE(a.status == OOM);
}