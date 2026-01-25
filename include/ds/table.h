/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by the Avant Systems Canon (ASC-1.2).
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
	void *value;
	bool alive;
} Entry;

typedef struct {
	Arena *source;
	Entry *entries;
	u64 cap;
	u64 count;
} Table;

// --- NAMESPACE ---

typedef struct {
	/*
	 * INTENT: Creates a Hash Table with Linear Probing.
	 * USAGE:
	 * ```
	 * Table config = table.create(&ctx, 64);
	 * ```
	 * INVARIANTS: Capacity is at least 16.
	 * FAILURE MODES: Returns empty/valid struct (alloc happens on creation).
	 */
	Table (*create)(Arena *a, u64 capacity);

	/*
	 * INTENT: Maps a String key to a value pointer. Overwrites if exists.
	 * USAGE:
	 * ```
	 * table.put(&config, string.from("Key"), &value);
	 * ```
	 * INVARIANTS: Auto-resizes if load factor > 0.75.
	 * FAILURE MODES: Triggers OOM if resize fails.
	 */
	void (*put)(Table *t, String key, void *value);

	/*
	 * INTENT: Retrieves the value pointer associated with the key.
	 * USAGE:
	 * ```
	 * int *val = table.get(&config, string.from("Key"));
	 * ```
	 * INVARIANTS: O(1) average case lookup.
	 * FAILURE MODES: Returns NULL if key not found.
	 */
	void *(*get)(Table *t, String key);
} TableNamespace;

extern const TableNamespace table;

#ifdef __cplusplus
}
#endif

#endif