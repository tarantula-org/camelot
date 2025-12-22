#include "tests.h"
#include "camelot.h"
#include <stdio.h>
#include <string.h>

static void setup_file(const char *name, const char *content) {
      FILE *f = fopen(name, "wb");
      if (f) { fputs(content, f); fclose(f); }
}

static void teardown_file(const char *name) {
      remove(name);
}

TEST(test_slurp_basic) {
      Arena a = arena.create(1024);

      const char *fname = "test_slurp.txt";
      setup_file(fname, "Camelot FS");

      String s = io.slurp(&a, fname);

      REQUIRE(s.ptr != NULL);
      REQUIRE(s.len == 10);
      REQUIRE(memcmp(s.ptr, "Camelot FS", 10) == 0);

      teardown_file(fname);
      arena.release(&a);
}

TEST(test_stream_dispatch) {
      const char *fname = "test_stream.txt";
      setup_file(fname, "0123456789");

      File f = {0};
      u64 res = io.stream(&f, OPEN, (void*)fname, 0);
      REQUIRE(res == 1);
      
      char buf[6] = {0};
      io.stream(&f, SKIP, NULL, 5);
      io.stream(&f, READ, buf, 5);
      REQUIRE(buf[0] == '5');

      io.stream(&f, CLOSE, NULL, 0);
      teardown_file(fname);
}

TEST(test_missing_file) {
      Arena a = arena.create(1024);

      String s = io.slurp(&a, "ghost_file.xyz");
      REQUIRE(s.ptr == NULL);

      arena.release(&a);
}