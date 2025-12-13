#ifndef CAMELOT_H
#define CAMELOT_H

// --- DEPENDENCIES ---
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

// --- CONFIGURATION ---
#ifdef CAMELOT_ALLOW_UNSAFE
      #define unsafe_malloc(size)     malloc(size)
      #define unsafe_free(ptr)        free(ptr)
      #define unsafe_scanf(...)       scanf(__VA_ARGS__)
#endif

// --- SECURITY: POISON ---
#pragma GCC poison malloc free realloc calloc
#pragma GCC poison scanf gets strcpy strcat

// --- TYPES: ERRORS ---
typedef enum {
      C_OK = 0,
      C_OOM,            // System or Arena Out of Memory
      C_INVALID_SIZE    // Size 0 or illogical request
} C_Error;

// --- TYPES: ARENA CORE ---
typedef struct {
      unsigned char *buffer;
      size_t length;
      size_t offset;
} Arena;

typedef struct {
      Arena *arena;
      size_t prev_offset;
} TempArena;

typedef struct {
      Arena *unwrap;
      C_Error error;
} ArenaResult;

typedef struct {
      void *unwrap;
      C_Error error;
} AllocResult;

// --- TYPES: DATA STRUCTURES ---
typedef struct {
      void *items;      // Pointer to start of array
      size_t count;     // Active elements
      size_t capacity;  // Total slots reserved
      size_t stride;    // sizeof(T)
      size_t align;     // _Alignof(T)
} List;

typedef struct {
      const char *ptr;  // Pointer to char buffer
      size_t len;       // Length (excluding null term)
} Str;

typedef struct {
      const char *key;
      void *value;
      uint64_t hash;
      bool is_alive;
} TableEntry;

typedef struct {
      TableEntry *entries;
      size_t capacity;  // Power of 2
      size_t count;
} Table;

// --- API: MEMORY KERNEL ---
[[nodiscard]] ArenaResult c_arena_create(size_t size);
void c_arena_destroy(Arena *a);
void c_arena_reset(Arena *a);

[[nodiscard]] AllocResult c_alloc(Arena *a, size_t size, size_t align);
[[nodiscard]] AllocResult c_resize(Arena *a, void *old_ptr, size_t old_size, size_t new_size, size_t align);

TempArena c_temp_begin(Arena *a);
void c_temp_end(TempArena temp);

// --- API: DYNAMIC LIST ---
AllocResult c_list_init_raw(Arena *a, List *list, size_t stride, size_t align, size_t capacity);
void *c_list_push_raw(Arena *a, List *list);

// --- API: STRINGS ---
Str c_str(const char *raw);
Str c_str_fmt(Arena *a, const char *fmt, ...);
bool c_str_eq(Str a, Str b);

// --- API: HASH TABLE ---
AllocResult c_table_init(Arena *a, Table *table, size_t capacity);
bool c_table_put(Arena *a, Table *table, const char *key, void *value);
void *c_table_get(Table *table, const char *key);

// --- MACROS: UTILITIES ---

// Memory
#define c_new(arena, type, count) \
      c_alloc(arena, sizeof(type) * (count), _Alignof(type))

#define c_resize_array(arena, ptr, type, old_count, new_count) \
      c_resize(arena, ptr, sizeof(type) * (old_count), sizeof(type) * (new_count), _Alignof(type))

// Lists
#define c_list_init(arena, list_ptr, type, capacity) \
      c_list_init_raw(arena, list_ptr, sizeof(type), _Alignof(type), capacity)

#define c_list_push(arena, list_ptr, type) \
      (type *)c_list_push_raw(arena, list_ptr)

#define c_list_at(list_ptr, type, index) \
      (((type *)(list_ptr)->items)[index])

#define c_list_len(list_ptr) ((list_ptr)->count)

// Strings
#define STR_FMT "%.*s"
#define STR_ARG(s) (int)(s).len, (s).ptr

#endif // CAMELOT_H