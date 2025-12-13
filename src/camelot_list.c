#include "../include/camelot.h"

// --- IMPLEMENTATION: DYNAMIC LIST ---

AllocResult c_list_init_raw(Arena *a, List *list, size_t stride, size_t align, size_t capacity) {
      list->count = 0;
      list->capacity = (capacity < 4) ? 4 : capacity;
      list->stride = stride;
      list->align = align;

      size_t total_size = list->capacity * list->stride;
      AllocResult res = c_alloc(a, total_size, list->align);
      
      list->items = res.unwrap;
      return res;
}

void *c_list_push_raw(Arena *a, List *list) {
      // 1. Check Capacity
      if (list->count < list->capacity) {
            uintptr_t base = (uintptr_t)list->items;
            uintptr_t offset = list->count * list->stride;
            list->count++;
            return (void*)(base + offset);
      }

      // 2. Growth Strategy (Double Capacity)
      size_t old_cap = list->capacity;
      size_t new_cap = old_cap * 2; 
      
      size_t old_size = old_cap * list->stride;
      size_t new_size = new_cap * list->stride;

      AllocResult res = c_resize(a, list->items, old_size, new_size, list->align);

      if (res.error != C_OK) {
            return NULL;
      }

      list->items = res.unwrap;
      list->capacity = new_cap;

      // 3. Return Slot
      uintptr_t base = (uintptr_t)list->items;
      uintptr_t offset = list->count * list->stride;
      list->count++;
      
      return (void*)(base + offset);
}