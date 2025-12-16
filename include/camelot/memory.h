#ifndef CAMELOT_MEMORY_H
#define CAMELOT_MEMORY_H

#include "types/primitives.h"

// A Linear Allocator (Arena).
// It manages a fixed block of memory and allocates by simply moving a pointer forward.
// This is O(1) fast and prevents memory fragmentation.
typedef struct {
      u8 *buf;
      u64 len;
      u64 cap;
      Result status;
} Arena;

// --- FUNCTIONS ---

// Hooks the Arena to a backing buffer.
// The Arena does not own the memory; it just manages it.
void hook(Arena *a, void *buffer, u64 size);

// Reserves a block of memory 'size' bytes long from the Arena.
// Returns NULL if the Arena is full.
void *allocate(Arena *a, u64 size);

// Resets the cursor to zero.
// This effectively "frees" everything in the Arena instantly.
void reset(Arena *a);

#endif