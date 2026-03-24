#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

// =============================================================================
// MEMORY TESTING TOOLS
// =============================================================================

// Memory allocation tracking
typedef struct {
    void *ptr;
    size_t size;
    const char *file;
    int line;
    time_t timestamp;
} AllocationRecord;

#define MAX_ALLOCATIONS 10000
static AllocationRecord allocations[MAX_ALLOCATIONS];
static int allocationCount = 0;
static size_t totalAllocated = 0;
static size_t peakAllocated = 0;

// =============================================================================
// MEMORY LEAK DETECTION
// =============================================================================

// Custom malloc with tracking
void* trackedMalloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (!ptr) return NULL;
    
    if (allocationCount < MAX_ALLOCATIONS) {
        allocations[allocationCount].ptr = ptr;
        allocations[allocationCount].size = size;
        allocations[allocationCount].file = file;
        allocations[allocationCount].line = line;
        allocations[allocationCount].timestamp = time(NULL);
        allocationCount++;
    }
    
    totalAllocated += size;
    if (totalAllocated > peakAllocated) {
        peakAllocated = totalAllocated;
    }
    
    return ptr;
}

// Custom free with tracking
void trackedFree(void *ptr, const char *file, int line) {
    if (!ptr) return;
    
    // Find and remove allocation record
    for (int i = 0; i < allocationCount; i++) {
        if (allocations[i].ptr == ptr) {
            totalAllocated -= allocations[i].size;
            
            // Shift remaining records
            for (int j = i; j < allocationCount - 1; j++) {
                allocations[j] = allocations[j + 1];
            }
            allocationCount--;
            break;
        }
    }
    
    free(ptr);
}

// Macros for automatic file/line tracking
#define MALLOC(size) trackedMalloc(size, __FILE__, __LINE__)
#define FREE(ptr) trackedFree(ptr, __FILE__, __LINE__)

// Print memory leak report
void printMemoryLeaks() {
    if (allocationCount == 0) {
        printf("✓ No memory leaks detected!\n");
        return;
    }
    
    printf("❌ MEMORY LEAKS DETECTED!\n");
    printf("Total leaked allocations: %d\n", allocationCount);
    printf("Total leaked memory: %zu bytes\n", totalAllocated);
    
    printf("\nLeak Details:\n");
    printf("Address\t\tSize\tFile:Line\n");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < allocationCount; i++) {
        printf("%p\t%zu\t%s:%d\n", 
               allocations[i].ptr, 
               allocations[i].size, 
               allocations[i].file, 
               allocations[i].line);
    }
}

// Print memory statistics
void printMemoryStats() {
    printf("=== MEMORY STATISTICS ===\n");
    printf("Current allocations: %d\n", allocationCount);
    printf("Currently allocated: %zu bytes\n", totalAllocated);
    printf("Peak allocated: %zu bytes\n", peakAllocated);
    printf("Allocation tracking capacity: %d/%d\n", allocationCount, MAX_ALLOCATIONS);
    printf("\n");
}

// =============================================================================
// BUFFER OVERFLOW DETECTION
// =============================================================================

// Safe buffer with canary values
typedef struct {
    char *data;
    size_t size;
    unsigned int canary_start;
    unsigned int canary_end;
} SafeBuffer;

// Canary values for overflow detection
#define CANARY_VALUE 0xDEADBEEF

// Create safe buffer
SafeBuffer* createSafeBuffer(size_t size) {
    SafeBuffer *buffer = (SafeBuffer*)malloc(sizeof(SafeBuffer));
    if (!buffer) return NULL;
    
    buffer->data = (char*)malloc(size);
    if (!buffer->data) {
        free(buffer);
        return NULL;
    }
    
    buffer->size = size;
    buffer->canary_start = CANARY_VALUE;
    buffer->canary_end = CANARY_VALUE;
    
    return buffer;
}

