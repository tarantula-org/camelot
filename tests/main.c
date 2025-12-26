#include "tests.h"

// Unity Build: Include the test logic directly
#include "test_memory.c"
#include "test_types.c"
#include "test_io.c"
#include "test_files.c"
#include "test_ds.c"

int main() {
      printf("--- CAMELOT TEST SUITE ---\n");

      // 1. Run Logic Tests (Memory & Types)
      RUN(test_alignment);
      RUN(test_oom);
      RUN(test_workspace_macro); // Updated name
      RUN(test_string_construction);
      RUN(test_paged_list);
      RUN(test_hash_table);

      // 2. Run File System Tests
      RUN(test_slurp_basic);
      RUN(test_stream_dispatch);
      RUN(test_missing_file);

      // 3. IO & Scanning Tests
      RUN(test_scan_basic);
      RUN(test_scan_limited);
      RUN(test_io_visual);

      // 4. Report
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