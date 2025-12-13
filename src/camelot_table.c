#include "../include/camelot.h"

// --- INTERNAL: HASHING (FNV-1a) ---

static uint64_t hash_key(const char *key) {
      uint64_t hash = 0xcbf29ce484222325ULL;
      for (const char *p = key; *p; p++) {
            hash ^= (uint8_t)*p;
            hash *= 0x100000001b3ULL;
      }
      return hash;
}

// --- INTERNAL: PROBING ---

static TableEntry *find_entry(TableEntry *entries, size_t capacity, const char *key) {
      uint64_t hash = hash_key(key);
      size_t index = hash & (capacity - 1);
      size_t start_index = index;

      while (true) {
            TableEntry *e = &entries[index];

            // Found Empty
            if (!e->is_alive) {
                  return e;
            }

            // Found Match
            if (strcmp(e->key, key) == 0) {
                  return e;
            }

            // Collision (Linear Probe)
            index = (index + 1) & (capacity - 1);

            if (index == start_index) return NULL; 
      }
}

static bool table_expand(Arena *a, Table *t) {
      size_t new_cap = t->capacity * 2;
      
      AllocResult res = c_alloc(a, sizeof(TableEntry) * new_cap, _Alignof(TableEntry));
      if (res.error != C_OK) return false;

      TableEntry *new_entries = (TableEntry *)res.unwrap;
      
      // Zero out memory to ensure is_alive is false
      memset(new_entries, 0, sizeof(TableEntry) * new_cap);

      // Re-hash
      for (size_t i = 0; i < t->capacity; i++) {
            TableEntry *old_e = &t->entries[i];
            if (old_e->is_alive) {
                  TableEntry *dest = find_entry(new_entries, new_cap, old_e->key);
                  dest->key = old_e->key;
                  dest->value = old_e->value;
                  dest->hash = old_e->hash;
                  dest->is_alive = true;
            }
      }

      t->entries = new_entries;
      t->capacity = new_cap;
      return true;
}

// --- IMPLEMENTATION: API ---

AllocResult c_table_init(Arena *a, Table *t, size_t capacity) {
      size_t cap = 16;
      while (cap < capacity) cap *= 2;

      t->count = 0;
      t->capacity = cap;

      AllocResult res = c_alloc(a, sizeof(TableEntry) * cap, _Alignof(TableEntry));
      if (res.error != C_OK) return res;

      t->entries = (TableEntry *)res.unwrap;
      memset(t->entries, 0, sizeof(TableEntry) * cap);
      
      return res;
}

bool c_table_put(Arena *a, Table *t, const char *key, void *value) {
      // Load Factor check (0.75)
      if (t->count + 1 > t->capacity * 0.75) {
            if (!table_expand(a, t)) return false;
      }

      TableEntry *e = find_entry(t->entries, t->capacity, key);
      
      bool is_new = !e->is_alive;
      if (is_new) {
            e->key = key;
            e->is_alive = true;
            e->hash = hash_key(key);
            t->count++;
      }
      
      e->value = value;
      return is_new;
}

void *c_table_get(Table *t, const char *key) {
      if (t->count == 0) return NULL;
      
      TableEntry *e = find_entry(t->entries, t->capacity, key);
      if (e && e->is_alive) {
            return e->value;
      }
      return NULL;
}