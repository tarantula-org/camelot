#ifndef CAMELOT_STRING_H
#define CAMELOT_STRING_H

#include "primitives.h"
#include "camelot/memory.h"   // Needed for Arena*

// --- TYPES ---

typedef struct {
      u8 *ptr;
      u64 len;
} String;

// --- NAMESPACE ---

typedef struct {
      // Wraps a null-terminated C-string.
      String (*from)(const char *c_str);

      // Concatenates two strings into a new one allocated on the Arena.
      // Usage: String s = string.join(&arena, s1, s2);
      String (*join)(Arena *a, String s1, String s2);

      // Checks if two strings are identical.
      bool (*equal)(String a, String b);
} StringNamespace;

extern const StringNamespace string;

#endif