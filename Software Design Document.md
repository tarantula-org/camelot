# Software Design Document Template

> [\!Important]
> All designs and implementations must strictly adhere to the [Yutila Security Policies](https://docs.google.com/document/d/1zqEZ9wyOiUj6hyH294iyZh9IXi8PnqnQC8UB7tnc0I0/edit?usp=sharing). Verify that your proposal incorporates the core architectural principles (Fail-Safe Defaults, Least Privilege, and Open Design) before submitting for review.

## Constraints

### Naming Coherence

The Camelot framework enforces strict nomenclature rules to ensure unparalleled consistency across the codebase.

1.  **Primary Requirement:** All structures and functions must strictly utilize the `NAMESPACE_function` format, where the prefix/namespace is fully uppercase and the action/function suffix is fully lowercase.
2.  **Secondary Requirement:** Namespace and function components must be connected by an underscore, and full words must be consistently favored to maintain semantic clarity.
3.  **Prohibitions & Restrictions:** Word truncations or casual abbreviations are strictly prohibited unless using universally standard acronyms (e.g., `IO`).

### Portability & Compiler Extensions

To guarantee absolute portability across arbitrary C compilers and environments, reliance on non-standard runtime compiler extensions is explicitly prohibited.

1.  **Primary Requirement:** The codebase must remain compatible with all major C compilers (e.g., MSVC, GCC, Clang) by avoiding features that inject or manipulate logic at runtime via compiler-specific extensions.
2.  **Secondary Requirement:** Compiler attributes that operate strictly during compilation without mutating runtime binaries, such as static analysis hints (e.g., `__attribute__((warn_unused_result))`), are acceptable and encouraged.
3.  **Prohibitions & Restrictions:** Runtime-altering extensions, specifically GCC's `__attribute__((cleanup))` for RAII emulation, are forbidden due to lack of support in non-GNU environments.

## 1\. Problems to be Solved
git
### Problem: Allocator agnosticism

  - **Statement:** Hardcoding `malloc` and `free` throughout a codebase creates rigid data structures. It prevents developers from swapping out memory strategies for testing, restricted environments, or performance optimization without rewriting the structure's internal logic.
  - **Solutions:** [[1]](#solution-vtable)

### Problem: Arena

  - **Statement:** Tracking and freeing thousands of individual object allocations (`malloc`/`free`) leads to memory fragmentation, high CPU overhead, and inevitable memory leaks when a single free is forgotten.
  - **Solutions:** [[1]](#solution-arena)

### Problem: Primitives

  - **Statement:** Primitives are not standardized by default and their sizes vary depending on the architecture and compiler.
  - **Solutions:** [[1]](#solution-primitives)

### Problem: Slices

  - **Statement:** Standard C arrays decay into raw pointers when passed across function boundaries, dropping length metadata. This forces developers to pass separate size arguments, or rely on unsafe assumptions. Additionally, accessing substrings or extracting segments typically requires allocation, copying new memory, causing unnecessary performance overhead and memory pressure.
  - **Solutions:** [[1]](#solution-slices)

### Problem: Strings

  - **Statement:** Null-terminated strings in C require `O(N)` traversal just to determine their length and are the root cause of most buffer overflow vulnerabilities.
  - **Solutions:** [[1]](#solution-strings)

### Problem: Vector

  - **Statement:** Statically sized C arrays cannot grow. Developers are forced to manually write capacity tracking, reallocation, and memory-moving boilerplate every time a dynamically sized collection is needed.
  - **Solutions:** [[1]](#solution-vector)

### Problem: Iterator

  - **Statement:** Different data structures and streams require entirely different logic loops for traversal. This forces developers to hardcode structural knowledge into their loops, making code rigid and algorithms untestable against generic inputs.
  - **Solutions:** [[1]](#solution-iterator)

### Problem: Table

  - **Statement:** Standard chaining or basic linear-probing hash maps suffer from severe cache misses, pointer chasing, and primary clustering. C lacks a native associative array, forcing developers to implement ad-hoc maps that degrade cache line efficiency and slow down CPU pipelines under high load.
  - **Solutions:** [[1]](#solution-table)

### Problem: Result

  - **Statement:** Standard C lacks mechanisms to strictly enforce return value checking, and frequently conflates expected logic branching (e.g., a missing table key) with systemic failures (e.g., out-of-memory). Returning magic values (`NULL`, `-1`) obscures the error origin and forces the caller to manually distinguish between an acceptable empty state and an application-halting crash, leading to unhandled edge cases.
  - **Solutions:** [[1]](#solution-result)

### Problem: Explicit Deferral

  - **Statement:** Functions with multiple return paths frequently leak memory or leave file handles open because developers forget to duplicate the cleanup code at every exit point.
  - **Solutions:** [[1]](#solution-explicit-deferral)

### Problem: Explicit Deinit

  - **Statement:** Data structures that own memory or system resources lose track of their cleanup requirements when passed around, forcing the programmer to remember exactly how to tear them down.
  - **Solutions:** [[1]](#solution-explicit-deinit)

### Problem: Files

  - **Statement:** Direct interaction with POSIX or Windows APIs creates platform-specific memory, file descriptor leaks, and inconsistent error codes throughout the core logic of an application.
  - **Solutions:** [[1]](#solution-files)

-----

## 2\. Proposed Solutions

### Solution: VTable

  - **Statement:** A VTable interface that abstracts memory requests. It allows any Camelot data structure to be instantiated with a heap allocator, a local arena, or a stack buffer, remaining completely agnostic to where the memory actually comes from.
  - **Implementations:** [[1]](#implementation-vtable)

### Solution: Arena

  - **Statement:** A contiguous block of memory that manages the lifetime of all objects allocated within a specific scope. It eliminates individual deallocations entirely, allowing the entire block to be reset instantly with a single pointer adjustment.
  - **Implementations:** [[1]](#implementation-arena)

### Solution: Primitives

  - **Statement:** A header that includes all standardized primitives alongside fixed naming conventions.
  - **Implementations:** [[1]](#implementation-primitives)

### Solution: Slices

  - **Statement:** A bounds-checked, fat-pointer slice representation (pointer + size). Slicing memory allows for zero-copy parsing and manipulation, completely decoupled from memory ownership.
  - **Implementations:** [[1]](#implementation-slices)

### Solution: Strings

  - **Statement:** A specialized Slice explicitly constrained to hold valid text. It provides `O(1)` length resolution, strict bounds checking, and completely ignores null terminators.
  - **Implementations:** [[1]](#implementation-strings)

### Solution: Vector

  - **Statement:** An allocator-aware struct that automatically manages its own capacity and resizing. It handles the underlying memory allocation while providing a safe API surface for access and mutation.
  - **Implementations:** [[1]](#implementation-vector)

### Solution: Iterator

  - **Statement:** A generic VTable interface abstracting traversal. It unifies loop logic via virtual dispatch, allowing generic algorithms to process sequential generic inputs entirely oblivious to the underlying memory layout.
  - **Implementations:** [[1]](#implementation-iterator)

### Solution: Table

  - **Statement:** A cache-optimized, allocator-backed hash map implementing the Swiss Table open-addressing algorithm. It segregates internal state by maintaining a continuous array of 8-bit control bytes (metadata) physically separated from the key-value data payload. This layout enables the framework to utilize SWAR (SIMD Within A Register) bitwise operations to evaluate up to 8 buckets simultaneously within a single L1 cache fetch, guaranteeing deterministic `O(1)` throughput.
  - **Trade-offs:** Requires a higher baseline memory footprint due to the parallel control array. Deletion logic complexity is elevated due to mandatory tombstone management to preserve probe sequences.
  - **Implementations:** [[1]](#implementation-table)

### Solution: Result

  - **Statement:** A tri-state tagged union (`OK`, `NIL`, `ERR`) marked with compiler attributes (`warn_unused_result`) to enforce ABI contracts. It physically separates the concept of expected absence (`NIL`) from systemic failure (`ERR`). The `ERR` state is paired with a domain-prefixed 32-bit integer error code, ensuring zero-collision error tracking across framework and application boundaries while sharing a zero-cost memory footprint with the success value via a `union`.
  - **Implementations:** [[1]](#implementation-result)

### Solution: Explicit Deferral

  - **Statement:** A strict architectural convention requiring all fallible functions to return through a single cleanup block via `goto`. It ensures deterministic resource release regardless of where the error occurred.
  - **Implementations:** [[1]](#implementation-explicit-deferral)

### Solution: Explicit Deinit

  - **Statement:** A mandatory, standardized destruction function paired with every owning type. It safely delegates the free operation back to the specific Allocator interface that created it.
  - **Implementations:** [[1]](#implementation-explicit-deinit)

### Solution: Files

  - **Statement:** A thin abstracted boundary layer that wraps OS-specific I/O directly into Camelot's Result types and Allocator interfaces, ensuring the framework's strict safety rules are applied whenever it touches data outside the system.
  - **Implementations:** [[1]](#implementation-files)

-----

## 3\. Implementation Details

### Implementation: VTable

  - **Verification:** [[1]](#test-vtable)

**Description:** Abstract base struct for memory allocation defining dynamic dispatch pointers.

```c
typedef struct Allocator Allocator;
struct Allocator {
    void* (*alloc)(Allocator* self, size_t size, size_t align);
    void  (*free)(Allocator* self, void* ptr, size_t size);
};
````

### Implementation: Arena

  - **Verification:** [[1]](test-arena)

**Description:** Pre-allocated memory block handling monotonic pointer bumping.

```c
typedef struct {
    Allocator base;
    u8* buffer;
    size_t capacity;
    size_t offset;
} Arena;

void* ARENA_alloc(Allocator* self, size_t size, size_t align);
void ARENA_reset(Arena* self);
```

### Implementation: Primitives

  - **Verification:** [[1]](test-primitives)

**Description:** Typedefs ensuring cross-platform deterministic byte sizing.

```c
#include <stdint.h>
#include <stdbool.h>
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;
```

### Implementation: Slices

  - **Verification:** [[1]](test-slices)

**Description:** Fat pointer for safe contiguous memory views.

```c
typedef struct {
    u8* ptr;
    size_t len;
} Slice;

Slice SLICE_new(u8* buffer, size_t len);
Slice SLICE_sub(Slice s, size_t offset, size_t len);
```

### Implementation: Strings

  - **Verification:** [[1]](test-strings)

**Description:** Immutable text slice wrapper.

```c
typedef Slice String;

String STRING_new(const char* literal, size_t len);
```

### Implementation: Vector

  - **Verification:** [[1]](test-vector)

**Description:** Dynamically resizing array coupled with a defined allocator.

```c
typedef struct {
    Allocator* alloc;
    void* data;
    size_t len;
    size_t cap;
    size_t stride;
} Vector;

Vector VECTOR_init(Allocator* alloc, size_t stride);
void VECTOR_push(Vector* vec, const void* item);
```

### Implementation: Iterator

  - **Verification:** [[1]](test-iterator)

**Description:** Abstract base interface guaranteeing forward-iterable structures using dynamic dispatch.

```c
typedef struct Iterator Iterator;
struct Iterator {
    void* (*next)(Iterator* self);
};

// Example concrete implementation wrapping a Vector
typedef struct {
    Iterator base;
    Vector* vec;
    size_t index;
} VectorIterator;

void VECTOR_ITERATOR_init(VectorIterator* self, Vector* vec);
```

### Implementation: Table

  - **Verification:** [[1]](test-table)

**Description:** Segregated metadata and data arrays to support vectorized probing.

```c
typedef struct {
    String key;
    void* value;
} TableEntry;

typedef struct {
    Allocator* alloc;
    u8* ctrl;
    TableEntry* slots;
    size_t len;
    size_t cap;
} Table;

Result TABLE_init(Allocator* alloc, size_t cap);
Result TABLE_set(Table* table, String key, void* value);
Result TABLE_get(Table* table, String key);
```

### Implementation: Result

  - **Verification:** [[1]](test-result)

**Description:** Domain-prefixed error codes and a strictly-typed tri-state union enforced by compiler warnings.

```c
#define DOMAIN_CAMELOT 0x00010000
#define DOMAIN_APP     0x00020000

#define ERR_OUT_OF_MEMORY (DOMAIN_CAMELOT | 0x0001)
#define ERR_FILE_ERROR    (DOMAIN_CAMELOT | 0x0002)
#define ERR_OUT_OF_BOUNDS (DOMAIN_CAMELOT | 0x0003)

typedef enum {
    OK,
    NIL,
    ERR
} State;

typedef struct __attribute__((warn_unused_result)) {
    State state;
    union {
        void* val;
        u32 err_code;
    } payload;
} Result;
```

### Implementation: Explicit Deferral

  - **Verification:** [[1]](test-explicit-deferral)

**Description:** Control flow convention mapping all exit paths to a single cleanup block.

```c
Result IO_file(Allocator* alloc, String path) {
    Result res = { .state = ERR, .payload.err_code = ERR_FILE_ERROR };
    void* buffer = alloc->alloc(alloc, 1024, 8);
    
    if (!buffer) { 
        res.payload.err_code = ERR_OUT_OF_MEMORY; 
        goto defer; 
    }
    
    res.state = OK;
    res.payload.val = buffer;
    
defer:
    if (res.state == ERR && buffer) {
        alloc->free(alloc, buffer, 1024);
    }
    return res;
}
```

### Implementation: Explicit Deinit

  - **Verification:** [[1]](test-explicit-deinit)

**Description:** Standardized teardown function mapping for resource-owning structs.

```c
void VECTOR_deinit(Vector* vec) {
    if (vec->data) {
        vec->alloc->free(vec->alloc, vec->data, vec->cap * vec->stride);
    }
    vec->len = 0;
    vec->cap = 0;
}
```

### Implementation: Files

  - **Verification:** [[1]](test-files)

**Description:** I/O subsystem wrapped in the Result architecture.

```c
Result IO_read(Allocator* alloc, String path);
Result IO_write(Allocator* alloc, String path, Slice data);
```

-----

## 4\. Testing and Validation

### Test: VTable

**Description:** Verify dynamic dispatch to multiple allocator implementations resolves correctly.

```c
Allocator* heap = ALLOCATOR_heap();
Allocator* arena = ALLOCATOR_arena(1024);
assert(heap->alloc != arena->alloc);
```

### Test: Arena

**Description:** Verify allocation bounds and monotonic increments; assert `reset` clears state instantly.

```c
void* ptr1 = ARENA_alloc(arena, 16, 8);
void* ptr2 = ARENA_alloc(arena, 16, 8);
assert(ptr2 == (u8*)ptr1 + 16);
ARENA_reset(arena);
assert(arena->offset == 0);
```

### Test: Primitives

**Description:** Assert correct static sizing on target architecture.

```c
_Static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
_Static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
_Static_assert(sizeof(b8) == 1, "b8 must be 1 byte");
```

### Test: Slices

**Description:** Verify bounds calculation and zero-copy property.

```c
Slice full = SLICE_new(buffer, 100);
Slice sub = SLICE_sub(full, 10, 20);
assert(sub.ptr == full.ptr + 10 && sub.len == 20);
```

### Test: Strings

**Description:** Verify strict length tracking.

```c
String s = STRING_new("test", 4);
assert(s.len == 4);
```

### Test: Vector

**Description:** Assert automatic reallocation scaling correctly moves elements.

```c
Vector v = VECTOR_init(alloc, sizeof(u32));
VECTOR_push(&v, &item);
assert(v.len == 1);
VECTOR_deinit(&v);
```

### Test: Iterator

**Description:** Assert generic forward traversal interface correctly dispatches to the underlying collection sequentially.

```c
VectorIterator vec_iter;
VECTOR_ITERATOR_init(&vec_iter, &v);

Iterator* iter = (Iterator*)&vec_iter;
void* el;
while ((el = iter->next(iter)) != NULL) {
    // Process element generically via vtable dispatch
}
```

### Test: Table

**Description:** Assert `O(1)` retrieval of values matching specific keys.

```c
Table t = TABLE_init(alloc);
TABLE_set(&t, str_key, val);
assert(TABLE_get(&t, str_key) == val);
```

### Test: Result

**Description:** Assert initialization, data insertion, and retrieval through the tri-state return architecture.

```c
Result res_init = TABLE_init(alloc, 16);
assert(res_init.state == OK);
Table* t = res_init.payload.val;

Result res_set = TABLE_set(t, str_key, val);
assert(res_set.state == OK);

Result res_get = TABLE_get(t, str_key);
assert(res_get.state == OK);
assert(res_get.payload.val == val);

Result res_miss = TABLE_get(t, missing_key);
assert(res_miss.state == NIL);
```

### Test: Explicit Deferral

**Description:** Assert early exits do not leave orphaned allocations.

```c
size_t alloc_count = 0;
IO_file_fail(mock_alloc, &alloc_count);
assert(alloc_count == 0); 
```

### Test: Explicit Deinit

**Description:** Assert the owning struct propagates the correct sizes to the original allocator's `free` function.

```c
Vector v = VECTOR_init(arena_base, sizeof(u8));
VECTOR_push(&v, &item);
VECTOR_deinit(&v);
assert(arena_base->freed_bytes == expected_bytes);
```

### Test: Files

**Description:** Assert mocked OS-level I/O failures correctly translate into `Result` types without crashing.

```c
Result r = IO_read(alloc, bad_path);
assert(r.state == ERR && r.payload.err_code == ERR_FILE_ERROR);
```

## 5\. Next Steps and Review

The implementation phase will commence after the final review and approval of this design document.

Following approval, a Trello issue tracker board will be provisioned to orchestrate development. To verify your active project assignments, check the issue tracker platform for the specific boards you have been granted access to or the code repository platform to see what projects you’ve been assigned to.
