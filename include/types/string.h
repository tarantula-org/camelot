#ifndef CAMELOT_STRING_H
#define CAMELOT_STRING_H

#include "primitives.h"

typedef struct {
      u8 *ptr;
      u64 len;
} String;

// --- FUNCTIONS ---

// Wraps a null-terminated C-string into a Camelot String.
String stringify(const char* str);

// Checks if two strings are identical in content and length.
bool str_eq(String a, String b);

#endif