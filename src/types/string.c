#include <string.h>
#include "types/string.h"

String stringify(const char* str) {
      return (String){
            .ptr = (u8*)str,
            .len = strlen(str)
      };
}

bool str_eq(String a, String b) {
      if (a.len != b.len) return false;
      if (a.ptr == b.ptr) return true;
      return memcmp(a.ptr, b.ptr, a.len) == 0;
}