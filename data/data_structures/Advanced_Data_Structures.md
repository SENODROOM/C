# Advanced Data Structures

This file contains implementations of advanced data structures in C, including Trie (Prefix Tree), Disjoint Set Union (Union-Find), Segment Tree, Fenwick Tree (Binary Indexed Tree), and LRU Cache.

## 📚 Advanced Data Structure Overview

### 🔤 Trie (Prefix Tree)
Efficient string storage and prefix-based operations

### 🔗 Disjoint Set Union
Manage partition of elements into disjoint sets

### 📊 Segment Tree
Range query and point update data structure

### 🌲 Fenwick Tree
Efficient prefix sum and range query structure

### 💾 LRU Cache
Cache with automatic eviction of least recently used items

## 🔤 Trie (Prefix Tree)

### Structure Definition
```c
typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    int frequency;
} TrieNode;
```

### Key Properties
- **Alphabet Size**: 26 for lowercase English letters
- **Time Complexity**: O(L) where L is word length
- **Space Complexity**: O(N × L) for N words
- **Use Case**: Dictionary, autocomplete, prefix matching

### Core Operations

#### Insertion
```c
void insertTrie(TrieNode* root, const char* word) {
    TrieNode* current = root;
    
    for (int level = 0; word[level]; level++) {
        int index = word[level] - 'a';
        if (current->children[index] == NULL) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    
    current->isEndOfWord = true;
    current->frequency++;
}
```

#### Search
```c
bool searchTrie(TrieNode* root, const char* word) {
    TrieNode* current = root;
    
    for (int level = 0; word[level]; level++) {
        int index = word[level] - 'a';
        if (current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }
    
    return current->isEndOfWord;
}
```

#### Prefix Search
```c
bool startsWith(TrieNode* root, const char* prefix) {
    TrieNode* current = root;
    
    for (int level = 0; prefix[level]; level++) {
        int index = prefix[level] - 'a';
        if (current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }
    
    return true;
}
```

### Applications
- **Autocomplete Systems**: Suggest words based on prefix
- **Spell Checkers**: Find similar words
- **IP Routing**: Longest prefix matching
- **Word Frequency Counting**: Track word occurrences

## 🔗 Disjoint Set Union (Union-Find)

### Structure Definition
```c
typedef struct {
    int parent[MAX_ELEMENTS];
    int rank[MAX_ELEMENTS];
    int size[MAX_ELEMENTS];
} DisjointSet;
```

### Key Optimizations
- **Path Compression**: Flatten tree structure during find
- **Union by Rank**: Attach smaller tree to larger tree
- **Time Complexity**: O(α(N)) - inverse Ackermann function

### Core Operations

#### Find with Path Compression
```c
int findSet(DisjointSet* ds, int x) {
    if (ds->parent[x] != x) {
        ds->parent[x] = findSet(ds, ds->parent[x]); // Path compression
    }
    return ds->parent[x];
}
```

#### Union by Rank
```c
void unionSets(DisjointSet* ds, int x, int y) {
    int xRoot = findSet(ds, x);
    int yRoot = findSet(ds, y);
    
    if (xRoot == yRoot) return; // Already in same set
    
    // Union by rank
    if (ds->rank[xRoot] < ds->rank[yRoot]) {
        ds->parent[xRoot] = yRoot;
        ds->size[yRoot] += ds->size[xRoot];
    } else if (ds->rank[xRoot] > ds->rank[yRoot]) {
        ds->parent[yRoot] = xRoot;
        ds->size[xRoot] += ds->size[yRoot];
    } else {
        ds->parent[yRoot] = xRoot;
        ds->rank[xRoot]++;
        ds->size[xRoot] += ds->size[yRoot];
    }
}
```

### Applications
- **Connected Components**: Find connected components in graphs
- **Kruskal's Algorithm**: Minimum spanning tree construction
- **Image Processing**: Connected component labeling
- **Network Connectivity**: Check network connectivity

## 📊 Segment Tree

