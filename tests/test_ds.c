#include "tests.h"
#include "camelot.h"

TEST(test_paged_list) {
      u8 buffer[1024 * 10];
      Arena a;
      hook(&a, buffer, sizeof(buffer));

      // 1. Safe "create" Syntax
      List nums = list.create(&a, sizeof(int));

      // 2. Push 300 items
      for (int i = 0; i < 300; i++) {
            list.push(&nums, &i);
      }

      REQUIRE(nums.count == 300);

      // 3. Verify Page Boundary Crossing
      int *val1 = list.get(&nums, 10);
      REQUIRE(*val1 == 10);

      int *val2 = list.get(&nums, 260); // 260 is in the second page
      REQUIRE(*val2 == 260);

      // 4. Test Remove
      list.remove(&nums, 10);
      int *new_val1 = list.get(&nums, 10);
      
      REQUIRE(*new_val1 == 299); // Swapped with last
      REQUIRE(nums.count == 299);
}

TEST(test_hash_table) {
      u8 buffer[4096];
      Arena a;
      hook(&a, buffer, sizeof(buffer));

      // 1. Create
      Table cfg = table.create(&a, 16);

      int val1 = 100;
      int val2 = 200;

      // 2. Put
      table.put(&cfg, string.from("Width"), &val1);
      table.put(&cfg, string.from("Height"), &val2);

      // 3. Get
      int *got1 = table.get(&cfg, string.from("Width"));
      REQUIRE(got1 != NULL);
      REQUIRE(*got1 == 100);

      int *got2 = table.get(&cfg, string.from("Height"));
      REQUIRE(*got2 == 200);

      // 4. Missing
      int *missing = table.get(&cfg, string.from("Depth"));
      REQUIRE(missing == NULL);

      // 5. Overwrite
      int val3 = 999;
      table.put(&cfg, string.from("Width"), &val3);
      int *got3 = table.get(&cfg, string.from("Width"));
      REQUIRE(*got3 == 999);
}