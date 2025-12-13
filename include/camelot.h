#ifndef CAMELOT_H
#define CAMELOT_H

// --- DEPENDENCIES ---
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// --- CONFIGURATION ---
//      Check if the user wants access to raw system memory.
//      This is the "Backdoor" for implementation files.
#ifdef CAMELOT_ALLOW_UNSAFE
    #define unsafe_malloc(size)     malloc(size)
    #define unsafe_free(ptr)        free(ptr)
    #define unsafe_scanf(...)       scanf(__VA_ARGS__)
#endif

// --- SECURITY: THE POISON BARRIER ---
//      These directives ban standard library memory functions
//      to ensure all memory goes through the Fortress.
#pragma GCC poison malloc free realloc calloc
#pragma GCC poison scanf gets strcpy strcat

// --- TYPES: ERROR HANDLING ---
typedef enum {
    C_OK = 0,
    C_OOM,              // Out of memory (System or Arena full)
    C_INVALID_SIZE      // Request was 0 or too large
} C_Error;

// --- TYPES: CORE STRUCTS ---
typedef struct {
    unsigned char *buffer;  // The raw contiguous memory block
    size_t length;          // Total capacity of the block
    size_t offset;          // Current "Bump Pointer" position
} Arena;

typedef struct {
    Arena *arena;
    size_t prev_offset;     // The "Snapshot" of the offset
} TempArena;

// --- TYPES: RESULTS ---
//      Modern C pattern: Return structs instead of pointers
//      to force the user to handle errors.

typedef struct {
    Arena *unwrap;
    C_Error error;
} ArenaResult;

typedef struct {
    void *unwrap;
    C_Error error;
} AllocResult;

// --- API: LIFECYCLE MANAGEMENT ---
//      Functions for creating, destroying, and resetting arenas.

[[nodiscard]] ArenaResult c_arena_create(size_t size);
void c_arena_destroy(Arena *a);
void c_arena_reset(Arena *a);

// --- API: ALLOCATION ---
//      Core memory operations (Alloc, Resize).

[[nodiscard]] AllocResult c_alloc(Arena *a, size_t size, size_t align);
[[nodiscard]] AllocResult c_resize(Arena *a, void *old_ptr, size_t old_size, size_t new_size, size_t align);

// --- API: TEMPORARY SCOPES ---
//      Functions for "Scratchpad" memory (Time Travel).

TempArena c_temp_begin(Arena *a);
void c_temp_end(TempArena temp);

// --- MACROS: USER CONVENIENCE ---
//      Type-safe wrappers for the core API.

//      Allocates 'count' items of 'type'.
//      Example: int *arr = c_new(arena, int, 10);
#define c_new(arena, type, count) \
    c_alloc(arena, sizeof(type) * (count), _Alignof(type))

//      Resizes an existing array, trying to expand in-place.
//      Example: c_resize_array(arena, arr, int, 10, 20);
#define c_resize_array(arena, ptr, type, old_count, new_count) \
    c_resize(arena, ptr, sizeof(type) * (old_count), sizeof(type) * (new_count), _Alignof(type))

#endif // CAMELOT_H