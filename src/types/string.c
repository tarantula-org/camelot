/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by The Architectural Rigor Standard (ARS-1.0).
 * Compliance is mandatory for all contributions.
*/

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

static String internal_join(Arena *a, String s1, String s2) {
      u64 new_len = s1.len + s2.len;
      // Allocate new memory from the Arena
      u8 *buf = arena.alloc(a, new_len + 1); 
      
      if (!buf) return (String){0};

      // Copy S1
      memcpy(buf, s1.ptr, s1.len);
      // Copy S2
      memcpy(buf + s1.len, s2.ptr, s2.len);
      
      // Null terminate for safety/compatibility
      buf[new_len] = '\0';

      return (String){ .ptr = buf, .len = new_len };
}

static bool internal_equal(String a, String b) {
      if (a.len != b.len) return false;
      if (a.ptr == b.ptr) return true;
      return memcmp(a.ptr, b.ptr, a.len) == 0;
}

// --- PUBLIC NAMESPACE ---

const StringNamespace string = {
      .from  = internal_from,
      .join  = internal_join,
      .equal = internal_equal
};