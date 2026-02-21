 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This Source Code Form is subject to the terms of the Mozilla Public	*
 * License, v. 2.0. If a copy of the MPL was not distributed with this	*
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.				*
 *																		*
 * Governed by the Avant Systems Canon (ASC-1.3).						*
 * Compliance is mandatory for all contributions.						*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef CAMELOT_PRIMITIVES_H
#define CAMELOT_PRIMITIVES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/* --- CORE TYPES --- */

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

/* --- ERROR HANDLING --- */

typedef enum {
	OK = 0,
	OOM,
	OOB,
	INVALID_KEY,
	FILE_NOT_FOUND,
	IO_ERROR,
} Result;

#ifdef __cplusplus
}
#endif

#endif