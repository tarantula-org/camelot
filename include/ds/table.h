/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by The Architectural Rigor Standard (ARS-1.0).
 * Compliance is mandatory for all contributions.
*/

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