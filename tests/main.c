#include "tests.h"

#include "test_memory.c"

int main() {
      printf("--- CAMELOT TEST SUITE ---\n");

      RUN(test_alignment);
      RUN(test_oom);

      printf("--------------------------\n");
      printf("Tests Run:    %d\n", tests_run);
      printf("Tests Failed: %d\n", tests_failed);

      if (tests_failed == 0) {
            printf("PASSED!\n");
      } else {
            printf("FAILED!\n");
      }

      return tests_failed;
}