// Check buffer integrity
int checkBufferIntegrity(SafeBuffer *buffer) {
    if (!buffer) return 0;
    
    if (buffer->canary_start != CANARY_VALUE) {
        printf("Buffer underflow detected!\n");
        return 0;
    }
    
    if (buffer->canary_end != CANARY_VALUE) {
        printf("Buffer overflow detected!\n");
        return 0;
    }
    
    return 1;
}

// Write to safe buffer with bounds checking
int safeBufferWrite(SafeBuffer *buffer, size_t offset, const char *data, size_t size) {
    if (!buffer || !buffer->data) return 0;
    
    if (offset + size > buffer->size) {
        printf("Buffer write overflow: offset=%zu, size=%zu, buffer_size=%zu\n", 
               offset, size, buffer->size);
        return 0;
    }
    
    memcpy(buffer->data + offset, data, size);
    return 1;
}

// Read from safe buffer with bounds checking
int safeBufferRead(SafeBuffer *buffer, size_t offset, char *data, size_t size) {
    if (!buffer || !buffer->data || !data) return 0;
    
    if (offset + size > buffer->size) {
        printf("Buffer read overflow: offset=%zu, size=%zu, buffer_size=%zu\n", 
               offset, size, buffer->size);
        return 0;
    }
    
    memcpy(data, buffer->data + offset, size);
    return 1;
}

// Free safe buffer
void freeSafeBuffer(SafeBuffer *buffer) {
    if (buffer) {
        if (buffer->data) {
            free(buffer->data);
        }
        free(buffer);
    }
}

// =============================================================================
// MEMORY STRESS TESTING
// =============================================================================

// Memory allocation stress test
void stressTestMemory(int iterations, size_t maxSize) {
    printf("=== MEMORY STRESS TEST ===\n");
    printf("Iterations: %d, Max size: %zu bytes\n", iterations, maxSize);
    
    void **ptrs = (void**)malloc(iterations * sizeof(void*));
    if (!ptrs) {
        printf("Failed to allocate pointer array\n");
        return;
    }
    
    clock_t start = clock();
    int successfulAllocs = 0;
    int failedAllocs = 0;
    
    for (int i = 0; i < iterations; i++) {
        size_t size = (rand() % maxSize) + 1;
        ptrs[i] = malloc(size);
        
        if (ptrs[i]) {
            // Write pattern to memory
            memset(ptrs[i], 0xAA, size);
            successfulAllocs++;
        } else {
            failedAllocs++;
        }
    }
    
    // Verify memory contents
    int verificationErrors = 0;
    for (int i = 0; i < iterations; i++) {
        if (ptrs[i]) {
            size_t size = (rand() % maxSize) + 1;
            char *ptr = (char*)ptrs[i];
            
            // Check first and last bytes
            if (ptr[0] != 0xAA || ptr[size-1] != 0xAA) {
                verificationErrors++;
            }
        }
    }
    
    // Free all memory
    for (int i = 0; i < iterations; i++) {
        if (ptrs[i]) {
            free(ptrs[i]);
        }
    }
    
    clock_t end = clock();
    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Results:\n");
    printf("Successful allocations: %d\n", successfulAllocs);
    printf("Failed allocations: %d\n", failedAllocs);
    printf("Verification errors: %d\n", verificationErrors);
    printf("Time taken: %f seconds\n", time);
    printf("Allocations per second: %.0f\n", iterations / time);
    
    free(ptrs);
    printf("\n");
}

