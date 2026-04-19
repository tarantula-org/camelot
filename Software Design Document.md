# Software Design Document Template

> [!Important]
> All designs and implementations must strictly adhere to the [Yutila Security Policies](https://docs.google.com/document/d/1zqEZ9wyOiUj6hyH294iyZh9IXi8PnqnQC8UB7tnc0I0/edit?usp=sharing). Verify that your proposal incorporates the core architectural principles (Fail-Safe Defaults, Least Privilege, and Open Design) before submitting for review.

## Constraints

### Enterprise Reliability (The Java Paradigm)

Camelot prioritizes long-term enterprise reliability, cross-platform interoperability, and transaction stability over trendy syntactic sugar. Borrowing the survivability thesis of platforms like the JVM ("Write Once, Run Anywhere"), Camelot's abstractions are engineered to be predictably robust, backward-compatible, and rigorously tested so they can run undisturbed for decades in mission-critical environments.

### Libc Interoperability & Parameter Coherence

Camelot is a framework that aims to make libc coherent across platforms and bring modernity to C. It is **not** a replacement, but a massive aid.
1. **[Built-in Helpers]:** Camelot intrinsically embraces libc. For example, it is a waste of time to reinvent `printf`; instead, Camelot provides structured built-in helpers either to flawlessly bridge Camelot's components into libc, or to execute libc functionality implicitly wrapped safely by Camelot types.
2. **[Clarity over Magic]:** Any logic designed to interface with libc must be engineered to be explicitly clear, accessible, and trivial to understand.
3. **[Parameter Ordering]:** API endpoints must maintain absolute coherence in parameter layout. Every interface function must follow a deterministic sequence (e.g., `Allocator*` always first, object instance pointer second, primitive arguments third).

### C23 Architectural Standard

Camelot leverages the definitive state-of-the-art C23 standard to eradicate legacy bloat.
1. **[Primary Requirement]:** Legacy abstractions and macros (e.g., `<stdbool.h>`, `NULL`) and visually ugly "underscore-uppercase" compiler keywords (e.g., `_Bool`, `_Static_assert`, `_Alignas`) are strictly forbidden.
2. **[First-class Types]:** The codebase must globally standardize on native C23 utilities, notably: `bool`, `true`, `false`, `nullptr`, `constexpr`, `alignas`, and `typeof`.

### Design Coherence & Nomenclature

The Camelot framework enforces strict structural nomenclature rules to ensure unparalleled consistency.
1. **[Structure Namespaces]:** All external structures and functions must utilize the `NAMESPACE_function` formatting (e.g., prefix namespace fully uppercase, action suffix fully lowercase, connected by an underscore).
2. **[Node Unique Topologies]:** Internal components of robust data structures (such as link nodes) must be uniquely dedicated and explicitly named after their parent structure (e.g., `DLIST_Node`) to eliminate shadowing and structural ambiguity.
3. **[Prohibitions & Restrictions]:** Word truncations or casual abbreviations are prohibited unless they are universal acronyms (e.g., `IO`). 

### Portability & Compiler Extensions

To guarantee absolute portability across arbitrary C compilers and environments, reliance on non-standard runtime compiler extensions is explicitly prohibited.
1. **[Primary Requirement]:** The codebase must remain compatible with all major C compilers (e.g., MSVC, GCC, Clang).
2. **[Secondary Requirement]:** Compiler attributes that operate strictly during compilation, such as `[[nodiscard]]`, are acceptable and encouraged.
3. **[Prohibitions & Restrictions]:** Runtime-altering extensions, specifically GCC's `__attribute__((cleanup))`, are forbidden due to lack of support in non-GNU environments.

## 1. Design Diagram Overview

**Diagram Link:** [Epoch 1 Diagram]

## 2. Problems to be Solved

### Problem: Allocator agnosticism

  - **Statement:** Hardcoding `malloc` and `free` throughout a codebase creates rigid data structures. It prevents developers from swapping out memory strategies for testing, restricted environments, or performance optimization without rewriting the structure's internal logic.
  - **Solutions:** [[1]](#solution-vtable)

### Problem: Arena

  - **Statement:** Tracking and freeing thousands of individual object allocations leads to memory fragmentation, high CPU overhead, and inevitable memory leaks when a single free is forgotten.
  - **Solutions:** [[1]](#solution-arena)

### Problem: Primitives

  - **Statement:** Primitives are not standardized by default and their sizes vary depending on the architecture and compiler.
  - **Solutions:** [[1]](#solution-primitives)

### Problem: Slices

  - **Statement:** Standard C arrays decay into raw pointers when passed across function boundaries, dropping length metadata. This forces length arguments and reliance on unsafe assumptions. 
  - **Solutions:** [[1]](#solution-slices)

### Problem: Strings

  - **Statement:** Null-terminated strings in C require `O(N)` traversal just to determine their length and are the root cause of most buffer overflow vulnerabilities.
  - **Solutions:** [[1]](#solution-strings)

### Problem: Dynamic Array

  - **Statement:** Statically sized C arrays cannot grow. Developers are forced to manually write capacity tracking, reallocation, and memory-moving boilerplate. When arrays do reallocate, standard 2.0x growth multipliers mathematically prevent the allocator from ever recycling the previously freed array memory, aggressively compounding heap space and slowing down logic.
  - **Solutions:** [[1]](#solution-dynamic-array)

### Problem: Doubly Linked List

  - **Statement:** Standard contiguous collections force memory shifts for internal insertions and invalidate pointers upon resizing. A stable, sequential access collection is required where elements preserve absolute hardware addresses.
  - **Solutions:** [[1]](#solution-doubly-linked-list)

### Problem: Iterator

  - **Statement:** Client code is overwhelmed by explicit iterator tracking patterns or ad-hoc enhanced `for` loops. Because different structures demand different looping boundaries, developers must hardcode structural knowledge to iterate, tightly coupling algorithms to specific data shapes.
  - **Solutions:** [[1]](#solution-iterator)

### Problem: Table

  - **Statement:** Standard chaining or basic linear-probing hash maps suffer from severe cache misses, pointer chasing, and primary clustering. C lacks a native associative array, forcing developers to implement ad-hoc maps that degrade cache line efficiency and slow down CPU pipelines under high load.
  - **Solutions:** [[1]](#solution-table)

### Problem: Result

  - **Statement:** Standard C lacks mechanisms to strictly enforce return value checking, and frequently conflates expected logic branching (e.g., a missing table key) with systemic failures (e.g., out-of-memory). Returning magic values obscures the error origin and forces the caller to manually distinguish between an acceptable empty state and an application-halting crash.
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

## 3. Proposed Solutions

### Solution: VTable

  - **Statement:** A VTable interface that abstracts memory requests. It allows any Camelot data structure to be instantiated with a heap allocator, a local arena, or a stack buffer, remaining completely agnostic to where the memory actually comes from.
  - **Implementations:** [[1]](#implementation-vtable)

### Solution: Arena

  - **Statement:** A contiguous block of memory that manages the lifetime of all objects allocated within a specific scope. It eliminates individual deallocations entirely, allowing the entire block to be reset instantly with a single pointer adjustment.
  - **Implementations:** [[1]](#implementation-arena)

### Solution: Primitives

  - **Statement:** A header that includes all standardized primitives alongside fixed naming conventions, natively exploiting C23 `bool`.
  - **Implementations:** [[1]](#implementation-primitives)

### Solution: Slices

  - **Statement:** A bounds-checked, fat-pointer slice representation (pointer + size). Slicing memory allows for zero-copy parsing and manipulation.
  - **Implementations:** [[1]](#implementation-slices)

### Solution: Strings

  - **Statement:** A specialized Slice explicitly constrained to hold valid text. It provides `O(1)` length resolution, strict bounds checking, and ignores null terminators, while supporting Camelot's built-in libc conversion utilities.
  - **Implementations:** [[1]](#implementation-strings)

### Solution: Dynamic Array

  - **Statement:** A memory-safe `DynamicArray` interface that manages its buffer lifetime. For its optimal resizing strategy, the array utilizes a mathematically proven 1.5x capacity growth multiplier (calculated efficiently via bitwise right-shift addition `cap = cap + (cap >> 1)`). By growing at exactly 1.5x instead of the industry standard 2.0x, the sum of all previously discarded block allocations will eventually exceed the next requested capacity. Once this threshold is crossed, the host system's memory allocator can immediately coalesce and recycle the older blocks natively, achieving supreme optimal memory reduction without manual tuning.
  - **Implementations:** [[1]](#implementation-dynamic-array)

### Solution: Doubly Linked List

  - **Statement:** A bidirectional pointer-based chain structured as a `DList` with dedicated `DLIST_Node` components. It strictly guarantees hardware pointer stability (nodes never move during list modification) and maintains robust, O(1) insertion scaling across any midpoint.
  - **Implementations:** [[1]](#implementation-doubly-linked-list)

### Solution: Iterator

  - **Statement:** A generic VTable interface designed specifically to streamline explicit iterator patterns present in typical client code. Through dynamic virtual dispatch, developers can execute simplified looping schemas that inherently extract elements safely without evaluating specific capacity, limits, or boundary checks for different types.
  - **Implementations:** [[1]](#implementation-iterator)

### Solution: Table

  - **Statement:** A cache-optimized, allocator-backed hash map employing Swiss Table open-addressing. It segregates metadata into a continuous block from the payload, yielding heavy SIMD-friendly access logic.
  - **Trade-offs:** Requires a higher baseline memory footprint due to the parallel control array. Deletion logic complexity is elevated due to mandatory tombstone management to preserve probe sequences.
  - **Implementations:** [[1]](#implementation-table)

### Solution: Result

  - **Statement:** A tri-state tagged union (`OK`, `NIL`, `ERR`) marked with the native C23 `[[nodiscard]]` attribute. It distinctly uncouples intended logical absence (`NIL`) from system halting failures (`ERR`). Returns contain unique application domains, negating integer collision risks.
  - **Implementations:** [[1]](#implementation-result)

### Solution: Explicit Deferral

  - **Statement:** A strict architectural convention requiring all fallible functions to return through a single cleanup block via `goto`. It ensures deterministic resource release regardless of where the error occurred.
  - **Implementations:** [[1]](#implementation-explicit-deferral)

### Solution: Explicit Deinit

  - **Statement:** A mandatory, standardized destruction function paired with every owning type. It safely delegates the free operation back to the specific Allocator interface that created it.
  - **Implementations:** [[1]](#implementation-explicit-deinit)

### Solution: Files

  - **Statement:** An abstracted translation boundary wrapping POSIX/Windows IO seamlessly into Camelot's Result types to ensure the framework's strict safety rules apply.
  - **Implementations:** [[1]](#implementation-files)

-----

## 4. Implementation Details

### Implementation: VTable

  - **Verification:** [[1]](#test-vtable)

**Description:** Abstract base struct for memory allocation defining dynamic dispatch pointers.

```c
typedef struct Allocator Allocator;
struct Allocator {
    void* (*alloc)(Allocator* self, size_t size, size_t align);
    void  (*free)(Allocator* self, void* ptr, size_t size);
};
```

### Implementation: Arena

  - **Verification:** [[1]](#test-arena)

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

  - **Verification:** [[1]](#test-primitives)

**Description:** Typedefs ensuring cross-platform deterministic byte sizing explicitly supporting native C23 structures.

```c
#include <stdint.h>

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

  - **Verification:** [[1]](#test-slices)

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

  - **Verification:** [[1]](#test-strings)

**Description:** Immutable text slice wrapper designed to seamlessly transform via built-in bridge macros/helpers when exposed to libc components.

```c
typedef Slice String;

String STRING_new(const char* literal, size_t len);
```

### Implementation: Dynamic Array

  - **Verification:** [[1]](#test-dynamic-array)

**Description:** Dynamically scalable array utilizing optimal 1.5x expansion logic to gracefully recycle allocations under heap pressure.

```c
typedef struct {
    Allocator* alloc;
    void* data;
    size_t len;
    size_t cap;
    size_t stride;
} DynamicArray;

DynamicArray DARRAY_init(Allocator* alloc, size_t stride);
void DARRAY_push(DynamicArray* arr, const void* item);
```

### Implementation: Doubly Linked List

  - **Verification:** [[1]](#test-doubly-linked-list)

**Description:** Segregated explicitly named structure node chain mapping strictly identical hardware pointers.

```c
typedef struct DLIST_Node DLIST_Node;
struct DLIST_Node {
    DLIST_Node* next;
    DLIST_Node* prev;
    void* value;
};

typedef struct {
    Allocator* alloc;
    DLIST_Node* head;
    DLIST_Node* tail;
    size_t len;
} DList;

DList DLIST_init(Allocator* alloc);
void DLIST_append(DList* list, void* value);
```

### Implementation: Iterator

  - **Verification:** [[1]](#test-iterator)

**Description:** Generic interface streamlining iterations and erasing underlying explicit indexing requirements.

```c
typedef struct Iterator Iterator;
struct Iterator {
    void* (*next)(Iterator* self);
};

// Example concrete implementation wrapping a DynamicArray
typedef struct {
    Iterator base;
    DynamicArray* arr;
    size_t index;
} DARRAY_Iterator;

void DARRAY_ITERATOR_init(DARRAY_Iterator* self, DynamicArray* arr);
```

### Implementation: Table

  - **Verification:** [[1]](#test-table)

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

  - **Verification:** [[1]](#test-result)

**Description:** Domain-prefixed error codes mapped into an ABI-protected C23 union structure.

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

typedef struct [[nodiscard]] {
    State state;
    union {
        void* val;
        u32 err_code;
    } payload;
} Result;
```

### Implementation: Explicit Deferral

  - **Verification:** [[1]](#test-explicit-deferral)

**Description:** Control flow convention mapping all exit paths to a single cleanup block via `goto`.

```c
Result IO_file(Allocator* alloc, String path) {
    Result res = { .state = ERR, .payload.err_code = ERR_FILE_ERROR };
    void* buffer = alloc->alloc(alloc, 1024, 8);
    
    if (buffer == nullptr) { 
        res.payload.err_code = ERR_OUT_OF_MEMORY; 
        goto defer; 
    }
    
    res.state = OK;
    res.payload.val = buffer;
    
defer:
    if (res.state == ERR && buffer != nullptr) {
        alloc->free(alloc, buffer, 1024);
    }
    return res;
}
```

### Implementation: Explicit Deinit

  - **Verification:** [[1]](#test-explicit-deinit)

**Description:** Standardized teardown function yielding identical allocations back to generic `Allocator`.

```c
void DARRAY_deinit(DynamicArray* arr) {
    if (arr->data != nullptr) {
        arr->alloc->free(arr->alloc, arr->data, arr->cap * arr->stride);
    }
    arr->len = 0;
    arr->cap = 0;
}
```

### Implementation: Files

  - **Verification:** [[1]](#test-files)

**Description:** I/O subsystem wrapped seamlessly within the Result tri-state architecture.

```c
Result IO_read(Allocator* alloc, String path);
Result IO_write(Allocator* alloc, String path, Slice data);
```

-----

## 5. Testing and Validation

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

**Description:** Assert correct static sizing on target architecture utilizing C23 features.

```c
static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
static_assert(sizeof(bool) == 1, "bool must be 1 byte");
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

### Test: Dynamic Array

**Description:** Assert scalable heap capacity correctly offsets elements safely via allocation strategies.

```c
DynamicArray arr = DARRAY_init(alloc, sizeof(u32));
DARRAY_push(&arr, &item);
assert(arr.len == 1);
DARRAY_deinit(&arr);
```

### Test: Doubly Linked List

**Description:** Assert node traversal stability.

```c
DList list = DLIST_init(alloc);
DLIST_append(&list, &item);
assert(list.head == list.tail && list.head != nullptr);
assert(list.len == 1);
```

### Test: Iterator

**Description:** By simplifying typical explicit iteration pattern boilerplate, developers cleanly process generic sequences decoupled from size logic completely.

```c
DARRAY_Iterator arr_iter;
DARRAY_ITERATOR_init(&arr_iter, &arr);

Iterator* iter = (Iterator*)&arr_iter;
void* el;
while ((el = iter->next(iter)) != nullptr) {
    // Process element cleanly via dynamically dispatched sequence without manual bounds tracking
}
```

### Test: Table

**Description:** Assert `O(1)` retrieval of values matching specific keys.

```c
Table t = TABLE_init(alloc, 16);
TABLE_set(&t, str_key, val);
assert(TABLE_get(&t, str_key).payload.val == val);
```

### Test: Result

**Description:** Assert strict type separation without implicit collisions or application halting via the tri-state design.

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

**Description:** Assert the owning struct propagates identical footprints exactly matching its generation logic.

```c
DynamicArray arr = DARRAY_init(arena_base, sizeof(u8));
DARRAY_push(&arr, &item);
DARRAY_deinit(&arr);
assert(arena_base->freed_bytes == expected_bytes);
```

### Test: Files

**Description:** Assert mocked OS-level I/O failures correctly translate into `Result` types without crashing.

```c
Result r = IO_read(alloc, bad_path);
assert(r.state == ERR && r.payload.err_code == ERR_FILE_ERROR);
```

## 6. Next Steps and Review

The implementation phase will commence after the final review and approval of this design document.

Following approval, a Trello issue tracker board will be provisioned to orchestrate development. To verify your active project assignments, check the issue tracker platform for the specific boards you have been granted access to or the code repository platform to see what projects you’ve been assigned to.