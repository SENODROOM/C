# Memory Testing Tools

This file contains comprehensive memory testing utilities for C programs, including memory leak detection, buffer overflow detection, stress testing, performance benchmarking, and memory usage analysis.

## 📚 Testing Tool Categories

### 🔍 Memory Leak Detection
Track allocations and detect unfreed memory

### 🛡️ Buffer Overflow Detection
Safe buffer operations with integrity checking

### 💪 Stress Testing
Memory allocation and fragmentation testing

### ⚡ Performance Benchmarking
 malloc/free and memory access performance

### 📊 Usage Analysis
Memory usage patterns and statistics

## 🔍 Memory Leak Detection

### Tracked Allocation System
Custom malloc/free wrappers that track all allocations:

```c
void* trackedMalloc(size_t size, const char *file, int line);
void trackedFree(void *ptr, const char *file, int line);
```

**Features**:
- Automatic file/line tracking
- Allocation size recording
- Timestamp tracking
- Peak memory usage monitoring

**Usage**:
```c
#define MALLOC(size) trackedMalloc(size, __FILE__, __LINE__)
#define FREE(ptr) trackedFree(ptr, __FILE__, __LINE__)

void *ptr = MALLOC(100);
FREE(ptr);
```

### Leak Reporting
```c
void printMemoryLeaks();
```

**Report Format**:
```
❌ MEMORY LEAKS DETECTED!
Total leaked allocations: 2
Total leaked memory: 400 bytes

Leak Details:
Address          Size    File:Line
----------------------------------------
0x55d4f5b2e2b0   100     main.c:42
0x55d4f5b2e3c0   300     main.c:43
```

### Memory Statistics
```c
void printMemoryStats();
```

**Statistics Provided**:
- Current allocation count
- Currently allocated memory
- Peak allocated memory
- Tracking capacity usage

## 🛡️ Buffer Overflow Detection

### Safe Buffer Structure
Protected buffer with canary values:

```c
typedef struct {
    char *data;
    size_t size;
    unsigned int canary_start;
    unsigned int canary_end;
} SafeBuffer;
```

**Canary Values**:
- Magic numbers (0xDEADBEEF)
- Placed before and after buffer
- Checked for integrity violations

### Safe Buffer Operations
```c
SafeBuffer* createSafeBuffer(size_t size);
int checkBufferIntegrity(SafeBuffer *buffer);
int safeBufferWrite(SafeBuffer *buffer, size_t offset, const char *data, size_t size);
int safeBufferRead(SafeBuffer *buffer, size_t offset, char *data, size_t size);
void freeSafeBuffer(SafeBuffer *buffer);
```

**Protection Features**:
- Bounds checking on all operations
- Automatic overflow/underflow detection
- Detailed error reporting
- Memory cleanup on destruction

## 💪 Stress Testing

### Memory Allocation Stress Test
```c
void stressTestMemory(int iterations, size_t maxSize);
```

**Test Scenarios**:
- Random size allocations
- Memory content verification
- Performance measurement
- Failure handling

**Metrics Collected**:
- Successful/failed allocations
- Verification errors
- Time performance
- Allocation rate

### Fragmentation Test
```c
void testMemoryFragmentation();
```

**Test Process**:
1. Allocate various sized blocks
2. Free alternating blocks (creates fragmentation)
3. Attempt large allocations in fragmented memory
4. Measure fragmentation impact

**Fragmentation Indicators**:
- Large allocation success rate
- Memory waste percentage
- Allocation pattern efficiency

## ⚡ Performance Benchmarking

### malloc/free Benchmark
```c
void benchmarkMallocFree(int iterations, size_t size);
```

**Performance Metrics**:
- Allocation time (allocations/second)
- Deallocation time (frees/second)
- Total throughput
- Memory overhead

**Benchmark Results**:
```
Iterations: 10000, Size: 1024 bytes
Allocation time: 0.002345 seconds (4265324 allocs/sec)
Deallocation time: 0.001234 seconds (8103727 frees/sec)
Total time: 0.003579 seconds
```

### Memory Access Benchmark
```c
void benchmarkMemoryAccess(size_t size, int iterations);
```

**Access Patterns**:
- **Sequential**: Linear memory access
- **Random**: Random memory access
- **Cache efficiency**: Cache line utilization

