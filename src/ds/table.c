#include <string.h> // For memset/memcpy if needed
#include "camelot.h"

// --- HELPERS ---

// FNV-1a Hash: Fast, good distribution for strings.
static u64 hash_str(String s) {
      u64 hash = 0xcbf29ce484222325; // FNV offset basis
      for (u64 i = 0; i < s.len; i++) {
            hash ^= s.ptr[i];
            hash *= 0x100000001b3;   // FNV prime
      }
      return hash;
}

// --- INTERNAL IMPLEMENTATION ---

static Table internal_create(Arena *a, u64 cap) {
      // Force power of 2 for fast modulo (optional, but good practice)
      if (cap < 16) cap = 16;
      
      Entry *entries = allocate(a, sizeof(Entry) * cap);
      // Important: allocate() in Camelot might not zero-init if we change implementation.
      // But currently our Arena doesn't zero. Let's rely on 'alive' boolean being false?
      // Actually, safest is to explicitly zero the 'alive' flags or the whole block.
      // Since we don't have memset in 'allocate', we should assume garbage.
      // Let's implement a quick zeroing loop or rely on 'allocate' returning zeroed memory
      // (Standard arenas often don't, but let's assume we need to clear).
      
      for (u64 i=0; i<cap; i++) entries[i].alive = false;

      return (Table){
            .source  = a,
            .entries = entries,
            .cap     = cap,
            .count   = 0
      };
}

static void internal_put(Table *t, String key, void *value); // Forward decl

static void resize(Table *t) {
      u64 new_cap = t->cap * 2;
      Entry *old_entries = t->entries;
      u64 old_cap = t->cap;

      // 1. Allocate new block
      t->entries = allocate(t->source, sizeof(Entry) * new_cap);
      t->cap = new_cap;
      t->count = 0;
      
      for (u64 i=0; i<new_cap; i++) t->entries[i].alive = false;

      // 2. Rehash old items
      for (u64 i = 0; i < old_cap; i++) {
            if (old_entries[i].alive) {
                  internal_put(t, old_entries[i].key, old_entries[i].value);
            }
      }
}

static void internal_put(Table *t, String key, void *value) {
      // 1. Load Factor Check (75%)
      if (t->count * 4 >= t->cap * 3) {
            resize(t);
      }

      u64 h = hash_str(key);
      u64 idx = h % t->cap;

      // 2. Linear Probe
      while (true) {
            Entry *e = &t->entries[idx];

            // Empty slot? Take it.
            if (!e->alive) {
                  e->key = key;
                  e->value = value;
                  e->alive = true;
                  t->count++;
                  return;
            }

            // Existing key? Overwrite.
            if (string.equal(e->key, key)) {
                  e->value = value;
                  return;
            }

            // Collision? Move next.
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

            // Found empty slot? Key doesn't exist.
            if (!e->alive) return NULL;

            // Found key? Return value.
            if (string.equal(e->key, key)) return e->value;

            // Move next
            idx = (idx + 1) % t->cap;
            
            // Full circle guard (shouldn't happen if resize works, but safe)
            if (idx == start_idx) return NULL;
      }
}

// --- NAMESPACE ---

const TableNamespace table = {
      .create = internal_create,
      .put    = internal_put,
      .get    = internal_get
};