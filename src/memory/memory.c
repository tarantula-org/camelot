/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by The Architectural Rigor Standard (ARS-1.0).
 * Compliance is mandatory for all contributions.
 */

#include "camelot.h"
#include <stdlib.h>
#include <string.h>

#define ALLOW_UNSAFE

// --- INTERNAL IMPLEMENTATION ---

static Arena internal_create(u64 size) {
  void *mem = malloc(size);
  if (!mem) {
    return (Arena){.status = OOM};
  }

  // Zero out immediately for security
  memset(mem, 0, size);

  return (Arena){
      .buf = (u8 *)mem,
      .cap = size,
      .len = 0,
      .status = OK,
  };
}

static void internal_release(Arena *a) {
  if (a->buf) {
    free(a->buf);
    a->buf = NULL;
  }
  a->cap = 0;
  a->len = 0;
}

static void internal_clear(Arena *a) {
  // SECURITY: Null the memory as requested before resetting cursor
  if (a->buf && a->len > 0) {
    memset(a->buf, 0, a->len);
  }
  a->len = 0;
}

static void *internal_alloc(Arena *a, u64 size) {
  if (a->status != OK)
    return NULL;

  uintptr_t address = (uintptr_t)a->buf + a->len;
  // 8-byte alignment
  u64 padding = (8 - (address % 8)) % 8;

  if (a->len + padding + size > a->cap) {
    a->status = OOM;
    return NULL;
  }

  a->len += padding;
  void *p = &a->buf[a->len];
  a->len += size;
  return p;
}

// --- NAMESPACE ---

const ArenaNamespace arena = {
    .create = internal_create,
    .release = internal_release,
    .clear = internal_clear,
    .alloc = internal_alloc,
};