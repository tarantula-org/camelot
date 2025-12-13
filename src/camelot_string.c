#include "../include/camelot.h"
#include <stdarg.h>

// --- IMPLEMENTATION: STRINGS ---

Str c_str(const char *raw) {
      if (raw == NULL) {
            return (Str){ .ptr = "", .len = 0 };
      }
      return (Str){ .ptr = raw, .len = strlen(raw) };
}

bool c_str_eq(Str a, Str b) {
      if (a.len != b.len) return false;
      return memcmp(a.ptr, b.ptr, a.len) == 0;
}

Str c_str_fmt(Arena *a, const char *fmt, ...) {
      va_list args;
      
      // 1. Calculate required length
      va_start(args, fmt);
      int len = vsnprintf(NULL, 0, fmt, args);
      va_end(args);

      if (len < 0) {
            return (Str){ .ptr = "", .len = 0 };
      }

      size_t size = (size_t)len + 1; // +1 for null terminator

      // 2. Allocate in Arena
      AllocResult res = c_alloc(a, size, 1);
      if (res.error != C_OK) {
            return (Str){ .ptr = "", .len = 0 };
      }

      char *buffer = (char *)res.unwrap;

      // 3. Write formatted string
      va_start(args, fmt);
      vsnprintf(buffer, size, fmt, args);
      va_end(args);

      return (Str){ .ptr = buffer, .len = (size_t)len };
}