// Memory fragmentation test
void testMemoryFragmentation() {
    printf("=== MEMORY FRAGMENTATION TEST ===\n");
    
    const int numAllocs = 1000;
    void *ptrs[numAllocs];
    int sizes[numAllocs];
    
    // Allocate various sizes
    for (int i = 0; i < numAllocs; i++) {
        sizes[i] = (rand() % 1000) + 64;
        ptrs[i] = malloc(sizes[i]);
    }
    
    // Free every other allocation (creates fragmentation)
    for (int i = 0; i < numAllocs; i += 2) {
        if (ptrs[i]) {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
    
    // Try to allocate large blocks in fragmented memory
    int successfulLargeAllocs = 0;
    for (int i = 0; i < 100; i++) {
        void *ptr = malloc(2048); // Larger than most freed blocks
        if (ptr) {
            successfulLargeAllocs++;
            free(ptr);
        }
    }
    
    // Free remaining memory
    for (int i = 1; i < numAllocs; i += 2) {
        if (ptrs[i]) {
            free(ptrs[i]);
        }
    }
    
    printf("Large allocations in fragmented memory: %d/100\n", successfulLargeAllocs);
    printf("Fragmentation impact: %s\n", 
           successfulLargeAllocs < 50 ? "High" : "Low");
    printf("\n");
}

// =============================================================================
// MEMORY BENCHMARKING
// =============================================================================

// Benchmark malloc/free performance
void benchmarkMallocFree(int iterations, size_t size) {
    printf("=== MALLOC/FREE BENCHMARK ===\n");
    printf("Iterations: %d, Size: %zu bytes\n", iterations, size);
    
    void **ptrs = (void**)malloc(iterations * sizeof(void*));
    if (!ptrs) return;
    
    // Benchmark allocation
    clock_t start = clock();
    for (int i = 0; i < iterations; i++) {
        ptrs[i] = malloc(size);
    }
    clock_t allocEnd = clock();
    
    // Benchmark deallocation
    clock_t freeStart = clock();
    for (int i = 0; i < iterations; i++) {
        if (ptrs[i]) free(ptrs[i]);
    }
    clock_t freeEnd = clock();
    
    double allocTime = ((double)(allocEnd - start)) / CLOCKS_PER_SEC;
    double freeTime = ((double)(freeEnd - freeStart)) / CLOCKS_PER_SEC;
    
    printf("Allocation time: %f seconds (%.0f allocs/sec)\n", 
           allocTime, iterations / allocTime);
    printf("Deallocation time: %f seconds (%.0f frees/sec)\n", 
           freeTime, iterations / freeTime);
    printf("Total time: %f seconds\n", allocTime + freeTime);
    
    free(ptrs);
    printf("\n");
}

// Benchmark memory access patterns
void benchmarkMemoryAccess(size_t size, int iterations) {
    printf("=== MEMORY ACCESS BENCHMARK ===\n");
    printf("Buffer size: %zu bytes, Iterations: %d\n", size, iterations);
    
    char *buffer = (char*)malloc(size);
    if (!buffer) return;
    
    // Sequential access
    clock_t start = clock();
    for (int i = 0; i < iterations; i++) {
        for (size_t j = 0; j < size; j++) {
            buffer[j] = (char)j;
        }
    }
    clock_t seqEnd = clock();
    
    // Random access
    srand(42); // Fixed seed for reproducibility
    clock_t randStart = clock();
    for (int i = 0; i < iterations; i++) {
        for (size_t j = 0; j < size; j++) {
            size_t index = rand() % size;
            buffer[index] = (char)index;
        }
    }
    clock_t randEnd = clock();
    
    double seqTime = ((double)(seqEnd - start)) / CLOCKS_PER_SEC;
    double randTime = ((double)(randEnd - randStart)) / CLOCKS_PER_SEC;
    
    printf("Sequential access: %f seconds\n", seqTime);
    printf("Random access: %f seconds\n", randTime);
    printf("Random/Sequential ratio: %.2fx\n", randTime / seqTime);
    
    free(buffer);
    printf("\n");
}

// =============================================================================
// MEMORY USAGE ANALYSIS
// =============================================================================

// Analyze memory usage patterns
void analyzeMemoryUsage() {
    printf("=== MEMORY USAGE ANALYSIS ===\n");
    
    // Get current allocation statistics
    printf("Current memory usage:\n");
    printf("Active allocations: %d\n", allocationCount);
    printf("Total allocated: %zu bytes\n", totalAllocated);
    printf("Peak allocated: %zu bytes\n", peakAllocated);
    
    if (allocationCount > 0) {
        printf("\nAllocation size distribution:\n");
        
        size_t sizeBuckets[5] = {0}; // <64, <256, <1024, <4096, >=4096
        for (int i = 0; i < allocationCount; i++) {
            size_t size = allocations[i].size;
            if (size < 64) sizeBuckets[0]++;
            else if (size < 256) sizeBuckets[1]++;
            else if (size < 1024) sizeBuckets[2]++;
            else if (size < 4096) sizeBuckets[3]++;
            else sizeBuckets[4]++;
        }
        
        printf("  <64 bytes: %zu\n", sizeBuckets[0]);
        printf("  64-255 bytes: %zu\n", sizeBuckets[1]);
        printf("  256-1023 bytes: %zu\n", sizeBuckets[2]);
        printf("  1024-4095 bytes: %zu\n", sizeBuckets[3]);
        printf("  >=4096 bytes: %zu\n", sizeBuckets[4]);
        
        // Calculate average allocation size
        size_t totalSize = 0;
        for (int i = 0; i < allocationCount; i++) {
            totalSize += allocations[i].size;
        }
        
        if (allocationCount > 0) {
            printf("Average allocation size: %.1f bytes\n", 
                   (double)totalSize / allocationCount);
        }
    }
    
    printf("\n");
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateMemoryLeakDetection() {
    printf("=== MEMORY LEAK DETECTION DEMO ===\n");
    
    // Reset tracking
    allocationCount = 0;
    totalAllocated = 0;
    peakAllocated = 0;
    
    // Create some allocations
    void *ptr1 = MALLOC(100);
    void *ptr2 = MALLOC(200);
    void *ptr3 = MALLOC(300);
    
    // Free some (leak others intentionally)
    FREE(ptr2);
    
    printMemoryStats();
    printMemoryLeaks();
    
    // Clean up remaining
    FREE(ptr1);
    FREE(ptr3);
    
    printMemoryLeaks();
    printf("\n");
}

void demonstrateBufferOverflowDetection() {
    printf("=== BUFFER OVERFLOW DETECTION DEMO ===\n");
    
    SafeBuffer *buffer = createSafeBuffer(100);
    if (!buffer) {
        printf("Failed to create buffer\n");
        return;
    }
    
    // Normal write
    char data[] = "Hello, World!";
    if (safeBufferWrite(buffer, 0, data, strlen(data))) {
        printf("Normal write successful\n");
    }
    
    // Check integrity
    if (checkBufferIntegrity(buffer)) {
        printf("Buffer integrity: OK\n");
    }
    
    // Attempt overflow write
    char largeData[200];
    memset(largeData, 'A', sizeof(largeData));
    if (!safeBufferWrite(buffer, 0, largeData, sizeof(largeData))) {
        printf("Overflow write correctly blocked\n");
    }
    
    // Check integrity after attempted overflow
    if (checkBufferIntegrity(buffer)) {
        printf("Buffer integrity still OK\n");
    }
    
    freeSafeBuffer(buffer);
    printf("\n");
}

void demonstrateMemoryBenchmarks() {
    printf("=== MEMORY BENCHMARKS ===\n");
    
    // Different allocation sizes
    benchmarkMallocFree(10000, 64);
    benchmarkMallocFree(5000, 1024);
    benchmarkMallocFree(1000, 4096);
    
    // Memory access patterns
    benchmarkMemoryAccess(1024 * 1024, 10); // 1MB buffer
    benchmarkMemoryAccess(64 * 1024, 100);   // 64KB buffer
}

void demonstrateStressTests() {
    printf("=== STRESS TESTS ===\n");
    
    stressTestMemory(1000, 1024);
    testMemoryFragmentation();
}

int main() {
    printf("Memory Testing Tools\n");
    printf("===================\n\n");
    
    // Run all demonstrations
    demonstrateMemoryLeakDetection();
    demonstrateBufferOverflowDetection();
    demonstrateMemoryBenchmarks();
    demonstrateStressTests();
    analyzeMemoryUsage();
    
    printf("All memory testing tools demonstrated!\n");
    return 0;
}
