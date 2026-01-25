/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by the Avant Systems Canon (ASC-1.2).
 * Compliance is mandatory for all contributions.
 */

#ifndef CAMELOT_H
#define CAMELOT_H

#ifdef __cplusplus
extern "C" {
#endif

// --- VERSION ---
// Format: Epoch.Major.Minor.Patch
#define CAMELOT_VERSION "1.2.0.0"

// --- MODULES ---
#include "camelot/io.h"
#include "camelot/memory.h"
#include "ds/list.h"
#include "ds/table.h"
#include "types/primitives.h"
#include "types/string.h"

// --- SAFETY ENFORCEMENT ---
// By default, we poison standard libc allocation/string functions
// to force usage of Camelot's Arena-based safe alternatives.
// Define ALLOW_UNSAFE before including this file to bypass.

#ifndef ALLOW_UNSAFE
// 1. Memory (Use arena.alloc)
#pragma GCC poison malloc calloc realloc free aligned_alloc

// 2. Strings (Use string.from / io.print)
#pragma GCC poison strcpy strncpy strcat strncat strtok gets

// 3. IO (Use io.print / io.scan)
// Note: printf is allowed for debugging but io.print is preferred.
#endif

#ifdef __cplusplus
}
#endif

#endif