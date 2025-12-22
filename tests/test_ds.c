#include "tests.h"
#include "camelot.h"

TEST(test_paged_list) {
      Arena a = arena.create(1024 * 10);

      List nums = list.create(&a, sizeof(int));

      for (int i = 0; i < 300; i++) {
            list.push(&nums, &i);
      }

      REQUIRE(nums.count == 300);

      int *val1 = list.get(&nums, 10);
      REQUIRE(*val1 == 10);

      int *val2 = list.get(&nums, 260);
      REQUIRE(*val2 == 260);

      list.remove(&nums, 10);
      int *new_val1 = list.get(&nums, 10);
      
      REQUIRE(*new_val1 == 299); 
      REQUIRE(nums.count == 299);

      arena.release(&a);
}

TEST(test_hash_table) {
      Arena a = arena.create(4096);

      Table cfg = table.create(&a, 16);

      int val1 = 100;
      table.put(&cfg, string.from("Width"), &val1);

      int *got1 = table.get(&cfg, string.from("Width"));
      REQUIRE(got1 != NULL);
      REQUIRE(*got1 == 100);

      int *missing = table.get(&cfg, string.from("Depth"));
      REQUIRE(missing == NULL);

      arena.release(&a);
}