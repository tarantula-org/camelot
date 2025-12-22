#ifndef CAMELOT_LIST_H
#define CAMELOT_LIST_H

#include "camelot/memory.h"

#define PAGE_SIZE 256

typedef struct {
      Arena* source;
      void** pages;     // Directory of pages
      u64 pages_cap;
      u64 pages_len;
      u64 item_size;
      u64 count;
} List;

// --- NAMESPACE ---

typedef struct {
      // Creates a new Paged List
      // Usage:
      // List foo = list.create(&arena, sizeof(int));
      List (*create)(Arena *a, u64 item_size);

      // Pushes an item to the end. O(1).
      void (*push)(List *l, void *data);

      // Gets a pointer to the item at index. O(1).
      void* (*get)(List *l, u64 index);

      // Removes item at index by swapping with last. O(1).
      void (*remove)(List *l, u64 index);
} ListNamespace;

extern const ListNamespace list;

#endif