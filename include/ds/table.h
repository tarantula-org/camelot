#ifndef CAMELOT_TABLE_H
#define CAMELOT_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../camelot/memory.h"
#include "../types/string.h"

typedef struct {
      String key;
      void  *value;
      bool   alive;
} Entry;

typedef struct {
      Arena *source;
      Entry *entries;
      u64    cap;
      u64    count;
} Table;

// --- NAMESPACE ---

typedef struct {
      // Creates a Hash Table with Linear Probing.
      // Usage:
      // ```
      // Table config = table.create(&ctx, 64);
      // ```
      Table (*create)(Arena *a, u64 capacity);

      // Maps a String key to a value pointer.
      // Usage:
      // ```
      // table.put(&config, string.from("Key"), &value);
      // ```
      void (*put)(Table *t, String key, void *value);

      // Retrieves the value pointer associated with the key.
      // Usage:
      // ```
      // int *val = table.get(&config, string.from("Key"));
      // ```
      void* (*get)(Table *t, String key);
} TableNamespace;

extern const TableNamespace table;

#ifdef __cplusplus
}
#endif

#endif