### Structure Definition
```c
typedef struct {
    int tree[4 * MAX_SEGMENT_SIZE];
    int size;
} SegmentTree;
```

### Key Properties
- **Tree Structure**: Complete binary tree
- **Space Complexity**: O(4 × N)
- **Time Complexity**: O(log N) for queries and updates
- **Use Case**: Range queries with point updates

### Core Operations

#### Build Tree
```c
void buildSegmentTree(SegmentTree* st, int arr[], int start, int end, int node) {
    if (start == end) {
        st->tree[node] = arr[start];
        return;
    }
    
    int mid = start + (end - start) / 2;
    buildSegmentTree(st, arr, start, mid, 2 * node + 1);
    buildSegmentTree(st, arr, mid + 1, end, 2 * node + 2);
    
    st->tree[node] = st->tree[2 * node + 1] + st->tree[2 * node + 2];
}
```

#### Range Query
```c
int querySegmentTree(SegmentTree* st, int start, int end, int l, int r, int node) {
    // No overlap
    if (r < start || end < l) {
        return 0;
    }
    
    // Complete overlap
    if (l <= start && end <= r) {
        return st->tree[node];
    }
    
    // Partial overlap
    int mid = start + (end - start) / 2;
    return querySegmentTree(st, start, mid, l, r, 2 * node + 1) +
           querySegmentTree(st, mid + 1, end, l, r, 2 * node + 2);
}
```

#### Point Update
```c
void updateSegmentTree(SegmentTree* st, int start, int end, int idx, int value, int node) {
    if (start == end) {
        st->tree[node] = value;
        return;
    }
    
    int mid = start + (end - start) / 2;
    if (idx <= mid) {
        updateSegmentTree(st, start, mid, idx, value, 2 * node + 1);
    } else {
        updateSegmentTree(st, mid + 1, end, idx, value, 2 * node + 2);
    }
    
    st->tree[node] = st->tree[2 * node + 1] + st->tree[2 * node + 2];
}
```

### Applications
- **Range Sum Queries**: Sum of elements in range
- **Range Minimum Queries**: Minimum in range
- **Range Maximum Queries**: Maximum in range
- **Lazy Propagation**: Range updates with queries

## 🌲 Fenwick Tree (Binary Indexed Tree)

### Structure Definition
```c
typedef struct {
    int tree[MAX_SEGMENT_SIZE + 1];
    int size;
} FenwickTree;
```

### Key Properties
- **Binary Indexed**: Uses binary representation
- **Space Complexity**: O(N)
- **Time Complexity**: O(log N) for queries and updates
- **1-based Indexing**: Simplifies bit operations

### Core Operations

#### Update Operation
```c
void updateFenwickTree(FenwickTree* ft, int idx, int delta) {
    idx++; // Convert to 1-based indexing
    
    while (idx <= ft->size) {
        ft->tree[idx] += delta;
        idx += idx & (-idx); // Add last set bit
    }
}
```

#### Prefix Sum Query
```c
int queryFenwickTree(FenwickTree* ft, int idx) {
    idx++; // Convert to 1-based indexing
    int sum = 0;
    
    while (idx > 0) {
        sum += ft->tree[idx];
        idx -= idx & (-idx); // Remove last set bit
    }
    
    return sum;
}
```

#### Range Sum Query
```c
int queryRangeFenwickTree(FenwickTree* ft, int l, int r) {
    return queryFenwickTree(ft, r) - queryFenwickTree(ft, l - 1);
}
```

### Applications
- **Prefix Sums**: Efficient prefix sum calculations
- **Range Sum Queries**: Sum of elements in range
- **Inversion Counting**: Count inversions in array
- **Dynamic Frequency Arrays**: Track element frequencies

## 💾 LRU Cache

### Structure Definition
```c
typedef struct CacheNode {
    int key;
    int value;
    struct CacheNode* prev;
    struct CacheNode* next;
} CacheNode;

typedef struct {
    CacheNode* head;
    CacheNode* tail;
    CacheNode* hash[CACHE_SIZE];
    int capacity;
    int size;
} LRUCache;
```

