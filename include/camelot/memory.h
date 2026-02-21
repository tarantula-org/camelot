 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This Source Code Form is subject to the terms of the Mozilla Public	*
 * License, v. 2.0. If a copy of the MPL was not distributed with this	*
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.				*
 *																		*
 * Governed by the Avant Systems Canon (ASC-1.3).						*
 * Compliance is mandatory for all contributions.						*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef CAMELOT_MEMORY_H
#define CAMELOT_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types/primitives.h"

typedef struct {
	u8 *buf;
	u64 len;
	u64 cap;
	Result status;
} Arena;

static inline void _cleanup_arena_func(Arena *a);
#define Workspace __attribute__((cleanup(_cleanup_arena_func))) Arena

// --- NAMESPACE ---

typedef struct {
	/*
	 * INTENT: Creates a new memory context from the OS (malloc wrapper).
	 * USAGE:
	 * ```
	 * Workspace ctx = arena.create(1024);
	 * ```
	 * INVARIANTS: Memory is securely zeroed upon creation.
	 * FAILURE MODES: Returns status=OOM if OS refuses allocation.
	 */
	Arena (*create)(u64 size);

	/*
	 * INTENT: Returns memory to the OS.
	 * USAGE:
	 * ```
	 * arena.release(&a);
	 * ```
	 * INVARIANTS: Sets buffer pointer to NULL to prevent double-free.
	 * FAILURE MODES: Safe to call on NULL/empty arenas.
	 */
	void (*release)(Arena *a);

	/*
	 * INTENT: Resets cursor to zero and securely zeroes buffer content.
	 * USAGE:
	 * ```
	 * arena.clear(&a);
	 * ```
	 * INVARIANTS: Capacity remains unchanged.
	 * FAILURE MODES: None.
	 */
	void (*clear)(Arena *a);

	/*
	 * INTENT: Reserves 'size' bytes from the linear buffer with 8-byte alignment.
	 * USAGE:
	 * ```
	 * int *x = arena.alloc(&ctx, sizeof(int));
	 * ```
	 * INVARIANTS: Returned pointer is 8-byte aligned.
	 * FAILURE MODES: Returns NULL and sets a->status=OOM if full.
	 */
	void *(*alloc)(Arena *a, u64 size);
} ArenaNamespace;

extern const ArenaNamespace arena;

static inline void _cleanup_arena_func(Arena *a) {
	if (a && a->buf)
		arena.release(a);
}

#ifdef __cplusplus
}
#endif

#endif