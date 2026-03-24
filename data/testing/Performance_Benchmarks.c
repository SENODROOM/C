#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>

// =============================================================================
// PERFORMANCE BENCHMARKING FRAMEWORK
// =============================================================================

// Benchmark result structure
typedef struct {
    char name[100];
    double timeSeconds;
    long long operationsPerSecond;
    double memoryUsageMB;
    int iterations;
} BenchmarkResult;

// High-resolution timer
typedef struct {
    clock_t start;
    clock_t end;
} Timer;

// =============================================================================
// TIMER UTILITIES
// =============================================================================

// Start timer
void startTimer(Timer *timer) {
    timer->start = clock();
}

// Stop timer and get elapsed time
double stopTimer(Timer *timer) {
    timer->end = clock();
    return (double)(timer->end - timer->start) / CLOCKS_PER_SEC;
}

// Get current memory usage (platform-specific)
double getCurrentMemoryUsage() {
    // Simplified memory usage estimation
    // In real implementation, use platform-specific APIs
    return 0.0; // Placeholder
}

// =============================================================================
// ALGORITHM BENCHMARKS
// =============================================================================

// Benchmark sorting algorithms
void benchmarkSorting() {
    printf("=== SORTING ALGORITHM BENCHMARKS ===\n");
    
    const int sizes[] = {1000, 5000, 10000, 50000};
    const int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int i = 0; i < numSizes; i++) {
        int size = sizes[i];
        int *array = (int*)malloc(size * sizeof(int));
        int *copy = (int*)malloc(size * sizeof(int));
        
        // Generate random data
        srand(42); // Fixed seed for reproducibility
        for (int j = 0; j < size; j++) {
            array[j] = rand() % 10000;
            copy[j] = array[j];
        }
        
        printf("\nArray size: %d elements\n", size);
        
        // Bubble Sort
        memcpy(array, copy, size * sizeof(int));
        Timer timer;
        startTimer(&timer);
        
        for (int a = 0; a < size - 1; a++) {
            for (int b = 0; b < size - a - 1; b++) {
                if (array[b] > array[b + 1]) {
                    int temp = array[b];
                    array[b] = array[b + 1];
                    array[b + 1] = temp;
                }
            }
        }
        
        double bubbleTime = stopTimer(&timer);
        printf("Bubble Sort: %.6f seconds (%.0f ops/sec)\n", 
               bubbleTime, (size * size) / bubbleTime);
        
        // Quick Sort (simplified)
        memcpy(array, copy, size * sizeof(int));
        startTimer(&timer);
        
        // Simple quick sort implementation
        void quickSort(int arr[], int left, int right) {
            if (left < right) {
                int pivot = arr[right];
                int i = left - 1;
                
                for (int j = left; j < right; j++) {
                    if (arr[j] <= pivot) {
                        i++;
                        int temp = arr[i];
                        arr[i] = arr[j];
                        arr[j] = temp;
                    }
                }
                
                int temp = arr[i + 1];
                arr[i + 1] = arr[right];
                arr[right] = temp;
                
                quickSort(arr, left, i);
                quickSort(arr, i + 2, right);
            }
        }
        
        quickSort(array, 0, size - 1);
        double quickTime = stopTimer(&timer);
        printf("Quick Sort: %.6f seconds (%.0f ops/sec)\n", 
               quickTime, (size * log2(size)) / quickTime);
        
        printf("Speedup: %.2fx\n", bubbleTime / quickTime);
        
        free(array);
        free(copy);
    }
}

