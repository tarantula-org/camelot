#ifndef CAMELOT_LIST_H
#define CAMELOT_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "camelot/memory.h"

#define PAGE_SIZE 256

// A Paged Dynamic Array.
// Ensures O(1) pointer stability (pointers to elements never invalidate).
// Grows automatically by allocating new pages from the source Arena.
typedef struct {
      Arena* source;
      void** pages;
      u64 pages_cap;
      u64 pages_len;
      u64 item_size;
      u64 count;
} List;

// --- NAMESPACE ---

typedef struct {
      // Initializes a new List on the given Arena.
      // Usage:
      // ```
      // List ints = list.create(&ctx, sizeof(int));
      // ```
      List (*create)(Arena *a, u64 item_size);

      // Appends data to the end of the list.
      // Usage:
      // ```
      // list.push(&ints, &value);
      // ```
      void (*push)(List *l, void *data);

      // Retrieves a pointer to the item at index.
      // Usage:
      // ```
      // int *x = list.get(&ints, 5);
      // ```
      void* (*get)(List *l, u64 index);

      // Swap-removes the item at index.
      // Usage:
      // ```
      // list.remove(&ints, 5);
      // ```
      void (*remove)(List *l, u64 index);
} ListNamespace;

extern const ListNamespace list;

#ifdef __cplusplus
}
#endif

#endif