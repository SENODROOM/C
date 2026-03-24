# Thread Management and Concurrency

This file contains comprehensive multithreading examples in C, including basic thread operations, producer-consumer problem, reader-writer problem, thread pools, and synchronization primitives.

## 📚 Threading Overview

### 🧵 Thread Types
- **User Threads**: Managed by user-level libraries
- **Kernel Threads**: Managed by operating system
- **Hybrid Threads**: Combination of both

### 🔒 Synchronization Primitives
- **Mutex**: Mutual exclusion lock
- **Semaphore**: Counting semaphore
- **Condition Variable**: Thread synchronization
- **Barrier**: Thread synchronization point

## 🧵 Basic Thread Operations

### Thread Creation and Management
```c
#include <pthread.h>

void* threadFunction(void* arg) {
    int thread_id = *(int*)arg;
    printf("Thread %d is running\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, threadFunction, &thread_ids[i]);
    }
    
    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
```

### Thread Attributes
```c
pthread_attr_t attr;
pthread_attr_init(&attr);

// Set stack size
pthread_attr_setstacksize(&attr, 1024 * 1024);

// Set detached state
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

// Create thread with attributes
pthread_create(&thread, &attr, threadFunction, arg);

pthread_attr_destroy(&attr);
```

## 🔒 Mutex Operations

### Basic Mutex Usage
```c
pthread_mutex_t mutex;

// Initialize mutex
pthread_mutex_init(&mutex, NULL);

// Lock mutex
pthread_mutex_lock(&mutex);

// Critical section
shared_resource++;

// Unlock mutex
pthread_mutex_unlock(&mutex);

// Destroy mutex
pthread_mutex_destroy(&mutex);
```

### Mutex Attributes
```c
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);

// Set mutex type
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

// Initialize with attributes
pthread_mutex_init(&mutex, &attr);

pthread_mutexattr_destroy(&attr);
```

## 📦 Producer-Consumer Problem

### Circular Buffer Implementation
```c
typedef struct {
    int buffer[BUFFER_SIZE];
    int in, out, count;
    pthread_mutex_t mutex;
    sem_t empty, full;
} CircularBuffer;

void* producer(void* arg) {
    while (1) {
        int item = produce_item();
        
        sem_wait(&cb.empty); // Wait for empty slot
        pthread_mutex_lock(&cb.mutex);
        
        // Add item to buffer
        cb.buffer[cb.in] = item;
        cb.in = (cb.in + 1) % BUFFER_SIZE;
        cb.count++;
        
        pthread_mutex_unlock(&cb.mutex);
        sem_post(&cb.full); // Signal that buffer has item
    }
}

void* consumer(void* arg) {
    while (1) {
        sem_wait(&cb.full); // Wait for item
        pthread_mutex_lock(&cb.mutex);
        
        // Remove item from buffer
        int item = cb.buffer[cb.out];
        cb.out = (cb.out + 1) % BUFFER_SIZE;
        cb.count--;
        
        pthread_mutex_unlock(&cb.mutex);
        sem_post(&cb.empty); // Signal empty slot
        
        consume_item(item);
    }
}
```

### Semaphore Operations
```c
// Initialize semaphores
sem_init(&empty, 0, BUFFER_SIZE); // Start with all slots empty
sem_init(&full, 0, 0);          // Start with no items

// Wait operation (P operation)
sem_wait(&semaphore);

// Signal operation (V operation)
sem_post(&semaphore);

// Destroy semaphore
sem_destroy(&semaphore);
```

## 📖 Reader-Writer Problem

### Reader-Writer Implementation
```c
typedef struct {
    char data[256];
    int readers;
    pthread_mutex_t mutex, write_mutex;
} SharedResource;

void* reader(void* arg) {
    // Acquire read lock
    pthread_mutex_lock(&shared_resource.mutex);
    shared_resource.readers++;
    if (shared_resource.readers == 1) {
        pthread_mutex_lock(&shared_resource.write_mutex);
    }
    pthread_mutex_unlock(&shared_resource.mutex);
    
    // Read data
    printf("Reading: %s\n", shared_resource.data);
    
    // Release read lock
    pthread_mutex_lock(&shared_resource.mutex);
    shared_resource.readers--;
    if (shared_resource.readers == 0) {
        pthread_mutex_unlock(&shared_resource.write_mutex);
    }
    pthread_mutex_unlock(&shared_resource.mutex);
}

void* writer(void* arg) {
    // Acquire write lock
    pthread_mutex_lock(&shared_resource.write_mutex);
    
    // Write data
    strcpy(shared_resource.data, "New data");
    printf("Writing: %s\n", shared_resource.data);
    
    // Release write lock
    pthread_mutex_unlock(&shared_resource.write_mutex);
}
```

### Multiple Readers, Single Writer
- **Multiple readers** can access simultaneously
- **Single writer** has exclusive access
- **Fairness**: Prevents writer starvation

