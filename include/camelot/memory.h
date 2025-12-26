#ifndef CAMELOT_MEMORY_H
#define CAMELOT_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types/primitives.h"

// The raw Linear Allocator struct.
// Represents a borrowing reference to a memory context.
typedef struct {
      u8 *buf;
      u64 len;
      u64 cap;
      Result status;
} Arena;

// A scoped, owning Arena that cleans itself up automatically.
// Usage:
// ```
// Workspace ctx = arena.create(1024);
// ```
static inline void _cleanup_arena_func(Arena *a); 
#define Workspace __attribute__((cleanup(_cleanup_arena_func))) Arena

// --- NAMESPACE ---

typedef struct {
      // Creates a new memory context from the OS.
      // Returns the Arena struct by value.
      // Usage:
      // ```
      // Workspace ctx = arena.create(1024);
      // ```
      Arena (*create)(u64 size);

      // Returns memory to the OS.
      // Note: Called automatically if using 'Workspace'.
      // Usage:
      // ```
      // arena.release(&a);
      // ```
      void (*release)(Arena *a);

      // Resets the cursor to zero and securely zeroes the buffer.
      // Usage:
      // ```
      // arena.clear(&a);
      // ```
      void (*clear)(Arena *a);

      // Reserves 'size' bytes from the linear buffer.
      // Returns NULL if the Arena is full (OOM).
      // Usage:
      // ```
      // int *x = arena.alloc(&ctx, sizeof(int));
      // ```
      void* (*alloc)(Arena *a, u64 size);
} ArenaNamespace;

extern const ArenaNamespace arena;

// Internal Cleanup Helper
static inline void _cleanup_arena_func(Arena *a) {
    if (a && a->buf) arena.release(a);
}

#ifdef __cplusplus
}
#endif

#endif