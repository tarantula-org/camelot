/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Governed by The Architectural Rigor Standard (ARS-1.0).
 * Compliance is mandatory for all contributions.
 */

#include "camelot.h"
#include <stdarg.h> // va_list, va_start, va_end
#include <stdio.h>  // snprintf
#include <unistd.h> // write, read

#ifndef ALLOW_UNSAFE
#define ALLOW_UNSAFE
#endif

#define IO_DEFAULT_SCAN_CAP 4096

// --- SCAN ---

String scan(Arena *a, u64 cap) {
  if (cap == 0) {
    cap = IO_DEFAULT_SCAN_CAP;
  }

  u8 *buf = arena.alloc(a, cap);
  if (!buf) {
    return (String){0};
  }

  u64 count = 0;
  char c;

  while (count < cap - 1) {
    ssize_t n = read(0, &c, 1);
    if (n <= 0) {
      break;
    }
    if (c == '\n') {
      break;
    }
    if (c == '\r') {
      continue; // Ignore CR
    }

    buf[count++] = c;
  }

  buf[count] = '\0';
  return (String){.ptr = buf, .len = count};
}

// --- PRINT ---

void put(String s) {
  write(1, s.ptr, s.len);
}

static void put_i64(long long n) {
  if (n == 0) {
    write(1, "0", 1);
    return;
  }
  if (n < 0) {
    write(1, "-", 1);
    n = -n;
  }

  char buf[24];
  int i = 0;
  while (n > 0) {
    buf[i++] = (n % 10) + '0';
    n /= 10;
  }
  while (i > 0) {
    write(1, &buf[--i], 1);
  }
}

static void put_f64(double n) {
  char buf[64];
  int len = snprintf(buf, sizeof(buf), "%.16g", n);
  write(1, buf, len);
}

void print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  for (const char *p = fmt; *p != '\0'; p++) {
    if (*p != '%') {
      write(1, p, 1);
      continue;
    }

    p++;
    switch (*p) {
    case 'i':
      put_i64(va_arg(args, int));
      break;
    case 'f':
      put_f64(va_arg(args, double));
      break;
    case 's': {
      char *c = va_arg(args, char *);
      write(1, c, strlen(c));
      break;
    }
    case 'S':
      put(va_arg(args, String));
      break;
    case '%':
      write(1, "%", 1);
      break;
    default:
      write(1, p - 1, 1);
      p--;
    }
  }
  va_end(args);
}