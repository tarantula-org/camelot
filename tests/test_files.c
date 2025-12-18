#include "tests.h"
#include "camelot.h"
#include <stdio.h>
#include <string.h>

// --- TEST HELPERS ---
static void setup_file(const char *name, const char *content) {
      FILE *f = fopen(name, "wb");
      if (f) {
            fputs(content, f);
            fclose(f);
      }
}

static void teardown_file(const char *name) {
      remove(name);
}

// --- TESTS ---

TEST(test_slurp_basic) {
      // Setup
      u8 raw_mem[1024];
      Arena a;
      hook(&a, raw_mem, 1024);

      const char *fname = "test_slurp.txt";
      const char *content = "Camelot FS";
      setup_file(fname, content);

      // Act
      String s = io.slurp(&a, fname);

      // Assert
      REQUIRE(s.ptr != NULL);
      REQUIRE(s.len == 10);
      REQUIRE(memcmp(s.ptr, "Camelot FS", 10) == 0);

      // Cleanup
      teardown_file(fname);
}

TEST(test_stream_dispatch) {
      const char *fname = "test_stream.txt";
      const char *content = "0123456789";
      setup_file(fname, content);

      File f = {0};

      // 1. OPEN
      u64 res = io.stream(&f, OPEN, (void*)fname, 0);
      REQUIRE(res == 1);
      REQUIRE(f.status == OK);

      // 2. SKIP
      io.stream(&f, SKIP, NULL, 5);

      // 3. READ
      char buf[6] = {0};
      io.stream(&f, READ, buf, 5);
      REQUIRE(buf[0] == '5');

      // 4. CLOSE
      io.stream(&f, CLOSE, NULL, 0);
      REQUIRE(f.handle == NULL);

      teardown_file(fname);
}

TEST(test_missing_file) {
      u8 raw_mem[1024];
      Arena a;
      hook(&a, raw_mem, 1024);

      String s = io.slurp(&a, "ghost_file.xyz");
      REQUIRE(s.ptr == NULL);
}