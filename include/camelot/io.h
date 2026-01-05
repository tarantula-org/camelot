/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by The Architectural Rigor Standard (ARS-1.0).
 * Compliance is mandatory for all contributions.
 */

#ifndef CAMELOT_IO_H
#define CAMELOT_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "camelot/memory.h"
#include "types/primitives.h"
#include "types/string.h"

// --- TYPES ---

typedef struct {
  void *handle;  // Internal FILE* pointer
  Result status; // Current state (OK, FILE_NOT_FOUND, etc.)
  u64 size;      // Total size (0 if unknown/pipe)
} File;

typedef enum { OPEN, READ, SKIP, CLOSE } Op;

// --- NAMESPACE ---

typedef struct {
  /*
   * INTENT: Reads a line from stdin into the Arena with buffer overflow
   * protection. USAGE:
   * ```
   * String name = io.scan(&ctx, 0);
   * ```
   * INVARIANTS: Result is always null-terminated. Handles CRLF/LF
   * automatically. FAILURE MODES: Returns empty string if EOF reached or
   * allocation fails.
   */
  String (*scan)(Arena *a, u64 cap);

  /*
   * INTENT: Writes a raw String view to stdout.
   * USAGE:
   * ```
   * io.put(s);
   * ```
   * INVARIANTS: Does not append newline. O(1) if write buffers are available.
   * FAILURE MODES: Silent failure if stdout is closed.
   */
  void (*put)(String s);

  /*
   * INTENT: Formatted print supporting Camelot types (%S, %s, %i, %f).
   * USAGE:
   * ```
   * io.print("User: %S\n", name);
   * ```
   * INVARIANTS: Thread-safe if underlying libc write is thread-safe.
   * FAILURE MODES: Truncates output if internal buffers (rare) are exceeded.
   */
  void (*print)(const char *fmt, ...);

  /*
   * INTENT: Reads an entire file into memory and auto-closes the handle.
   * USAGE:
   * ```
   * String config = io.slurp(&ctx, "config.ini");
   * ```
   * INVARIANTS: Returns null-terminated string.
   * FAILURE MODES: Returns NULL string (ptr=0) if file missing or read fails.
   */
  String (*slurp)(Arena *a, const char *path);

  /*
   * INTENT: Unified dispatcher for Files, Pipes, and Sockets operations.
   * USAGE:
   * ```
   * io.stream(&f, READ, buf, 1024);
   * ```
   * INVARIANTS: Maintains file cursor state.
   * FAILURE MODES: Returns 0 on failure/EOF. Check f->status for details.
   */
  u64 (*stream)(File *f, Op op, void *arg, u64 num);
} IONamespace;

extern const IONamespace io;

#ifdef __cplusplus
}
#endif

#endif