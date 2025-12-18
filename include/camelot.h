#ifndef CAMELOT_H
#define CAMELOT_H

// RELEASE.MAJOR.MINOR.PATCH
#define CAMELOT_VERSION "2.0.0.4"

// --- MODULES ---
// We now point to the sub-directories since this file sits at the root.
#include "camelot/io.h"
#include "camelot/memory.h"
#include "types/primitives.h"
#include "types/string.h"
#include "ds/list.h"
#include "ds/table.h"

// --- ALLOW UNSAFE ---
// If the user has NOT explicitly asked for unsafe access,
// we ban the dangerous standard library functions.

#ifndef ALLOW_UNSAFE

      // --- 1. MEMORY MANAGEMENT ---
      // We strictly use Arenas. Manual heap management is forbidden.
      #pragma GCC poison malloc calloc realloc free aligned_alloc
      
      // --- 2. DANGEROUS STRING MANIPULATION ---
      // These functions cause buffer overflows. Use Camelot 'Str' instead.
      // 'gets' is so bad it was removed from C11, but we poison it just in case.
      #pragma GCC poison strcpy strncpy strcat strncat strtok gets
      
      // --- 3. UNSAFE FORMATTING & IO ---
      // We strictly use Camelot I/O. Standard IO is banned to prevent
      // format string vulnerabilities and unhandled errors.
      #pragma GCC poison sprintf vsprintf scanf fscanf sscanf printf fprintf vprintf

#endif

#endif