/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef CAMELOT_STRING_H
#define CAMELOT_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "camelot/memory.h"
#include "primitives.h"

// A Slice-based String View (Pointer + Length).
// Immutable and non-owning by default.
typedef struct {
	u8 *ptr;
	u64 len;
} String;

// --- NAMESPACE ---

typedef struct {
	/*
	 * INTENT: Wraps a C-String constant into a Camelot String view.
	 * USAGE:
	 * ```
	 * String s = string.from("Hello");
	 * ```
	 * INVARIANTS: O(N) where N is strlen. Does not copy data.
	 * FAILURE MODES: Returns empty string if input is NULL.
	 */
	String (*from)(const char *c_str);

	/*
	 * INTENT: Concatenates two strings into a new buffer on the Arena.
	 * USAGE:
	 * ```
	 * String combined = string.join(&ctx, s1, s2);
	 * ```
	 * INVARIANTS: Result is null-terminated.
	 * FAILURE MODES: Returns empty string on OOM.
	 */
	String (*join)(Arena *a, String s1, String s2);

	/*
	 * INTENT: Compares two strings for byte-wise equality.
	 * USAGE:
	 * ```
	 * if (string.equal(s1, s2)) { ... }
	 * ```
	 * INVARIANTS: O(N) complexity. Returns true if pointers are identical.
	 * FAILURE MODES: None.
	 */
	bool (*equal)(String a, String b);
} StringNamespace;

extern const StringNamespace string;

#ifdef __cplusplus
}
#endif

#endif