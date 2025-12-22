#ifndef CAMELOT_MEMORY_H
#define CAMELOT_MEMORY_H

#include "types/primitives.h"

// A Linear Allocator (Arena).
// ALWAYS manages heap memory.
typedef struct {
      u8 *buf;
      u64 len;
      u64 cap;
      Result status;
} Arena;

// --- NAMESPACE ---

typedef struct {
      // Creates a new Arena by requesting 'size' bytes from the OS.
      // Returns the Arena struct by value.
      // Usage: 
      // ```
      // Arena a = arena.create(1024);
      // ```
      Arena (*create)(u64 size);

      // Returns the memory to the OS and invalidates the Arena.
      // Usage:
      // ```
      // arena.release(&a);
      // ```
      void (*release)(Arena *a);

      // Resets cursor to zero AND securely zeroes out the used memory.
      // Usage:
      // ```
      // arena.clear(&a);
      // ```
      void (*clear)(Arena *a);

      // Reserves a block of memory from the Arena.
      // Usage:
      // ```
      // int *x = arena.alloc(&a, sizeof(int));
      // ```
      void* (*alloc)(Arena *a, u64 size);
} ArenaNamespace;

extern const ArenaNamespace arena;

// Backwards compatibility / Shortcut if you prefer global access
// void *allocate(Arena *a, u64 size); 

#endif