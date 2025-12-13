#include "include/camelot.h"

int main() {
    printf("--- CAMELOT ALLOCATION TEST ---\n");

    // 1. Initialize Arena (1024 bytes total)
    ArenaResult sys = c_arena_create(1024);
    if (sys.error != C_OK) {
        printf("System Init Failed!\n");
        return 1;
    }
    Arena *arena = sys.unwrap;
    printf("[*] Arena Online: 1024 bytes capacity.\n");

    // --- TEST 1: Valid Allocation ---
    printf("\n[TEST 1] Allocating 2 integers...\n");
    
    // We get a result struct back
    AllocResult res1 = c_new(arena, int, 2);

    if (res1.error == C_OK) {
        // Success: We must manually cast the void* to int*
        int *nums = (int *)res1.unwrap;
        nums[0] = 42;
        nums[1] = 99;
        printf("    [PASS] Allocation Success. Data: [%d, %d]\n", nums[0], nums[1]);
    } else {
        printf("    [FAIL] Unexpected allocation failure!\n");
    }

    // --- TEST 2: Intentional OOM ---
    printf("\n[TEST 2] Attempting to allocate 10,000 bytes (Should Fail)...\n");
    
    AllocResult res2 = c_new(arena, char, 10000);

    if (res2.error == C_OOM) {
        printf("    [PASS] System correctly blocked the allocation (Error: C_OOM)\n");
    } else {
        printf("    [FAIL] System allowed the allocation or gave wrong error!\n");
    }

    // --- TEST 3: The Reset Superpower ---
    printf("\n[TEST 3] Testing Arena Reset (Reuse)...\n");

    // A. Fill the arena (Capturing the result to satisfy [[nodiscard]])
    AllocResult fill_res = c_new(arena, int, 100); 
    
    if (fill_res.error == C_OK) {
        printf("    [INFO] Used before reset: %zu bytes\n", arena->offset);
    } else {
        printf("    [FAIL] Setup failed (OOM during fill).\n");
        return 1;
    }

    // B. Reset
    c_arena_reset(arena);
    printf("    [ACTION] Arena Reset Triggered.\n");

    // C. Verify
    if (arena->offset == 0) {
        printf("    [PASS] Offset is 0. Memory is ready for reuse.\n");
    } else {
        printf("    [FAIL] Offset is not 0!\n");
    }

    // D. Allocate again (Overwriting old data)
    AllocResult res3 = c_new(arena, int, 1);
    if (res3.error == C_OK) {
        printf("    [PASS] Re-allocation successful at start of buffer (%p)\n", res3.unwrap);
        
        // Verify it points to the start of the buffer
        if (res3.unwrap == (void*)((uintptr_t)arena->buffer)) {
             printf("    [PASS] Pointer address matches buffer start.\n");
        }
    }

    // --- TEST 4: Temporary Scopes (Time Travel) ---
    printf("\n[TEST 4] Testing Temporary Scopes...\n");
    c_arena_reset(arena); // Start clean

    // A. Permanent Allocation (Level Data)
    AllocResult p_res = c_new(arena, int, 10); // 40 bytes
    size_t permanent_offset = arena->offset;
    printf("    [INFO] Base Offset: %zu bytes\n", permanent_offset);

    // B. Begin Scope (Start Scratchpad)
    {
        TempArena scratch = c_temp_begin(arena);
        
        // C. Allocate Temp Garbage
        c_new(arena, double, 100); // 800 bytes
        printf("    [INFO] Offset inside scope: %zu bytes\n", arena->offset);

        // Verify we actually moved forward
        if (arena->offset <= permanent_offset) {
            printf("    [FAIL] Pointer didn't move inside scope!\n");
        }

        // D. End Scope (Rewind)
        c_temp_end(scratch);
    }

    // E. Verify Rewind
    printf("    [INFO] Offset after scope: %zu bytes\n", arena->offset);
    
    if (arena->offset == permanent_offset) {
        printf("    [PASS] Time Travel Successful. Memory rolled back perfectly.\n");
    } else {
        printf("    [FAIL] Memory leak! Offset is %zu, expected %zu\n", arena->offset, permanent_offset);
    }

    // ... inside main ...

    // --- TEST 5: The Resize Optimization ---
    printf("\n[TEST 5] Testing Resize Optimization...\n");
    c_arena_reset(arena);

    // A. Create an "Obstacle" (Something allocated first)
    // We cast to (void) to ignore the result since we assume success for this test
    (void)c_new(arena, int, 10); 

    // B. Create our "Dynamic Array" (The latest allocation)
    AllocResult arr_res = c_new(arena, int, 2);
    int *arr = (int *)arr_res.unwrap;
    arr[0] = 100; 
    arr[1] = 200;
    printf("    [INFO] Original Array: %p (Values: %d, %d)\n", (void*)arr, arr[0], arr[1]);

    // --- CASE 1: FAST PATH (In-Place) ---
    // Since 'arr' is at the top, this should NOT move the pointer.
    printf("    [ACTION] Expanding array in-place (Fast Path)...\n");
    AllocResult resize_1 = c_resize_array(arena, arr, int, 2, 4);
    
    if (resize_1.unwrap == arr) {
        printf("    [PASS] Pointer address stayed the same! (Zero Copy)\n");
    } else {
        printf("    [FAIL] Pointer moved! Optimization failed.\n");
    }

    // --- CASE 2: SLOW PATH (Force Move) ---
    // 1. Allocate something NEW on top, burying our array.
    (void)c_new(arena, double, 1);
    printf("    [ACTION] Allocated new object. Array is now 'buried'.\n");

    // 2. Try to resize the array again. It MUST move now.
    printf("    [ACTION] Expanding buried array (Slow Path)...\n");
    AllocResult resize_2 = c_resize_array(arena, arr, int, 4, 8);
    
    if (resize_2.unwrap != arr) {
        printf("    [PASS] Pointer moved to %p (Data preserved)\n", resize_2.unwrap);
        
        // Verify Data Integrity
        int *new_arr = (int *)resize_2.unwrap;
        if (new_arr[0] == 100 && new_arr[1] == 200) {
            printf("    [PASS] Data copied correctly.\n");
        } else {
            printf("    [FAIL] Data corruption detected!\n");
        }
    } else {
        printf("    [FAIL] Pointer didn't move! We overwrote the next object!\n");
    }

    c_arena_destroy(arena);
    return 0;
}