// Benchmark search algorithms
void benchmarkSearching() {
    printf("\n=== SEARCH ALGORITHM BENCHMARKS ===\n");
    
    const int arraySize = 100000;
    int *array = (int*)malloc(arraySize * sizeof(int));
    
    // Create sorted array
    for (int i = 0; i < arraySize; i++) {
        array[i] = i * 2;
    }
    
    const int numSearches = 10000;
    int searchKeys[numSearches];
    
    // Generate random search keys
    srand(42);
    for (int i = 0; i < numSearches; i++) {
        searchKeys[i] = rand() % (arraySize * 2);
    }
    
    // Linear Search
    Timer timer;
    startTimer(&timer);
    
    int linearFound = 0;
    for (int i = 0; i < numSearches; i++) {
        int key = searchKeys[i];
        for (int j = 0; j < arraySize; j++) {
            if (array[j] == key) {
                linearFound++;
                break;
            }
        }
    }
    
    double linearTime = stopTimer(&timer);
    printf("Linear Search: %.6f seconds (%d found)\n", linearTime, linearFound);
    
    // Binary Search
    startTimer(&timer);
    
    int binaryFound = 0;
    for (int i = 0; i < numSearches; i++) {
        int key = searchKeys[i];
        int left = 0, right = arraySize - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (array[mid] == key) {
                binaryFound++;
                break;
            } else if (array[mid] < key) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    
    double binaryTime = stopTimer(&timer);
    printf("Binary Search: %.6f seconds (%d found)\n", binaryTime, binaryFound);
    
    printf("Speedup: %.2fx\n", linearTime / binaryTime);
    
    free(array);
}

// =============================================================================
// DATA STRUCTURE BENCHMARKS
// =============================================================================

// Simple linked list node
typedef struct ListNode {
    int data;
    struct ListNode *next;
} ListNode;

// Benchmark linked list vs array
void benchmarkDataStructures() {
    printf("\n=== DATA STRUCTURE BENCHMARKS ===\n");
    
    const int numElements = 100000;
    const int numOperations = 10000;
    
    // Array operations
    int *array = (int*)malloc(numElements * sizeof(int));
    for (int i = 0; i < numElements; i++) {
        array[i] = i;
    }
    
    Timer timer;
    startTimer(&timer);
    
    // Random array access
    srand(42);
    for (int i = 0; i < numOperations; i++) {
        int index = rand() % numElements;
        volatile int value = array[index]; // Prevent optimization
        (void)value; // Suppress unused variable warning
    }
    
    double arrayTime = stopTimer(&timer);
    printf("Array random access: %.6f seconds\n", arrayTime);
    
    // Linked list operations
    ListNode *head = NULL;
    ListNode *current = NULL;
    
    // Build linked list
    for (int i = numElements - 1; i >= 0; i--) {
        ListNode *node = (ListNode*)malloc(sizeof(ListNode));
        node->data = i;
        node->next = head;
        head = node;
    }
    
    startTimer(&timer);
    
    // Random linked list access (traverse to position)
    srand(42);
    for (int i = 0; i < numOperations; i++) {
        int index = rand() % numElements;
        
        current = head;
        for (int j = 0; j < index && current; j++) {
            current = current->next;
        }
        
        if (current) {
            volatile int value = current->data;
            (void)value;
        }
    }
    
    double listTime = stopTimer(&timer);
    printf("Linked list random access: %.6f seconds\n", listTime);
    
    printf("Array/Linked List Speedup: %.2fx\n", listTime / arrayTime);
    
    // Cleanup linked list
    while (head) {
        ListNode *temp = head;
        head = head->next;
        free(temp);
    }
    
    free(array);
}

// =============================================================================
// STRING OPERATION BENCHMARKS
// =============================================================================

void benchmarkStringOperations() {
    printf("\n=== STRING OPERATION BENCHMARKS ===\n");
    
    const int stringLength = 1000;
    const int numOperations = 10000;
    
    char *str1 = (char*)malloc(stringLength + 1);
    char *str2 = (char*)malloc(stringLength + 1);
    
    // Generate random strings
    srand(42);
    for (int i = 0; i < stringLength; i++) {
        str1[i] = 'A' + (rand() % 26);
        str2[i] = 'a' + (rand() % 26);
    }
    str1[stringLength] = '\0';
    str2[stringLength] = '\0';
    
    // String length comparison
    Timer timer;
    startTimer(&timer);
    
    volatile int totalLength = 0;
    for (int i = 0; i < numOperations; i++) {
        totalLength += strlen(str1);
        totalLength += strlen(str2);
    }
    
    double lengthTime = stopTimer(&timer);
    printf("String length: %.6f seconds\n", lengthTime);
    
    // String copy
    char *buffer = (char*)malloc(stringLength + 1);
    startTimer(&timer);
    
    for (int i = 0; i < numOperations; i++) {
        strcpy(buffer, str1);
        strcpy(buffer, str2);
    }
    
    double copyTime = stopTimer(&timer);
    printf("String copy: %.6f seconds\n", copyTime);
    
    // String comparison
    startTimer(&timer);
    
    int comparisons = 0;
    for (int i = 0; i < numOperations; i++) {
        comparisons += (strcmp(str1, str2) == 0);
    }
    
    double compareTime = stopTimer(&timer);
    printf("String comparison: %.6f seconds (%d matches)\n", compareTime, comparisons);
    
    free(str1);
    free(str2);
    free(buffer);
}

// =============================================================================
// MEMORY BENCHMARKS
// =============================================================================

void benchmarkMemoryOperations() {
    printf("\n=== MEMORY OPERATION BENCHMARKS ===\n");
    
    const int numAllocations = 10000;
    const int allocationSize = 1024;
    
    void **ptrs = (void**)malloc(numAllocations * sizeof(void*));
    
    // Allocation benchmark
    Timer timer;
    startTimer(&timer);
    
    for (int i = 0; i < numAllocations; i++) {
        ptrs[i] = malloc(allocationSize);
        if (ptrs[i]) {
            memset(ptrs[i], 0xAA, allocationSize);
        }
    }
    
    double allocTime = stopTimer(&timer);
    printf("Memory allocation: %.6f seconds (%.0f allocs/sec)\n", 
           allocTime, numAllocations / allocTime);
    
    // Memory access benchmark
    startTimer(&timer);
    
    volatile int sum = 0;
    for (int i = 0; i < numAllocations; i++) {
        if (ptrs[i]) {
            char *ptr = (char*)ptrs[i];
            for (int j = 0; j < allocationSize; j += 64) { // Cache line size
                sum += ptr[j];
            }
        }
    }
    
    double accessTime = stopTimer(&timer);
    printf("Memory access: %.6f seconds\n", accessTime);
    
    // Deallocation benchmark
    startTimer(&timer);
    
    for (int i = 0; i < numAllocations; i++) {
        if (ptrs[i]) {
            free(ptrs[i]);
        }
    }
    
    double freeTime = stopTimer(&timer);
    printf("Memory deallocation: %.6f seconds (%.0f frees/sec)\n", 
           freeTime, numAllocations / freeTime);
    
    free(ptrs);
}

// =============================================================================
// MATHEMATICAL BENCHMARKS
// =============================================================================

void benchmarkMathOperations() {
    printf("\n=== MATHEMATICAL OPERATION BENCHMARKS ===\n");
    
    const int numOperations = 10000000;
    
    // Integer arithmetic
    Timer timer;
    startTimer(&timer);
    
    volatile int intResult = 0;
    for (int i = 0; i < numOperations; i++) {
        intResult += i * (i + 1) / 2;
    }
    
    double intTime = stopTimer(&timer);
    printf("Integer arithmetic: %.6f seconds (%.0f ops/sec)\n", 
           intTime, numOperations / intTime);
    
    // Floating point arithmetic
    startTimer(&timer);
    
    volatile double floatResult = 0.0;
    for (int i = 0; i < numOperations; i++) {
        floatResult += sin(i) * cos(i);
    }
    
    double floatTime = stopTimer(&timer);
    printf("Floating point: %.6f seconds (%.0f ops/sec)\n", 
           floatTime, numOperations / floatTime);
    
    // Square root operations
    startTimer(&timer);
    
    volatile double sqrtResult = 0.0;
    for (int i = 0; i < numOperations; i++) {
        sqrtResult += sqrt(i + 1);
    }
    
    double sqrtTime = stopTimer(&timer);
    printf("Square root: %.6f seconds (%.0f ops/sec)\n", 
           sqrtTime, numOperations / sqrtTime);
}

// =============================================================================
// RECURSION VS ITERATION BENCHMARKS
// =============================================================================

// Recursive factorial
long long factorialRecursive(int n) {
    if (n <= 1) return 1;
    return n * factorialRecursive(n - 1);
}

// Iterative factorial
long long factorialIterative(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

void benchmarkRecursionVsIteration() {
    printf("\n=== RECURSION VS ITERATION BENCHMARKS ===\n");
    
    const int numTests = 1000;
    const int factorialValue = 20;
    
    // Recursive factorial
    Timer timer;
    startTimer(&timer);
    
    volatile long long recursiveResult = 0;
    for (int i = 0; i < numTests; i++) {
        recursiveResult += factorialRecursive(factorialValue);
    }
    
    double recursiveTime = stopTimer(&timer);
    printf("Recursive factorial: %.6f seconds\n", recursiveTime);
    
    // Iterative factorial
    startTimer(&timer);
    
    volatile long long iterativeResult = 0;
    for (int i = 0; i < numTests; i++) {
        iterativeResult += factorialIterative(factorialValue);
    }
    
    double iterativeTime = stopTimer(&timer);
    printf("Iterative factorial: %.6f seconds\n", iterativeTime);
    
    printf("Iteration/Recursion speedup: %.2fx\n", recursiveTime / iterativeTime);
}

// =============================================================================
// CACHE PERFORMANCE BENCHMARKS
// =============================================================================

void benchmarkCachePerformance() {
    printf("\n=== CACHE PERFORMANCE BENCHMARKS ===\n");
    
    const int arraySize = 1024 * 1024; // 1M elements
    const int iterations = 100;
    
    int *array = (int*)malloc(arraySize * sizeof(int));
    
    // Initialize array
    for (int i = 0; i < arraySize; i++) {
        array[i] = i;
    }
    
    // Sequential access (cache-friendly)
    Timer timer;
    startTimer(&timer);
    
    volatile long long sum = 0;
    for (int iter = 0; iter < iterations; iter++) {
        for (int i = 0; i < arraySize; i++) {
            sum += array[i];
        }
    }
    
    double sequentialTime = stopTimer(&timer);
    printf("Sequential access: %.6f seconds\n", sequentialTime);
    
    // Random access (cache-unfriendly)
    startTimer(&timer);
    
    sum = 0;
    srand(42);
    for (int iter = 0; iter < iterations; iter++) {
        for (int i = 0; i < arraySize; i++) {
            int index = rand() % arraySize;
            sum += array[index];
        }
    }
    
    double randomTime = stopTimer(&timer);
    printf("Random access: %.6f seconds\n", randomTime);
    
    printf("Cache performance ratio: %.2fx\n", randomTime / sequentialTime);
    
    free(array);
}

// =============================================================================
// COMPREHENSIVE BENCHMARK REPORT
// =============================================================================

void generateBenchmarkReport() {
    printf("\n" "=" * 50 "\n");
    printf("COMPREHENSIVE BENCHMARK REPORT\n");
    printf("=" * 50 "\n");
    
    // Run a subset of benchmarks for report
    const char* testNames[] = {
        "Integer Operations",
        "Memory Allocation",
        "String Operations",
        "Cache Performance"
    };
    
    double times[4];
    
    // Integer operations
    Timer timer;
    startTimer(&timer);
    volatile int result = 0;
    for (int i = 0; i < 1000000; i++) {
        result += i * (i + 1);
    }
    times[0] = stopTimer(&timer);
    
    // Memory allocation
    void *ptrs[1000];
    startTimer(&timer);
    for (int i = 0; i < 1000; i++) {
        ptrs[i] = malloc(1024);
    }
    for (int i = 0; i < 1000; i++) {
        free(ptrs[i]);
    }
    times[1] = stopTimer(&timer);
    
    // String operations
    char str[100];
    strcpy(str, "Hello, World!");
    startTimer(&timer);
    for (int i = 0; i < 100000; i++) {
        volatile int len = strlen(str);
        (void)len;
    }
    times[2] = stopTimer(&timer);
    
    // Cache performance
    int cacheArray[1024];
    for (int i = 0; i < 1024; i++) cacheArray[i] = i;
    startTimer(&timer);
    volatile int sum = 0;
    for (int i = 0; i < 100000; i++) {
        sum += cacheArray[i % 1024];
    }
    times[3] = stopTimer(&timer);
    
    // Print report
    printf("\nTest Performance Summary:\n");
    printf("%-20s %12s %12s\n", "Test Name", "Time (sec)", "Ops/sec");
    printf("-" * 45 "\n");
    
    for (int i = 0; i < 4; i++) {
        printf("%-20s %12.6f %12.0f\n", 
               testNames[i], times[i], 1000000.0 / times[i]);
    }
    
    printf("\nSystem Information:\n");
    printf("Clocks per second: %ld\n", CLOCKS_PER_SEC);
    printf("Time resolution: %.6f seconds\n", 1.0 / CLOCKS_PER_SEC);
}

// =============================================================================
// MAIN BENCHMARK RUNNER
// =============================================================================

int main() {
    printf("Performance Benchmarks\n");
    printf("=====================\n");
    printf("System clock resolution: %.6f seconds\n\n", 1.0 / CLOCKS_PER_SEC);
    
    // Run all benchmarks
    benchmarkSorting();
    benchmarkSearching();
    benchmarkDataStructures();
    benchmarkStringOperations();
    benchmarkMemoryOperations();
    benchmarkMathOperations();
    benchmarkRecursionVsIteration();
    benchmarkCachePerformance();
    
    // Generate comprehensive report
    generateBenchmarkReport();
    
    printf("\nAll benchmarks completed!\n");
    return 0;
}
