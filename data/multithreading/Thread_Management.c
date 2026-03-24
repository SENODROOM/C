#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>

// =============================================================================
// BASIC THREAD OPERATIONS
// =============================================================================

#define NUM_THREADS 5
#define ARRAY_SIZE 100

// Shared data structure
typedef struct {
    int array[ARRAY_SIZE];
    int sum;
    pthread_mutex_t mutex;
} SharedData;

SharedData shared_data;

// Thread function that calculates sum of array portion
void* calculateSum(void* arg) {
    int thread_id = *(int*)arg;
    int start = thread_id * (ARRAY_SIZE / NUM_THREADS);
    int end = start + (ARRAY_SIZE / NUM_THREADS);
    
    int local_sum = 0;
    for (int i = start; i < end; i++) {
        local_sum += shared_data.array[i];
    }
    
    // Lock mutex to update shared sum
    pthread_mutex_lock(&shared_data.mutex);
    shared_data.sum += local_sum;
    printf("Thread %d: local_sum = %d, total_sum = %d\n", 
           thread_id, local_sum, shared_data.sum);
    pthread_mutex_unlock(&shared_data.mutex);
    
    return NULL;
}

// =============================================================================
// PRODUCER-CONSUMER PROBLEM
// =============================================================================

#define BUFFER_SIZE 10

typedef struct {
    int buffer[BUFFER_SIZE];
    int in, out, count;
    pthread_mutex_t mutex;
    sem_t empty, full;
} CircularBuffer;

CircularBuffer cb;

void* producer(void* arg) {
    int producer_id = *(int*)arg;
    
    for (int i = 0; i < 20; i++) {
        int item = producer_id * 100 + i;
        
        sem_wait(&cb.empty); // Wait for empty slot
        pthread_mutex_lock(&cb.mutex);
        
        // Add item to buffer
        cb.buffer[cb.in] = item;
        printf("Producer %d: Produced %d at position %d\n", 
               producer_id, item, cb.in);
        cb.in = (cb.in + 1) % BUFFER_SIZE;
        cb.count++;
        
        pthread_mutex_unlock(&cb.mutex);
        sem_post(&cb.full); // Signal that buffer has item
        
        usleep(100000); // Simulate production time
    }
    
    return NULL;
}

void* consumer(void* arg) {
    int consumer_id = *(int*)arg;
    
    for (int i = 0; i < 20; i++) {
        sem_wait(&cb.full); // Wait for item
        pthread_mutex_lock(&cb.mutex);
        
        // Remove item from buffer
        int item = cb.buffer[cb.out];
        printf("Consumer %d: Consumed %d from position %d\n", 
               consumer_id, item, cb.out);
        cb.out = (cb.out + 1) % BUFFER_SIZE;
        cb.count--;
        
        pthread_mutex_unlock(&cb.mutex);
        sem_post(&cb.empty); // Signal empty slot
        
        usleep(150000); // Simulate consumption time
    }
    
    return NULL;
}

// =============================================================================
// READER-WRITER PROBLEM
// =============================================================================

typedef struct {
    char data[256];
    int readers;
    pthread_mutex_t mutex, write_mutex;
    pthread_cond_t can_read, can_write;
} SharedResource;

SharedResource shared_resource;

void* reader(void* arg) {
    int reader_id = *(int*)arg;
    
    for (int i = 0; i < 5; i++) {
        // Acquire read lock
        pthread_mutex_lock(&shared_resource.mutex);
        shared_resource.readers++;
        if (shared_resource.readers == 1) {
            pthread_mutex_lock(&shared_resource.write_mutex);
        }
        pthread_mutex_unlock(&shared_resource.mutex);
        
        // Read data
        printf("Reader %d: Reading data: '%s'\n", reader_id, shared_resource.data);
        usleep(200000); // Simulate reading time
        
        // Release read lock
        pthread_mutex_lock(&shared_resource.mutex);
        shared_resource.readers--;
        if (shared_resource.readers == 0) {
            pthread_mutex_unlock(&shared_resource.write_mutex);
        }
        pthread_mutex_unlock(&shared_resource.mutex);
        
        usleep(100000); // Wait between reads
    }
    
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *(int*)arg;
    
    for (int i = 0; i < 3; i++) {
        // Acquire write lock
        pthread_mutex_lock(&shared_resource.write_mutex);
        
        // Write data
        snprintf(shared_resource.data, sizeof(shared_resource.data), 
                 "Data written by Writer %d - Iteration %d", writer_id, i + 1);
        printf("Writer %d: Wrote data: '%s'\n", writer_id, shared_resource.data);
        usleep(300000); // Simulate writing time
        
        // Release write lock
        pthread_mutex_unlock(&shared_resource.write_mutex);
        
        usleep(200000); // Wait between writes
    }
    
    return NULL;
}

