#include "camelot/camelot.h"

int main() {
      // 1. Setup Memory
      u8 buffer[1024 * 1024]; // 1MB Stack Buffer
      Arena mem;
      hook(&mem, buffer, sizeof(buffer));

      // 2. Setup Data
      String title = stringify("Camelot Systems Engine");
      int *nums = (int*)allocate(&mem, 3 * sizeof(int));
      if (!nums) return 1;
      nums[0] = 10; nums[1] = 20; nums[2] = 30;

      // 3. Test The Voice (I/O)
      print("\n--- %S ---\n", title);
      
      // Basic Types
      print("Integers:   %i, %i, %i\n", nums[0], nums[1], nums[2]);
      print("C-String:   %s\n", "Hello World");
      
      // float/double Tests (The new stuff!)
      print("Precision:  %f\n", 3.14159265359);
      print("Scientific: %f\n", 6.022e23);  // Avogadro's number
      print("Tiny:       %f\n", 1.6e-19);   // Electron charge

      // Memory Status
      print("\n[Mem] Used: %i / %i bytes\n", mem.len, mem.cap);

      return 0;
}