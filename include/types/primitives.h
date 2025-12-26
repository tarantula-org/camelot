#ifndef CAMELOT_PRIMITIVES_H
#define CAMELOT_PRIMITIVES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// --- FIXED WIDTH TYPES ---
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
typedef enum {
      OK = 0,
      OOM,
      OOB,
      INVALID_KEY,
      FILE_NOT_FOUND,
      IO_ERROR,
      TYPE_ERROR
} Result;

#ifdef __cplusplus
}
#endif

#endif