### Key Components
- **Doubly Linked List**: Maintain access order
- **Hash Table**: O(1) key lookup
- **Eviction Policy**: Remove least recently used

### Core Operations

#### Get Operation
```c
int get(LRUCache* cache, int key) {
    int hashIndex = cacheHash(key);
    CacheNode* current = cache->hash[hashIndex];
    
    while (current) {
        if (current->key == key) {
            moveToHead(cache, current); // Mark as recently used
            return current->value;
        }
        current = current->next;
    }
    
    return -1; // Not found
}
```

#### Put Operation
```c
void put(LRUCache* cache, int key, int value) {
    // Check if key already exists
    CacheNode* existing = findNode(cache, key);
    if (existing) {
        existing->value = value;
        moveToHead(cache, existing);
        return;
    }
    
    // Create new node
    CacheNode* newNode = createCacheNode(key, value);
    addToHash(cache, newNode);
    addToFront(cache, newNode);
    cache->size++;
    
    // Remove least recently used if capacity exceeded
    if (cache->size > cache->capacity) {
        removeTail(cache);
    }
}
```

### Applications
- **Memory Management**: Cache frequently accessed data
- **Database Systems**: Cache query results
- **Web Browsers**: Cache web pages and resources
- **Operating Systems**: Cache file system metadata

## 💡 Implementation Details

### Memory Management
```c
// Always free allocated memory
void freeTrie(TrieNode* root) {
    if (root == NULL) return;
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(root->children[i]);
    }
    free(root);
}
```

### Error Handling
```c
// Check for allocation failures
TrieNode* createTrieNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    // Initialize node...
    return node;
}
```

### Hash Functions
```c
// Simple hash function for cache
int cacheHash(int key) {
    return key % CACHE_SIZE;
}

// Better hash function (if needed)
int betterHash(int key) {
    key = ((key >> 2) ^ key) & 0x7fffffff;
    return key % CACHE_SIZE;
}
```

## 🚀 Advanced Topics

### 1. Trie Variants
- **Patricia Trie**: Compressed trie for space efficiency
- **Ternary Search Tree**: Three-way branching
- **Suffix Tree**: Efficient substring searching
- **Radix Tree**: Space-optimized trie

### 2. Advanced Union-Find
- **Persistent DSU**: Maintain history of operations
- **Dynamic Connectivity**: Handle edge insertions/deletions
- **Parallel DSU**: Concurrent operations

### 3. Segment Tree Extensions
- **Lazy Propagation**: Range updates
- **2D Segment Trees**: Matrix range queries
- **Persistent Segment Trees**: Version control
- **Wavelet Trees**: Range mode queries

### 4. Fenwick Tree Variants
- **2D Fenwick Trees**: Matrix prefix sums
- **Range Update Fenwick Trees**: Range modifications
- **Order Statistics Trees**: K-th element queries

### 5. Cache Variants
- **LFU Cache**: Least frequently used
- **ARC Cache**: Adaptive replacement cache
- **Write-Through Cache**: Immediate write-back
- **Write-Behind Cache**: Delayed write-back

## 📊 Complexity Analysis

| Data Structure | Insert | Search | Delete | Space |
|----------------|--------|--------|--------|-------|
| Trie | O(L) | O(L) | O(L) | O(N×L) |
| DSU | O(α(N)) | O(α(N)) | O(α(N)) | O(N) |
| Segment Tree | O(log N) | O(log N) | O(log N) | O(N) |
| Fenwick Tree | O(log N) | O(log N) | O(log N) | O(N) |
| LRU Cache | O(1) | O(1) | O(1) | O(C) |

*L = word length, N = elements, C = cache capacity, α(N) = inverse Ackermann*

## 🧪 Testing Strategies

### 1. Unit Testing
```c
void testTrieOperations() {
    TrieNode* root = createTrieNode();
    
    insertTrie(root, "hello");
    assert(searchTrie(root, "hello") == true);
    assert(searchTrie(root, "world") == false);
    assert(startsWith(root, "hel") == true);
    
    freeTrie(root);
}
```