**Performance Comparison**:
```
Buffer size: 1048576 bytes, Iterations: 10
Sequential access: 0.123456 seconds
Random access: 0.456789 seconds
Random/Sequential ratio: 3.70x
```

## 📊 Memory Usage Analysis

### Usage Statistics
```c
void analyzeMemoryUsage();
```

**Analysis Features**:
- Current allocation count
- Total allocated memory
- Peak memory usage
- Size distribution analysis

**Size Distribution Buckets**:
- < 64 bytes
- 64-255 bytes
- 256-1023 bytes
- 1024-4095 bytes
- ≥ 4096 bytes

**Sample Output**:
```
Current memory usage:
Active allocations: 15
Total allocated: 8192 bytes
Peak allocated: 16384 bytes

Allocation size distribution:
  <64 bytes: 5
  64-255 bytes: 7
  256-1023 bytes: 2
  1024-4095 bytes: 1
  >=4096 bytes: 0
Average allocation size: 546.1 bytes
```

## 💡 Implementation Details

### Allocation Tracking
```c
typedef struct {
    void *ptr;
    size_t size;
    const char *file;
    int line;
    time_t timestamp;
} AllocationRecord;

static AllocationRecord allocations[MAX_ALLOCATIONS];
static int allocationCount = 0;
static size_t totalAllocated = 0;
static size_t peakAllocated = 0;
```

**Key Features**:
- Fixed-size tracking array (configurable)
- O(1) allocation recording
- O(n) deallocation lookup
- Constant-time statistics

### Canary-Based Protection
```c
#define CANARY_VALUE 0xDEADBEEF

// Before buffer
unsigned int canary_start = CANARY_VALUE;

// After buffer
unsigned int canary_end = CANARY_VALUE;

// Integrity check
if (canary_start != CANARY_VALUE) {
    // Buffer underflow detected
}
if (canary_end != CANARY_VALUE) {
    // Buffer overflow detected
}
```

## 🚀 Advanced Techniques

### 1. Custom Memory Pools
```c
typedef struct {
    char *pool;
    size_t poolSize;
    size_t used;
} MemoryPool;

MemoryPool* createPool(size_t size);
void* poolAlloc(MemoryPool *pool, size_t size);
void destroyPool(MemoryPool *pool);
```

### 2. Memory Alignment Testing
```c
void testAlignment(size_t alignment) {
    void *ptr = aligned_alloc(alignment, 1024);
    assert((uintptr_t)ptr % alignment == 0);
    free(ptr);
}
```

### 3. Double-Free Detection
```c
void trackedFree(void *ptr, const char *file, int line) {
    // Check if already freed
    for (int i = 0; i < freedCount; i++) {
        if (freedPtrs[i] == ptr) {
            printf("Double free detected!\n");
            return;
        }
    }
    
    // Normal free logic...
}
```

### 4. Use-After-Free Detection
```c
void markAsFreed(void *ptr) {
    // Fill with pattern to detect use
    memset(ptr, 0xFE, size);
}
```

## 📊 Performance Analysis

| Test Type | Time Complexity | Space Complexity | Use Case |
|-----------|-----------------|------------------|----------|
| Leak Detection | O(1) alloc, O(n) free | O(n) | Development |
| Buffer Protection | O(1) | O(1) per buffer | Runtime safety |
| Stress Test | O(n) | O(n) | Load testing |
| Benchmark | O(n) | O(1) | Performance tuning |

## 🧪 Testing Strategies

### 1. Unit Testing Memory Functions
```c
void testMemoryTracking() {
    allocationCount = 0;
    
    void *ptr = MALLOC(100);
    assert(allocationCount == 1);
    assert(totalAllocated == 100);
    
    FREE(ptr);
    assert(allocationCount == 0);
    assert(totalAllocated == 0);
}
```

### 2. Integration Testing
```c
void testApplicationMemoryUsage() {
    // Run application code
    runApplication();
    
    // Check for leaks
    printMemoryLeaks();
    assert(allocationCount == 0);
}
```

