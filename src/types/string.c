#include <string.h>
#include "camelot.h"

// --- INTERNAL HELPERS ---

static String internal_from(const char* c) {
      if (!c) return (String){0};
      return (String){
            .ptr = (u8*)c,
            .len = strlen(c)
      };
}

static bool internal_equal(String a, String b) {
      if (a.len != b.len) return false;
      if (a.ptr == b.ptr) return true;
      return memcmp(a.ptr, b.ptr, a.len) == 0;
}

// --- PUBLIC NAMESPACE ---

const StringNamespace string = {
      .from = internal_from,
      .equal   = internal_equal
};