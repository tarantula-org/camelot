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

#include "types/string.h"
#include "types/primitives.h"
#include "camelot/memory.h"

// --- TYPES ---

typedef struct {
      void *handle;     // Internal FILE* pointer
      Result status;    // Current state (OK, FILE_NOT_FOUND, etc.)
      u64 size;         // Total size (0 if unknown/pipe)
} File;

typedef enum {
      OPEN, READ, SKIP, CLOSE
} Op;

// --- NAMESPACE ---

typedef struct {
      // Reads a line from stdin into the Arena.
      // Buffer-overflow proof. Handles CRLF/LF line endings.
      // Usage:
      // ```
      // String name = io.scan(&ctx, 0); 
      // ```
      String (*scan)(Arena *a, u64 cap);

      // Writes a raw String to stdout.
      // Usage:
      // ```
      // io.put(s);
      // ```
      void (*put)(String s);

      // Formatted print supporting Camelot types (%S, %s, %i, %f).
      // Usage:
      // ```
      // io.print("User: %S\n", name);
      // ```
      void (*print)(const char *fmt, ...);

      // Reads an entire file into memory. Auto-closes the file.
      // Usage:
      // ```
      // String config = io.slurp(&ctx, "config.ini");
      // ```
      String (*slurp)(Arena *a, const char *path);

      // Unified dispatcher for Files, Pipes, and Sockets.
      // Usage:
      // ```
      // io.stream(&f, READ, buf, 1024);
      // ```
      u64 (*stream)(File *f, Op op, void *arg, u64 num);
} IONamespace;

extern const IONamespace io;

#ifdef __cplusplus
}
#endif

#endif