### 3. Regression Testing
```c
void testMemoryRegression() {
    // Test with known memory patterns
    void *ptrs[100];
    
    // Allocate pattern
    for (int i = 0; i < 100; i++) {
        ptrs[i] = MALLOC(i * 10 + 100);
    }
    
    // Free pattern
    for (int i = 0; i < 100; i += 2) {
        FREE(ptrs[i]);
    }
    
    // Verify expected state
    assert(allocationCount == 50);
}
```

## ⚠️ Common Pitfalls

### 1. Tracking Array Overflow
```c
// Wrong - No bounds checking
allocations[allocationCount++] = record;

// Right - Check bounds
if (allocationCount < MAX_ALLOCATIONS) {
    allocations[allocationCount++] = record;
} else {
    printf("Tracking array full!\n");
}
```

### 2. Thread Safety
```c
// Current implementation is not thread-safe
// For multi-threaded applications, add mutexes:

static pthread_mutex_t allocMutex = PTHREAD_MUTEX_INITIALIZER;

void* trackedMalloc(size_t size, const char *file, int line) {
    pthread_mutex_lock(&allocMutex);
    // Allocation logic...
    pthread_mutex_unlock(&allocMutex);
    return ptr;
}
```

### 3. Performance Overhead
```c
// Tracking adds overhead
// Use conditional compilation for release builds:

#ifdef DEBUG
#define MALLOC(size) trackedMalloc(size, __FILE__, __LINE__)
#define FREE(ptr) trackedFree(ptr, __FILE__, __LINE__)
#else
#define MALLOC(size) malloc(size)
#define FREE(ptr) free(ptr)
#endif
```

### 4. False Positives
```c
// Some libraries might allocate memory internally
// Exclude known allocations from tracking:

void* trackedMalloc(size_t size, const char *file, int line) {
    // Skip tracking for certain files
    if (strstr(file, "third_party.c")) {
        return malloc(size);
    }
    // Normal tracking...
}
```

## 🔧 Real-World Applications

### 1. Development Testing
```c
void runDevelopmentTests() {
    // Enable memory tracking
    allocationCount = 0;
    
    // Run application
    runApplication();
    
    // Check for leaks
    printMemoryLeaks();
    
    // Exit with error if leaks found
    if (allocationCount > 0) {
        exit(EXIT_FAILURE);
    }
}
```

### 2. Production Monitoring
```c
void monitorMemoryUsage() {
    static time_t lastCheck = 0;
    time_t now = time(NULL);
    
    if (now - lastCheck > 60) { // Check every minute
        printMemoryStats();
        lastCheck = now;
    }
}
```

### 3. Performance Profiling
```c
void profileMemoryOperations() {
    printf("=== MEMORY PROFILE ===\n");
    
    benchmarkMallocFree(10000, 64);
    benchmarkMemoryAccess(1024*1024, 10);
    
    analyzeMemoryUsage();
}
```

### 4. Automated Testing
```c
int automatedMemoryTest() {
    // Reset tracking
    allocationCount = 0;
    
    // Run test suite
    runTestSuite();
    
    // Check results
    int leaks = allocationCount;
    printMemoryLeaks();
    
    return leaks == 0 ? 0 : 1;
}
```

## 🎓 Best Practices

### 1. Enable in Debug Only
```c
#ifdef DEBUG
    // Enable memory tracking
#else
    // Use standard malloc/free
#endif
```

### 2. Set Reasonable Limits
```c
#define MAX_ALLOCATIONS 10000  // Adjust based on application
#define MAX_TRACKED_SIZE (1024*1024)  // 1MB max tracked allocation
```

### 3. Clean Reporting
```c
void printCleanMemoryReport() {
    if (allocationCount == 0) {
        printf("✓ No memory leaks detected!\n");
    } else {
        printf("❌ Found %d memory leaks\n", allocationCount);
    }
}
```

### 4. Integration with Build System
```c
// In Makefile
debug: CFLAGS += -DDEBUG -DMEMORY_TRACKING

// In code
#ifdef MEMORY_TRACKING
    #define MALLOC(size) trackedMalloc(size, __FILE__, __LINE__)
#endif
```

### 5. Regular Testing
```c
// Add to test suite
void testMemoryManagement() {
    testLeakDetection();
    testBufferOverflow();
    testStressConditions();
    testPerformance();
}
```

These memory testing tools provide comprehensive coverage for memory-related issues in C programs, helping developers catch bugs early and optimize memory usage patterns.