## 🚧 Barrier Synchronization

### Barrier Implementation
```c
pthread_barrier_t barrier;

// Initialize barrier
pthread_barrier_init(&barrier, NULL, NUM_THREADS);

void* threadFunction(void* arg) {
    // Phase 1 work
    do_phase1_work();
    
    // Wait at barrier
    pthread_barrier_wait(&barrier);
    
    // Phase 2 work (all threads reach this point)
    do_phase2_work();
    
    return NULL;
}

// Destroy barrier
pthread_barrier_destroy(&barrier);
```

### Barrier Use Cases
- **Parallel algorithms**: Synchronize computation phases
- **Initialization**: Wait for all threads to start
- **Checkpointing**: Ensure consistent state

## 🏊 Thread Pool Implementation

### Thread Pool Structure
```c
typedef struct {
    pthread_t threads[THREAD_POOL_SIZE];
    Task task_queue[TASK_QUEUE_SIZE];
    int queue_size, queue_front, queue_rear;
    int shutdown;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_not_empty, queue_not_full;
} ThreadPool;

void* workerThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&thread_pool.queue_mutex);
        
        // Wait for tasks
        while (thread_pool.queue_size == 0 && !thread_pool.shutdown) {
            pthread_cond_wait(&thread_pool.queue_not_empty, &thread_pool.queue_mutex);
        }
        
        if (thread_pool.shutdown) break;
        
        // Get task from queue
        Task task = thread_pool.task_queue[thread_pool.queue_front];
        thread_pool.queue_front = (thread_pool.queue_front + 1) % TASK_QUEUE_SIZE;
        thread_pool.queue_size--;
        
        pthread_cond_signal(&thread_pool.queue_not_full);
        pthread_mutex_unlock(&thread_pool.queue_mutex);
        
        // Execute task
        task.function(task.arg);
    }
    
    return NULL;
}
```

### Task Submission
```c
void submitTask(void (*function)(void*), void* arg) {
    pthread_mutex_lock(&thread_pool.queue_mutex);
    
    // Wait if queue is full
    while (thread_pool.queue_size == TASK_QUEUE_SIZE) {
        pthread_cond_wait(&thread_pool.queue_not_full, &thread_pool.queue_mutex);
    }
    
    // Add task to queue
    thread_pool.task_queue[thread_pool.queue_rear].function = function;
    thread_pool.task_queue[thread_pool.queue_rear].arg = arg;
    thread_pool.queue_rear = (thread_pool.queue_rear + 1) % TASK_QUEUE_SIZE;
    thread_pool.queue_size++;
    
    pthread_cond_signal(&thread_pool.queue_not_empty);
    pthread_mutex_unlock(&thread_pool.queue_mutex);
}
```

## ⚛️ Atomic Operations

### Simulated Atomic Operations
```c
typedef struct {
    int value;
    pthread_mutex_t mutex;
} AtomicInteger;

int atomicIncrement(AtomicInteger* atomic) {
    pthread_mutex_lock(&atomic->mutex);
    int old_value = atomic->value++;
    pthread_mutex_unlock(&atomic->mutex);
    return old_value;
}

int atomicCompareAndSwap(AtomicInteger* atomic, int expected, int desired) {
    pthread_mutex_lock(&atomic->mutex);
    int success = (atomic->value == expected);
    if (success) {
        atomic->value = desired;
    }
    pthread_mutex_unlock(&atomic->mutex);
    return success;
}
```

### Real Atomic Operations (C11)
```c
#include <stdatomic.h>

atomic_int counter;

void atomicOperations() {
    atomic_store(&counter, 0);
    int value = atomic_load(&counter);
    atomic_fetch_add(&counter, 1);
    atomic_compare_exchange_strong(&counter, &expected, desired);
}
```

## 🔄 Condition Variables

### Condition Variable Usage
```c
pthread_cond_t cond;
pthread_mutex_t mutex;
int shared_data = 0;

void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        
        shared_data = i;
        printf("Produced: %d\n", i);
        
        pthread_cond_signal(&cond); // Signal consumer
        pthread_mutex_unlock(&mutex);
        
        usleep(100000);
    }
}

void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        
        while (shared_data == 0) {
            pthread_cond_wait(&cond, &mutex); // Wait for producer
        }
        
        printf("Consumed: %d\n", shared_data);
        shared_data = 0;
        
        pthread_mutex_unlock(&mutex);
    }
}
```

## 💡 Advanced Concepts

### 1. Thread-Local Storage
```c
__thread int thread_local_var;

void* threadFunction(void* arg) {
    thread_local_var = *(int*)arg;
    printf("Thread %d: thread_local_var = %d\n", 
           thread_local_var, thread_local_var);
    return NULL;
}
```

