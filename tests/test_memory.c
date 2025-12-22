#include "tests.h"
#include "camelot.h"
#include <stdint.h> 

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