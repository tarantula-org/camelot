#include "camelot.h"

void hook(Arena *a, void *buffer, u64 size) {
      a->buf = buffer;
      a->cap = size;
      a->len = 0;
      a->status = OK;
}

void *allocate(Arena *a, u64 size) {
      // 1. STICKY ERROR CHECK
      // If the arena is already broken (OOM), don't even try.
      if (a->status != OK) {
            return NULL;
      }

      // 2. Calculate current physical address
      uintptr_t address = (uintptr_t)a->buf + a->len;

      // 3. Calculate Alignment Padding
      // CPUs access memory faster (or won't crash) if data starts at 8-byte boundaries.
      // Logic:
      // - (address % 8): The remainder. How many bytes are we PAST the last boundary?
      // - (8 - remainder): How many bytes forward to the NEXT boundary?
      // - (% 8): Edge case fix. If we are already aligned (remainder 0), 8-0=8.
      // We want 0 padding, not 8. So (8 % 8) becomes 0.
      u64 padding = (8 - (address % 8)) % 8;

      // 4. SAFETY CHECK (Critical!)
      // Ensure we have space for both the padding AND the requested size.
      if (a->len + padding + size > a->cap) {
            a->status = OOM;
            return NULL;
      }

      // 5. Commit
      a->len += padding;            // Skip the padding bytes
      void *p = &a->buf[a->len];    // Grab the pointer
      a->len += size;               // Reserve the actual size

      return p;
}

void reset(Arena *a) {
      a->len = 0;
}