// =============================================================================
// THREAD SYNCHRONIZATION
// =============================================================================

pthread_barrier_t barrier;

void* barrierThread(void* arg) {
    int thread_id = *(int*)arg;
    
    printf("Thread %d: Starting work\n", thread_id);
    usleep(100000 * thread_id); // Different work times
    
    printf("Thread %d: Reached barrier\n", thread_id);
    pthread_barrier_wait(&barrier); // Wait for all threads
    
    printf("Thread %d: Passed barrier, continuing\n", thread_id);
    
    return NULL;
}

// =============================================================================
// THREAD POOL IMPLEMENTATION
// =============================================================================

#define THREAD_POOL_SIZE 4
#define TASK_QUEUE_SIZE 20

typedef struct Task {
    void (*function)(void*);
    void* arg;
} Task;

typedef struct {
    pthread_t threads[THREAD_POOL_SIZE];
    Task task_queue[TASK_QUEUE_SIZE];
    int queue_size, queue_front, queue_rear;
    int shutdown;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_not_empty, queue_not_full;
} ThreadPool;

ThreadPool thread_pool;

void* workerThread(void* arg) {
    while (1) {
        pthread_mutex_lock(&thread_pool.queue_mutex);
        
        // Wait for tasks
        while (thread_pool.queue_size == 0 && !thread_pool.shutdown) {
            pthread_cond_wait(&thread_pool.queue_not_empty, &thread_pool.queue_mutex);
        }
        
        if (thread_pool.shutdown) {
            pthread_mutex_unlock(&thread_pool.queue_mutex);
            break;
        }
        
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

void initThreadPool() {
    thread_pool.queue_size = 0;
    thread_pool.queue_front = 0;
    thread_pool.queue_rear = 0;
    thread_pool.shutdown = 0;
    
    pthread_mutex_init(&thread_pool.queue_mutex, NULL);
    pthread_cond_init(&thread_pool.queue_not_empty, NULL);
    pthread_cond_init(&thread_pool.queue_not_full, NULL);
    
    // Create worker threads
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_create(&thread_pool.threads[i], NULL, workerThread, NULL);
    }
}

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

void destroyThreadPool() {
    pthread_mutex_lock(&thread_pool.queue_mutex);
    thread_pool.shutdown = 1;
    pthread_cond_broadcast(&thread_pool.queue_not_empty);
    pthread_mutex_unlock(&thread_pool.queue_mutex);
    
    // Wait for all threads to finish
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        pthread_join(thread_pool.threads[i], NULL);
    }
    
    pthread_mutex_destroy(&thread_pool.queue_mutex);
    pthread_cond_destroy(&thread_pool.queue_not_empty);
    pthread_cond_destroy(&thread_pool.queue_not_full);
}

// Sample task functions
void sampleTask1(void* arg) {
    int task_id = *(int*)arg;
    printf("Executing Task 1 with ID %d\n", task_id);
    usleep(100000);
}

void sampleTask2(void* arg) {
    int task_id = *(int*)arg;
    printf("Executing Task 2 with ID %d\n", task_id);
    usleep(150000);
}

// =============================================================================
// ATOMIC OPERATIONS SIMULATION
// =============================================================================

typedef struct {
    int value;
    pthread_mutex_t mutex;
} AtomicInteger;

void initAtomic(AtomicInteger* atomic, int initial_value) {
    atomic->value = initial_value;
    pthread_mutex_init(&atomic->mutex, NULL);
}

int atomicGet(AtomicInteger* atomic) {
    pthread_mutex_lock(&atomic->mutex);
    int value = atomic->value;
    pthread_mutex_unlock(&atomic->mutex);
    return value;
}

void atomicSet(AtomicInteger* atomic, int value) {
    pthread_mutex_lock(&atomic->mutex);
    atomic->value = value;
    pthread_mutex_unlock(&atomic->mutex);
}

int atomicIncrement(AtomicInteger* atomic) {
    pthread_mutex_lock(&atomic->mutex);
    int old_value = atomic->value++;
    pthread_mutex_unlock(&atomic->mutex);
    return old_value;
}

AtomicInteger counter;

