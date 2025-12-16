// We unlock safety internally to use 'snprintf'.
// This is necessary for accurate floating-point math (IEEE-754 is hard).
#define ALLOW_UNSAFE 

#include "camelot/io.h"

#include <unistd.h> // write
#include <stdarg.h> // va_list
#include <stdio.h>  // snprintf
#include <string.h> // strlen

// --- CORE I/O ---

void put(String s) {
      write(1, s.ptr, s.len);
}

void putn(String s) {
      put(s);
      write(1, "\n", 1);
}

// --- HELPERS ---

// Efficiently writes a 64-bit integer to stdout.
static void put_i64(long long n) {
      if (n == 0) { write(1, "0", 1); return; }
      if (n < 0)  { write(1, "-", 1); n = -n; }

      char buf[24]; // Max i64 is ~19 digits + sign
      int i = 0;

      while (n > 0) {
            buf[i++] = (n % 10) + '0';
            n /= 10;
      }
      while (i > 0) write(1, &buf[--i], 1);
}

// Uses stdlib to handle complex float cases (scientific notation, infinity).
static void put_f64(double n) {
      char buf[64];
      // %.16g: General format, max precision, no trailing zeros.
      int len = snprintf(buf, sizeof(buf), "%.16g", n);
      write(1, buf, len);
}

// --- FORMATTER ---

void print(const char *fmt, ...) {
      va_list args;
      va_start(args, fmt);

      for (const char *p = fmt; *p != '\0'; p++) {
            if (*p != '%') {
                  write(1, p, 1);
                  continue;
            }

            p++; // Consume '%'
            switch (*p) {
                  case 'i': // Integer
                        put_i64(va_arg(args, int));
                        break;

                  case 'f': // Float (Standard double)
                        put_f64(va_arg(args, double));
                        break;

                  case 's': { // C-String (null-terminated)
                        char *c = va_arg(args, char*);
                        write(1, c, strlen(c));
                        break;
                  }

                  case 'S': // Camelot String
                        put(va_arg(args, String)); 
                        break;

                  default: // Unknown (e.g. "50%") -> print "%"
                        write(1, p-1, 1); // Print the '%'
                        p--;              // Backtrack to print the char next loop
            }
      }

      va_end(args);
}