### 2. Thread Cancellation
```c
void* cancellableThread(void* arg) {
    // Set cancellation state
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    
    while (1) {
        // Do work...
        pthread_testcancel(); // Cancellation point
    }
}

// Cancel thread
pthread_cancel(thread);
```

### 3. Thread Affinity
```c
void setThreadAffinity(pthread_t thread, int cpu_core) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_core, &cpuset);
    
    pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
}
```

### 4. Thread Scheduling
```c
void setThreadPriority(pthread_t thread, int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    
    pthread_setschedparam(thread, SCHED_FIFO, &param);
}
```

## 📊 Performance Considerations

### 1. Lock Granularity
```c
// Fine-grained locking
struct {
    pthread_mutex_t mutex1;
    pthread_mutex_t mutex2;
    int data1, data2;
} fine_grained;

// Coarse-grained locking
struct {
    pthread_mutex_t mutex;
    int data1, data2;
} coarse_grained;
```

### 2. Lock Contention
```c
// High contention
pthread_mutex_t global_mutex;

// Low contention (per-thread locks)
pthread_mutex_t thread_mutex[NUM_THREADS];
```

### 3. Deadlock Prevention
```c
// Always acquire locks in same order
void safeFunction() {
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex2);
    // Critical section
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}
```

## ⚠️ Common Pitfalls

### 1. Race Conditions
```c
// Wrong - Race condition
int counter = 0;
void* incrementThread(void* arg) {
    counter++; // Not atomic!
    return NULL;
}

// Right - Protected with mutex
int counter = 0;
pthread_mutex_t counter_mutex;

void* incrementThread(void* arg) {
    pthread_mutex_lock(&counter_mutex);
    counter++;
    pthread_mutex_unlock(&counter_mutex);
    return NULL;
}
```

### 2. Deadlock
```c
// Wrong - Potential deadlock
void thread1() {
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex2);
    // Critical section
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}

void thread2() {
    pthread_mutex_lock(&mutex2);
    pthread_mutex_lock(&mutex1); // Deadlock!
    // Critical section
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
}

// Right - Consistent lock ordering
void thread2() {
    pthread_mutex_lock(&mutex1); // Same order as thread1
    pthread_mutex_lock(&mutex2);
    // Critical section
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}
```

### 3. Priority Inversion
```c
// Wrong - Low-priority thread holds lock needed by high-priority thread
// Right - Use priority inheritance or priority ceiling protocols
```

### 4. Forgotten Unlock
```c
// Wrong - Forgetting to unlock
pthread_mutex_lock(&mutex);
if (error_condition) {
    return; // Forgot to unlock mutex!
}
pthread_mutex_unlock(&mutex);

// Right - Always unlock
pthread_mutex_lock(&mutex);
if (error_condition) {
    pthread_mutex_unlock(&mutex);
    return;
}
pthread_mutex_unlock(&mutex);
```

## 🔧 Real-World Applications

### 1. Web Server
```c
void* handleRequest(void* arg) {
    int client_socket = *(int*)arg;
    
    // Process HTTP request
    processHTTPRequest(client_socket);
    
    close(client_socket);
    return NULL;
}

void startWebServer() {
    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        
        pthread_t thread;
        pthread_create(&thread, NULL, handleRequest, &client_socket);
        pthread_detach(thread);
    }
}
```

### 2. Parallel Processing
```c
void* parallelArraySum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;
    
    for (int i = data->start; i < data->end; i++) {
        sum += array[i];
    }
    
    data->result = sum;
    return NULL;
}
```

### 3. Producer-Consumer Pipeline
```c
void* pipelineStage1(void* arg) {
    while (1) {
        Item* item = getInput();
        processStage1(item);
        addToQueue2(item);
    }
}

void* pipelineStage2(void* arg) {
    while (1) {
        Item* item = getFromQueue2();
        processStage2(item);
        addToQueue3(item);
    }
}
```

## 🎓 Best Practices

### 1. Resource Management
```c
// Always initialize and cleanup
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL);
// Use mutex...
pthread_mutex_destroy(&mutex);
```

### 2. Error Handling
```c
int result = pthread_mutex_lock(&mutex);
if (result != 0) {
    fprintf(stderr, "Mutex lock failed: %s\n", strerror(result));
    // Handle error
}
```

### 3. Thread Safety
```c
// Design for thread safety from the beginning
// Use appropriate synchronization
// Minimize shared state
// Prefer immutable data when possible
```

### 4. Performance Optimization
```c
// Use lock-free algorithms when possible
// Minimize lock contention
// Use thread-local storage
// Consider thread pools for short-lived tasks
```

### 5. Debugging
```c
// Use thread-safe logging
// Enable thread debugging tools
// Use assertions for invariants
// Test with different thread counts
```

Multithreading in C provides powerful concurrency capabilities but requires careful design to avoid race conditions, deadlocks, and performance issues. Master these concepts to build efficient concurrent applications!
