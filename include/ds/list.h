/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef CAMELOT_LIST_H
#define CAMELOT_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "camelot/memory.h"

#define PAGE_SIZE 256

typedef struct {
	Arena *source;
	void **pages;
	u64 pages_cap;
	u64 pages_len;
	u64 item_size;
	u64 count;
} List;

// --- NAMESPACE ---

typedef struct {
	/*
	 * INTENT: Initializes a new Paged List on the given Arena.
	 * USAGE:
	 * ```
	 * List ints = list.create(&ctx, sizeof(int));
	 * ```
	 * INVARIANTS: List owns no memory until first push.
	 * FAILURE MODES: Returns valid struct; allocation failures occur on push.
	 */
	List (*create)(Arena *a, u64 item_size);

	/*
	 * INTENT: Appends a copy of the data to the end of the list.
	 * USAGE:
	 * ```
	 * list.push(&ints, &value);
	 * ```
	 * INVARIANTS: Pointers to existing elements remain valid (No Realloc).
	 * FAILURE MODES: Triggers OOM on Arena if page allocation fails.
	 */
	void (*push)(List *l, void *item_ptr);

	/*
	 * INTENT: Retrieves a pointer to the mutable item at index.
	 * USAGE:
	 * ```
	 * int *x = list.get(&ints, 5);
	 * ```
	 * INVARIANTS: O(1) access time.
	 * FAILURE MODES: Returns NULL if index >= count.
	 */
	void *(*get)(List *l, u64 index);

	/*
	 * INTENT: Swap-removes the item at index (unordered removal).
	 * USAGE:
	 * ```
	 * list.remove(&ints, 5);
	 * ```
	 * INVARIANTS: Moves the last element into the removed slot.
	 * FAILURE MODES: No-op if index >= count.
	 */
	void (*remove)(List *l, u64 index);
} ListNamespace;

extern const ListNamespace list;

#ifdef __cplusplus
}
#endif

#endif