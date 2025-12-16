#ifndef CAMELOT_PRIMITIVES_H
#define CAMELOT_PRIMITIVES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// --- FIXED WIDTH TYPES ---
// We use Rust/Odin style names for brevity and clarity.

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

// --- ERROR HANDLING ---
// A unified error enum for the entire framework.
// No checking 'errno'. No exceptions.

typedef enum {
      OK = 0,
      OOM,
      OOB,
      INVALID_KEY,
      FILE_NOT_FOUND,
      IO_ERROR,
      TYPE_ERROR
} Result;

#endif