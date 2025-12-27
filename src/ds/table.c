/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by The Architectural Rigor Standard (ARS-1.0).
 * Compliance is mandatory for all contributions.
*/

#include <string.h>
#include "camelot.h"

// --- HELPERS ---

static u64 hash_str(String s) {
      u64 hash = 0xcbf29ce484222325;
      for (u64 i = 0; i < s.len; i++) {
            hash ^= s.ptr[i];
            hash *= 0x100000001b3;
      }
      return hash;
}

// --- INTERNAL IMPLEMENTATION ---

static Table internal_create(Arena *a, u64 cap) {
      if (cap < 16) cap = 16;
      
      Entry *entries = arena.alloc(a, sizeof(Entry) * cap);
      
      // We assume entries are garbage, so we explicitly clear the 'alive' flag
      // or we rely on arena.create zeroing the memory. 
      // Safe bet: explicitly set alive = false.
      for (u64 i=0; i<cap; i++) entries[i].alive = false;

      return (Table){
            .source  = a,
            .entries = entries,
            .cap     = cap,
            .count   = 0
      };
}

static void internal_put(Table *t, String key, void *value); 

static void resize(Table *t) {
      u64 new_cap = t->cap * 2;
      Entry *old_entries = t->entries;
      u64 old_cap = t->cap;

      t->entries = arena.alloc(t->source, sizeof(Entry) * new_cap);
      t->cap = new_cap;
      t->count = 0;
      
      for (u64 i=0; i<new_cap; i++) t->entries[i].alive = false;

      for (u64 i = 0; i < old_cap; i++) {
            if (old_entries[i].alive) {
                  internal_put(t, old_entries[i].key, old_entries[i].value);
            }
      }
}

static void internal_put(Table *t, String key, void *value) {
      if (t->count * 4 >= t->cap * 3) {
            resize(t);
      }

      u64 h = hash_str(key);
      u64 idx = h % t->cap;

      while (true) {
            Entry *e = &t->entries[idx];

            if (!e->alive) {
                  e->key = key;
                  e->value = value;
                  e->alive = true;
                  t->count++;
                  return;
            }

            if (string.equal(e->key, key)) {
                  e->value = value;
                  return;
            }

            idx = (idx + 1) % t->cap;
      }
}

static void* internal_get(Table *t, String key) {
      if (t->count == 0) return NULL;

      u64 h = hash_str(key);
      u64 idx = h % t->cap;
      u64 start_idx = idx;

      while (true) {
            Entry *e = &t->entries[idx];

            if (!e->alive) return NULL;
            if (string.equal(e->key, key)) return e->value;

            idx = (idx + 1) % t->cap;
            if (idx == start_idx) return NULL;
      }
}

// --- NAMESPACE ---

const TableNamespace table = {
      .create = internal_create,
      .put    = internal_put,
      .get    = internal_get
};