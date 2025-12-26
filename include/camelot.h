#ifndef CAMELOT_H
#define CAMELOT_H

#ifdef __cplusplus
extern "C" {
#endif

// RELEASE.MAJOR.MINOR.PATCH
#define CAMELOT_VERSION "2.3.2.0"

// --- MODULES ---
#include "camelot/io.h"
#include "camelot/memory.h"
#include "types/primitives.h"
#include "types/string.h"
#include "ds/list.h"
#include "ds/table.h"

// --- SAFETY SWITCHES ---
// Poison standard library functions to enforce Camelot usage.
#ifndef ALLOW_UNSAFE
    // 1. Memory (Use arena.alloc)
    #pragma GCC poison malloc calloc realloc free aligned_alloc
    
    // 2. Strings (Use string.from / io.print)
    #pragma GCC poison strcpy strncpy strcat strncat strtok gets
    
    // 3. IO (Use io.print / io.scan)
    #pragma GCC poison sprintf vsprintf scanf fscanf sscanf printf fprintf vprintf
#endif

#ifdef __cplusplus
}
#endif

#endif