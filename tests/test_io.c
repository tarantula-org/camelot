#include "tests.h"
#include "camelot.h"

TEST(test_io_visual) {
      io.print("\n      [VISUAL CHECK START]\n");
      
      io.print("      INT expect 42         -> %i\n", 42);
      
      String s = string.from("Camelot String");
      io.print("      STR expect 'Camelot..'-> %S\n", s);

      io.print("      [VISUAL CHECK END]\n");
}