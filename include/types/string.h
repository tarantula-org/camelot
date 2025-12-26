#ifndef CAMELOT_STRING_H
#define CAMELOT_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "primitives.h"
#include "camelot/memory.h"

// A Slice-based String View (Pointer + Length).
// Immutable and non-owning by default.
typedef struct {
      u8 *ptr;
      u64 len;
} String;

// --- NAMESPACE ---

typedef struct {
      // Wraps a C-String constant into a Camelot String.
      // Usage:
      // ```
      // String s = string.from("Hello");
      // ```
      String (*from)(const char *c_str);

      // Concatenates two strings into a new buffer on the Arena.
      // Usage:
      // ```
      // String combined = string.join(&ctx, s1, s2);
      // ```
      String (*join)(Arena *a, String s1, String s2);

      // Compares two strings for equality.
      // Usage:
      // ```
      // if (string.equal(s1, s2)) { ... }
      // ```
      bool (*equal)(String a, String b);
} StringNamespace;

extern const StringNamespace string;

#ifdef __cplusplus
}
#endif

#endif