### 2. Integration Testing
```c
void testLRUCacheEviction() {
    LRUCache cache;
    initLRUCache(&cache, 2);
    
    put(&cache, 1, 100);
    put(&cache, 2, 200);
    put(&cache, 3, 300); // Should evict key 1
    
    assert(get(&cache, 1) == -1); // Should be evicted
    assert(get(&cache, 2) == 200);
    assert(get(&cache, 3) == 300);
    
    freeLRUCache(&cache);
}
```

### 3. Performance Testing
```c
void benchmarkDataStructures() {
    clock_t start = clock();
    
    // Test operations
    for (int i = 0; i < 100000; i++) {
        // Perform data structure operations
    }
    
    clock_t end = clock();
    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Performance: %f seconds\n", time);
}
```

## ⚠️ Common Pitfalls

### 1. Memory Leaks in Trie
```c
// Wrong - forgetting to free children
void freeTrie(TrieNode* root) {
    free(root); // Only frees root, leaks children
}

// Right - recursively free all nodes
void freeTrie(TrieNode* root) {
    if (root == NULL) return;
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(root->children[i]);
    }
    free(root);
}
```

### 2. Indexing Errors in Fenwick Tree
```c
// Wrong - using 0-based indexing
void updateFenwickTree(FenwickTree* ft, int idx, int delta) {
    while (idx <= ft->size) { // idx starts from 0
        ft->tree[idx] += delta;
        idx += idx & (-idx);
    }
}

// Right - convert to 1-based indexing
void updateFenwickTree(FenwickTree* ft, int idx, int delta) {
    idx++; // Convert to 1-based
    while (idx <= ft->size) {
        ft->tree[idx] += delta;
        idx += idx & (-idx);
    }
}
```

### 3. Cache Hash Collisions
```c
// Wrong - poor hash function
int badHash(int key) {
    return key % 2; // Only 2 buckets!
}

// Right - better distribution
int goodHash(int key) {
    return key % CACHE_SIZE; // Use full cache size
}
```

### 4. Segment Tree Range Errors
```c
// Wrong - incorrect range handling
int querySegmentTree(SegmentTree* st, int l, int r) {
    // Missing bounds checking
    return querySegmentTree(st, 0, st->size - 1, l, r, 0);
}

// Right - proper range validation
int querySegmentTree(SegmentTree* st, int l, int r) {
    if (l < 0 || r >= st->size || l > r) {
        printf("Invalid range!\n");
        return 0;
    }
    return querySegmentTree(st, 0, st->size - 1, l, r, 0);
}
```

## 🔧 Real-World Applications

### 1. Search Engines
```c
// Trie for autocomplete
typedef struct SearchTrie {
    TrieNode* root;
    char** suggestions;
    int suggestionCount;
} SearchTrie;
```

### 2. Social Networks
```c
// DSU for friend connections
typedef struct SocialNetwork {
    DisjointSet ds;
    int numUsers;
    char** names;
} SocialNetwork;
```

### 3. Financial Systems
```c
// Segment tree for stock prices
typedef struct StockAnalyzer {
    SegmentTree priceTree;
    SegmentTree volumeTree;
    int numDays;
} StockAnalyzer;
```

### 4. Web Caching
```c
// LRU cache for web pages
typedef struct WebCache {
    LRUCache cache;
    int maxAge; // Time-based eviction
    time_t lastCleanup;
} WebCache;
```

## 🎓 Learning Path

### Intermediate Level
1. **Trie**: String processing and autocomplete
2. **DSU**: Connected components and MST
3. **Segment Tree**: Range queries basics

### Advanced Level
1. **Fenwick Tree**: Efficient prefix sums
2. **LRU Cache**: Memory management
3. **Advanced Variants**: Specialized data structures

### Expert Level
1. **Persistent Structures**: Version control
2. **Parallel Data Structures**: Concurrent operations
3. **Hybrid Structures**: Combining multiple DS

These advanced data structures provide powerful tools for solving complex problems efficiently and are essential for competitive programming and real-world applications.
