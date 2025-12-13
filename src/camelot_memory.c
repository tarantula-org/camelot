#include <stdint.h>

// --- INTERNAL: UNLOCK FORTRESS ---
#define CAMELOT_ALLOW_UNSAFE 
#include "../include/camelot.h"

// --- INTERNAL: HELPERS ---

static uintptr_t c_align_forward(uintptr_t ptr, size_t align) {
      uintptr_t p = ptr;
      size_t modulo = p % align;
      
      if (modulo != 0) {
            p += (align - modulo);
      }
      return p;
}

// --- IMPLEMENTATION: LIFECYCLE ---

ArenaResult c_arena_create(size_t size) {
      if (size == 0) {
            return (ArenaResult){ .unwrap = NULL, .error = C_INVALID_SIZE };
      }

      // Allocate Header + Buffer in one block
      size_t total_size = sizeof(Arena) + size;
      void *block = unsafe_malloc(total_size);

      if (block == NULL) {
            return (ArenaResult){ .unwrap = NULL, .error = C_OOM };
      }

      Arena *a = (Arena *)block;
      a->buffer = (unsigned char *)(a + 1);
      a->length = size;
      a->offset = 0;

      return (ArenaResult){ .unwrap = a, .error = C_OK };
}

void c_arena_destroy(Arena *a) {
      if (a) {
            unsafe_free(a);
      }
}

void c_arena_reset(Arena *a) {
      if (a) {
            a->offset = 0;
      }
}

// --- IMPLEMENTATION: ALLOCATOR ---

AllocResult c_alloc(Arena *a, size_t size, size_t align) {
      uintptr_t curr_ptr = (uintptr_t)a->buffer + (uintptr_t)a->offset;
      uintptr_t aligned_ptr = c_align_forward(curr_ptr, align);

      size_t padding = aligned_ptr - curr_ptr;

      if (a->offset + padding + size > a->length) {
            return (AllocResult){ .unwrap = NULL, .error = C_OOM };
      }

      a->offset += (padding + size);

      return (AllocResult){ .unwrap = (void*)aligned_ptr, .error = C_OK };
}

AllocResult c_resize(Arena *a, void *old_ptr, size_t old_size, size_t new_size, size_t align) {
      if (old_ptr == NULL) {
            return c_alloc(a, new_size, align);
      }

      if (new_size == 0) {
            return (AllocResult){ .unwrap = NULL, .error = C_OK };
      }

      // Check if this is the last allocation (Top of Stack)
      uintptr_t arena_top = (uintptr_t)a->buffer + a->offset;
      uintptr_t old_end   = (uintptr_t)old_ptr + old_size;

      if (arena_top == old_end) {
            // Optimization: In-Place Expansion
            if (new_size > old_size) {
                  size_t diff = new_size - old_size;
                  if (a->offset + diff > a->length) {
                        return (AllocResult){ .unwrap = NULL, .error = C_OOM };
                  }
                  a->offset += diff;
            } else {
                  size_t diff = old_size - new_size;
                  a->offset -= diff;
            }
            return (AllocResult){ .unwrap = old_ptr, .error = C_OK };
      }

      // Fallback: Allocate New + Copy
      AllocResult new_block = c_alloc(a, new_size, align);
      if (new_block.error != C_OK) {
            return new_block;
      }

      size_t copy_size = (old_size < new_size) ? old_size : new_size;
      memcpy(new_block.unwrap, old_ptr, copy_size);

      return new_block;
}

// --- IMPLEMENTATION: SCOPES ---

TempArena c_temp_begin(Arena *a) {
      return (TempArena) {
            .arena = a,
            .prev_offset = a->offset
      };
}

void c_temp_end(TempArena temp) {
      if (temp.arena) {
            temp.arena->offset = temp.prev_offset;
      }
}