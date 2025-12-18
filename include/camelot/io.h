#ifndef CAMELOT_IO_H
#define CAMELOT_IO_H

#include "types/string.h"
#include "types/primitives.h"
#include "camelot/memory.h"

typedef struct {
      void *handle;     // Internal FILE* pointer
      Result status;    // Current state (OK, FILE_NOT_FOUND, etc.)
      u64 size;         // Total size in bytes (calculated on open)
} File;

typedef enum {
      OPEN, // arg: (char*)path.    num: unused. Returns 1 (ok) or 0 (fail).
      READ, // arg: (void*)buf.     num: bytes.   Returns bytes read.
      SKIP, // arg: NULL.           num: bytes.   Returns new offset.
      CLOSE // arg: NULL.           num: unused.  Returns 0.
} Op;

// --- OUTPUT ---

// Writes a raw string to Standard Output (console).
// Does not append a newline.
void put(String s);

// Writes a string followed by a newline character `'\n'`.
void putn(String s);

// Replaces 'printf'. Parses a format string and prints arguments.
//
// Supported: 
// ```
// %i (int), %f (double), %s (char*), %S (String)
// ```
void print(const char *fmt, ...);

// --- FILE OPERATIONS ---

// Reads entire file into Arena. Zero config.
String slurp(Arena *a, const char *path);

// The Unified I/O Dispatcher.
// Performs the action 'op' on file 'f'.
// 
// Usage:
// ```
// File f = {0};
// if (stream(&f, OPEN, "data.txt", 0)) {
//    stream(&f, READ, buffer, 1024);
//    stream(&f, CLOSE, NULL, 0);
// }
// ```
u64 stream(File *f, Op op, void *arg, u64 num);

#endif