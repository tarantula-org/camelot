#include "tests.h"

// Unity Build: Include the test logic directly
#include "test_memory.c"
#include "test_types.c"
#include "test_io.c"

int main() {
      printf("--- CAMELOT TEST SUITE ---\n");

      // 1. Run Logic Tests (Memory & Types)
      RUN(test_alignment);
      RUN(test_oom);
      RUN(test_string_construction);
      RUN(test_stringify_macro);

      // 2. Run Visual Tests (IO)
      RUN(test_io_visual);

      // 3. Report
      printf("--------------------------\n");
      printf("Tests Run:    %d\n", tests_run);
      printf("Tests Failed: %d\n", tests_failed);

      if (tests_failed == 0) {
            printf("[V] ALL TESTS PASSED\n");
      } else {
            printf("[X] SOME TESTS FAILED\n");
      }

      return tests_failed;
}