void* atomicCounterThread(void* arg) {
    int thread_id = *(int*)arg;
    
    for (int i = 0; i < 1000; i++) {
        int old_value = atomicIncrement(&counter);
        printf("Thread %d: Incremented counter from %d to %d\n", 
               thread_id, old_value, old_value + 1);
    }
    
    return NULL;
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateBasicThreading() {
    printf("=== BASIC THREADING ===\n");
    
    // Initialize shared data
    for (int i = 0; i < ARRAY_SIZE; i++) {
        shared_data.array[i] = i + 1;
    }
    shared_data.sum = 0;
    pthread_mutex_init(&shared_data.mutex, NULL);
    
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, calculateSum, &thread_ids[i]);
    }
    
    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final sum: %d (Expected: %d)\n", shared_data.sum, ARRAY_SIZE * (ARRAY_SIZE + 1) / 2);
    
    pthread_mutex_destroy(&shared_data.mutex);
    printf("\n");
}

void demonstrateProducerConsumer() {
    printf("=== PRODUCER-CONSUMER ===\n");
    
    // Initialize circular buffer
    cb.in = cb.out = cb.count = 0;
    pthread_mutex_init(&cb.mutex, NULL);
    sem_init(&cb.empty, 0, BUFFER_SIZE);
    sem_init(&cb.full, 0, 0);
    
    pthread_t producer_threads[2], consumer_threads[2];
    int producer_ids[2] = {1, 2};
    int consumer_ids[2] = {1, 2};
    
    // Create producer and consumer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_ids[i]);
    }
    
    // Wait for completion
    for (int i = 0; i < 2; i++) {
        pthread_join(producer_threads[i], NULL);
        pthread_join(consumer_threads[i], NULL);
    }
    
    // Cleanup
    pthread_mutex_destroy(&cb.mutex);
    sem_destroy(&cb.empty);
    sem_destroy(&cb.full);
    printf("\n");
}

void demonstrateReaderWriter() {
    printf("=== READER-WRITER ===\n");
    
    // Initialize shared resource
    strcpy(shared_resource.data, "Initial data");
    shared_resource.readers = 0;
    pthread_mutex_init(&shared_resource.mutex, NULL);
    pthread_mutex_init(&shared_resource.write_mutex, NULL);
    pthread_cond_init(&shared_resource.can_read, NULL);
    pthread_cond_init(&shared_resource.can_write, NULL);
    
    pthread_t reader_threads[3], writer_threads[2];
    int reader_ids[3] = {1, 2, 3};
    int writer_ids[2] = {1, 2};
    
    // Create reader and writer threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&reader_threads[i], NULL, reader, &reader_ids[i]);
    }
    for (int i = 0; i < 2; i++) {
        pthread_create(&writer_threads[i], NULL, writer, &writer_ids[i]);
    }
    
    // Wait for completion
    for (int i = 0; i < 3; i++) {
        pthread_join(reader_threads[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writer_threads[i], NULL);
    }
    
    // Cleanup
    pthread_mutex_destroy(&shared_resource.mutex);
    pthread_mutex_destroy(&shared_resource.write_mutex);
    pthread_cond_destroy(&shared_resource.can_read);
    pthread_cond_destroy(&shared_resource.can_write);
    printf("\n");
}

void demonstrateBarrierSynchronization() {
    printf("=== BARRIER SYNCHRONIZATION ===\n");
    
    pthread_barrier_init(&barrier, NULL, 3);
    
    pthread_t threads[3];
    int thread_ids[3] = {1, 2, 3};
    
    // Create threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, barrierThread, &thread_ids[i]);
    }
    
    // Wait for completion
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&barrier);
    printf("\n");
}

void demonstrateThreadPool() {
    printf("=== THREAD POOL ===\n");
    
    initThreadPool();
    
    // Submit tasks
    for (int i = 0; i < 10; i++) {
        int* task_id = malloc(sizeof(int));
        *task_id = i;
        
        if (i % 2 == 0) {
            submitTask(sampleTask1, task_id);
        } else {
            submitTask(sampleTask2, task_id);
        }
    }
    
    // Wait a bit for tasks to complete
    sleep(2);
    
    destroyThreadPool();
    printf("\n");
}

void demonstrateAtomicOperations() {
    printf("=== ATOMIC OPERATIONS ===\n");
    
    initAtomic(&counter, 0);
    
    pthread_t threads[5];
    int thread_ids[5] = {1, 2, 3, 4, 5};
    
    // Create threads
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, atomicCounterThread, &thread_ids[i]);
    }
    
    // Wait for completion
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final counter value: %d\n", atomicGet(&counter));
    
    pthread_mutex_destroy(&counter.mutex);
    printf("\n");
}

int main() {
    printf("Thread Management and Concurrency\n");
    printf("=================================\n\n");
    
    demonstrateBasicThreading();
    demonstrateProducerConsumer();
    demonstrateReaderWriter();
    demonstrateBarrierSynchronization();
    demonstrateThreadPool();
    demonstrateAtomicOperations();
    
    printf("All threading examples demonstrated!\n");
    return 0;
}
