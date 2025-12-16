#include "tests.h"
#include "camelot/io.h"

TEST(test_io_visual) {
      // This test always "passes" logic-wise, but requires human eyes
      // to verify the output formatting.
      
      print("\n      [VISUAL CHECK START]\n");
      
      // 1. Integers
      print("      INT expect 42         -> %i\n", 42);
      print("      INT expect -99        -> %i\n", -99);
      
      // 2. Floats (High Precision)
      print("      FLT expect 3.14159    -> %f\n", 3.14159);
      print("      FLT expect 1.5e-10    -> %f\n", 1.5e-10);
      
      // 3. Strings
      String s = stringify("Camelot String");
      print("      STR expect 'Hello'    -> %s\n", "Hello"); // %s (char*)
      print("      STR expect 'Camelot..'-> %S\n", s);       // %S (String)

      print("      [VISUAL CHECK END]\n");
}