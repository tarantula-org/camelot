#include <stdint.h>

// --- INTERNAL: UNLOCK FORTRESS ---
#define CAMELOT_ALLOW_UNSAFE 
#include "../include/camelot.h"

// --- INTERNAL: HELPERS ---

//      Calculates the forward alignment padding.
//      Returns the next memory address that satisfies 'align'.
static uintptr_t c_align_forward(uintptr_t ptr, size_t align) {
    uintptr_t p = ptr;
    size_t modulo = p % align;
    
    if (modulo != 0) {
        p += (align - modulo);
    }
    return p;
}

// --- API: LIFECYCLE MANAGEMENT ---

ArenaResult c_arena_create(size_t size) {
    // 1. Input Validation
    if (size == 0) {
        return (ArenaResult){ .unwrap = NULL, .error = C_INVALID_SIZE };
    }

    // 2. Request System Memory
    //    We allocate Struct + Buffer in one contiguous block.
    size_t total_size = sizeof(Arena) + size;
    void *block = unsafe_malloc(total_size);

    if (block == NULL) {
        return (ArenaResult){ .unwrap = NULL, .error = C_OOM };
    }

    // 3. Initialize Structure
    Arena *a = (Arena *)block;
    a->buffer = (unsigned char *)(a + 1); // Buffer starts after the struct
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
    // Instantly invalidates all data by resetting the cursor.
    if (a) {
        a->offset = 0;
    }
}

// --- API: ALLOCATION ---

AllocResult c_alloc(Arena *a, size_t size, size_t align) {
    // 1. Calculate Addresses
    uintptr_t curr_ptr = (uintptr_t)a->buffer + (uintptr_t)a->offset;
    uintptr_t aligned_ptr = c_align_forward(curr_ptr, align);

    // 2. Calculate Padding (Wasted space for alignment)
    size_t padding = aligned_ptr - curr_ptr;

    // 3. Check Capacity (OOM Guard)
    if (a->offset + padding + size > a->length) {
        return (AllocResult){ .unwrap = NULL, .error = C_OOM };
    }

    // 4. Commit Allocation
    a->offset += (padding + size);

    // 5. Return Aligned Pointer
    return (AllocResult){ .unwrap = (void*)aligned_ptr, .error = C_OK };
}

AllocResult c_resize(Arena *a, void *old_ptr, size_t old_size, size_t new_size, size_t align) {
    // 1. Edge Case: NULL ptr -> Fresh Allocation
    if (old_ptr == NULL) {
        return c_alloc(a, new_size, align);
    }

    // 2. Edge Case: Size 0 -> No-op (or Free)
    if (new_size == 0) {
        return (AllocResult){ .unwrap = NULL, .error = C_OK };
    }

    // 3. Optimization: Try In-Place Expansion
    //    Check if old_ptr is the *very last* thing allocated.
    uintptr_t arena_top = (uintptr_t)a->buffer + a->offset;
    uintptr_t old_end   = (uintptr_t)old_ptr + old_size;

    if (arena_top == old_end) {
        if (new_size > old_size) {
            // Expand: Check capacity for the delta
            size_t diff = new_size - old_size;
            if (a->offset + diff > a->length) {
                return (AllocResult){ .unwrap = NULL, .error = C_OOM };
            }
            a->offset += diff;
        } else {
            // Shrink: Just pull the cursor back
            size_t diff = old_size - new_size;
            a->offset -= diff;
        }
        return (AllocResult){ .unwrap = old_ptr, .error = C_OK };
    }

    // 4. Slow Path: Relocation
    //    We are stuck in the middle, so we must alloc new + copy.
    AllocResult new_block = c_alloc(a, new_size, align);
    if (new_block.error != C_OK) {
        return new_block;
    }

    size_t copy_size = (old_size < new_size) ? old_size : new_size;
    memcpy(new_block.unwrap, old_ptr, copy_size);

    return new_block;
}

// --- API: TEMPORARY SCOPES ---

TempArena c_temp_begin(Arena *a) {
    // Snapshot the current state
    return (TempArena) {
        .arena = a,
        .prev_offset = a->offset
    };
}

void c_temp_end(TempArena temp) {
    // Restore the snapshot (Freeing everything since 'begin')
    if (temp.arena) {
        temp.arena->offset = temp.prev_offset;
    }
}