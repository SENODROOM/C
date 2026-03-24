#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

// =============================================================================
// TRIE (PREFIX TREE)
// =============================================================================

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    int frequency; // For word frequency counting
} TrieNode;

// Create new trie node
TrieNode* createTrieNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    if (node) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
        node->isEndOfWord = false;
        node->frequency = 0;
    }
    return node;
}

// Insert word into trie
void insertTrie(TrieNode* root, const char* word) {
    TrieNode* current = root;
    int index;
    
    for (int level = 0; word[level]; level++) {
        index = word[level] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) continue; // Skip non-lowercase
        
        if (current->children[index] == NULL) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    
    current->isEndOfWord = true;
    current->frequency++;
}

// Search word in trie
bool searchTrie(TrieNode* root, const char* word) {
    TrieNode* current = root;
    int index;
    
    for (int level = 0; word[level]; level++) {
        index = word[level] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) return false;
        
        if (current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }
    
    return current->isEndOfWord;
}

// Check if any word starts with prefix
bool startsWith(TrieNode* root, const char* prefix) {
    TrieNode* current = root;
    int index;
    
    for (int level = 0; prefix[level]; level++) {
        index = prefix[level] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) return false;
        
        if (current->children[index] == NULL) {
            return false;
        }
        current = current->children[index];
    }
    
    return true;
}

// Get word frequency
int getWordFrequency(TrieNode* root, const char* word) {
    TrieNode* current = root;
    int index;
    
    for (int level = 0; word[level]; level++) {
        index = word[level] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) return 0;
        
        if (current->children[index] == NULL) {
            return 0;
        }
        current = current->children[index];
    }
    
    return current->frequency;
}

// Free trie memory
void freeTrie(TrieNode* root) {
    if (root == NULL) return;
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(root->children[i]);
    }
    free(root);
}

// =============================================================================
// DISJOINT SET UNION (UNION-FIND)
// =============================================================================

#define MAX_ELEMENTS 100

typedef struct {
    int parent[MAX_ELEMENTS];
    int rank[MAX_ELEMENTS];
    int size[MAX_ELEMENTS];
} DisjointSet;

// Initialize disjoint set
void initDisjointSet(DisjointSet* ds, int n) {
    for (int i = 0; i < n; i++) {
        ds->parent[i] = i;
        ds->rank[i] = 0;
        ds->size[i] = 1;
    }
}

// Find set with path compression
int findSet(DisjointSet* ds, int x) {
    if (ds->parent[x] != x) {
        ds->parent[x] = findSet(ds, ds->parent[x]); // Path compression
    }
    return ds->parent[x];
}

// Union sets by rank
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

// Check if two elements are in same set
bool inSameSet(DisjointSet* ds, int x, int y) {
    return findSet(ds, x) == findSet(ds, y);
}

// Get set size
int getSetSize(DisjointSet* ds, int x) {
    return ds->size[findSet(ds, x)];
}

// =============================================================================
// SEGMENT TREE
// =============================================================================

#define MAX_SEGMENT_SIZE 1000

typedef struct {
    int tree[4 * MAX_SEGMENT_SIZE];
    int size;
} SegmentTree;

// Build segment tree
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

// Initialize segment tree
void initSegmentTree(SegmentTree* st, int arr[], int n) {
    st->size = n;
    buildSegmentTree(st, arr, 0, n - 1, 0);
}

// Update element at index
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

// Query range sum
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

// =============================================================================
// FENWICK TREE (BINARY INDEXED TREE)
// =============================================================================

typedef struct {
    int tree[MAX_SEGMENT_SIZE + 1];
    int size;
} FenwickTree;

// Initialize Fenwick tree
void initFenwickTree(FenwickTree* ft, int arr[], int n) {
    ft->size = n;
    
    // Initialize tree with zeros
    for (int i = 1; i <= n; i++) {
        ft->tree[i] = 0;
    }
    
    // Build tree
    for (int i = 0; i < n; i++) {
        updateFenwickTree(ft, i, arr[i]);
    }
}

// Update element at index
void updateFenwickTree(FenwickTree* ft, int idx, int delta) {
    idx++; // Convert to 1-based indexing
    
    while (idx <= ft->size) {
        ft->tree[idx] += delta;
        idx += idx & (-idx); // Add last set bit
    }
}

// Query prefix sum
int queryFenwickTree(FenwickTree* ft, int idx) {
    idx++; // Convert to 1-based indexing
    int sum = 0;
    
    while (idx > 0) {
        sum += ft->tree[idx];
        idx -= idx & (-idx); // Remove last set bit
    }
    
    return sum;
}

// Query range sum
int queryRangeFenwickTree(FenwickTree* ft, int l, int r) {
    return queryFenwickTree(ft, r) - queryFenwickTree(ft, l - 1);
}

// =============================================================================
// LRU CACHE (LEAST RECENTLY USED)
// =============================================================================

#define CACHE_SIZE 5

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

// Create new cache node
CacheNode* createCacheNode(int key, int value) {
    CacheNode* node = (CacheNode*)malloc(sizeof(CacheNode));
    if (node) {
        node->key = key;
        node->value = value;
        node->prev = NULL;
        node->next = NULL;
    }
    return node;
}

// Initialize LRU cache
void initLRUCache(LRUCache* cache, int capacity) {
    cache->head = NULL;
    cache->tail = NULL;
    cache->capacity = capacity;
    cache->size = 0;
    
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache->hash[i] = NULL;
    }
}

// Hash function for cache
int cacheHash(int key) {
    return key % CACHE_SIZE;
}

// Remove node from doubly linked list
void removeNode(LRUCache* cache, CacheNode* node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        cache->head = node->next;
    }
    
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        cache->tail = node->prev;
    }
}

// Add node to front of list (most recently used)
void addToFront(LRUCache* cache, CacheNode* node) {
    node->next = cache->head;
    node->prev = NULL;
    
    if (cache->head) {
        cache->head->prev = node;
    }
    cache->head = node;
    
    if (cache->tail == NULL) {
        cache->tail = node;
    }
}

// Move node to front (mark as recently used)
void moveToHead(LRUCache* cache, CacheNode* node) {
    removeNode(cache, node);
    addToFront(cache, node);
}

// Remove tail node (least recently used)
void removeTail(LRUCache* cache) {
    CacheNode* last = cache->tail;
    if (last) {
        removeNode(cache, last);
        
        // Remove from hash
        int hashIndex = cacheHash(last->key);
        CacheNode* current = cache->hash[hashIndex];
        if (current == last) {
            cache->hash[hashIndex] = last->next;
        } else {
            while (current && current->next != last) {
                current = current->next;
            }
            if (current) {
                current->next = last->next;
            }
        }
        
        free(last);
        cache->size--;
    }
}

// Get value from cache
int get(LRUCache* cache, int key) {
    int hashIndex = cacheHash(key);
    CacheNode* current = cache->hash[hashIndex];
    
    while (current) {
        if (current->key == key) {
            moveToHead(cache, current);
            return current->value;
        }
        current = current->next;
    }
    
    return -1; // Not found
}

// Put value in cache
void put(LRUCache* cache, int key, int value) {
    int hashIndex = cacheHash(key);
    CacheNode* current = cache->hash[hashIndex];
    
    // Check if key already exists
    while (current) {
        if (current->key == key) {
            current->value = value;
            moveToHead(cache, current);
            return;
        }
        current = current->next;
    }
    
    // Create new node
    CacheNode* newNode = createCacheNode(key, value);
    newNode->next = cache->hash[hashIndex];
    cache->hash[hashIndex] = newNode;
    
    addToFront(cache, newNode);
    cache->size++;
    
    // Remove least recently used if capacity exceeded
    if (cache->size > cache->capacity) {
        removeTail(cache);
    }
}

// Free LRU cache
void freeLRUCache(LRUCache* cache) {
    CacheNode* current = cache->head;
    while (current) {
        CacheNode* temp = current;
        current = current->next;
        free(temp);
    }
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateTrie() {
    printf("=== TRIE (PREFIX TREE) ===\n");
    
    TrieNode* root = createTrieNode();
    
    // Insert words
    const char* words[] = {"hello", "help", "hell", "heaven", "heavy"};
    for (int i = 0; i < 5; i++) {
        insertTrie(root, words[i]);
        printf("Inserted: %s\n", words[i]);
    }
    
    // Search words
    printf("\nSearch results:\n");
    printf("hello: %s\n", searchTrie(root, "hello") ? "Found" : "Not found");
    printf("help: %s\n", searchTrie(root, "help") ? "Found" : "Not found");
    printf("world: %s\n", searchTrie(root, "world") ? "Found" : "Not found");
    
    // Prefix search
    printf("\nPrefix search:\n");
    printf("Words starting with 'hel': %s\n", startsWith(root, "hel") ? "Yes" : "No");
    printf("Words starting with 'wor': %s\n", startsWith(root, "wor") ? "Yes" : "No");
    
    // Word frequencies
    insertTrie(root, "hello"); // Insert again
    insertTrie(root, "hello"); // Insert again
    
    printf("\nWord frequencies:\n");
    printf("hello: %d\n", getWordFrequency(root, "hello"));
    printf("help: %d\n", getWordFrequency(root, "help"));
    
    freeTrie(root);
    printf("\n");
}

void demonstrateDisjointSet() {
    printf("=== DISJOINT SET UNION ===\n");
    
    DisjointSet ds;
    int n = 10;
    initDisjointSet(&ds, n);
    
    // Union operations
    unionSets(&ds, 0, 1);
    unionSets(&ds, 2, 3);
    unionSets(&ds, 4, 5);
    unionSets(&ds, 1, 2); // Merge sets {0,1} and {2,3}
    unionSets(&ds, 6, 7);
    unionSets(&ds, 8, 9);
    
    printf("Union operations completed\n");
    
    // Check sets
    printf("\nSet membership:\n");
    printf("0 and 3 in same set: %s\n", inSameSet(&ds, 0, 3) ? "Yes" : "No");
    printf("4 and 5 in same set: %s\n", inSameSet(&ds, 4, 5) ? "Yes" : "No");
    printf("0 and 4 in same set: %s\n", inSameSet(&ds, 0, 4) ? "Yes" : "No");
    
    // Set sizes
    printf("\nSet sizes:\n");
    printf("Set containing 0: %d elements\n", getSetSize(&ds, 0));
    printf("Set containing 4: %d elements\n", getSetSize(&ds, 4));
    printf("Set containing 8: %d elements\n", getSetSize(&ds, 8));
    
    printf("\n");
}

void demonstrateSegmentTree() {
    printf("=== SEGMENT TREE ===\n");
    
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    SegmentTree st;
    initSegmentTree(&st, arr, n);
    
    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // Range sum queries
    printf("\nRange sum queries:\n");
    printf("Sum of range [1, 3]: %d\n", querySegmentTree(&st, 0, n - 1, 1, 3, 0));
    printf("Sum of range [0, 5]: %d\n", querySegmentTree(&st, 0, n - 1, 0, 5, 0));
    printf("Sum of range [2, 4]: %d\n", querySegmentTree(&st, 0, n - 1, 2, 4, 0));
    
    // Update operation
    printf("\nUpdating index 2 to 6\n");
    updateSegmentTree(&st, 0, n - 1, 2, 6, 0);
    
    printf("Sum of range [1, 3] after update: %d\n", querySegmentTree(&st, 0, n - 1, 1, 3, 0));
    
    printf("\n");
}

void demonstrateFenwickTree() {
    printf("=== FENWICK TREE (BINARY INDEXED TREE) ===\n");
    
    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    FenwickTree ft;
    initFenwickTree(&ft, arr, n);
    
    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // Prefix sum queries
    printf("\nPrefix sum queries:\n");
    printf("Sum of first 3 elements: %d\n", queryFenwickTree(&ft, 2));
    printf("Sum of first 5 elements: %d\n", queryFenwickTree(&ft, 4));
    printf("Sum of all elements: %d\n", queryFenwickTree(&ft, n - 1));
    
    // Range sum queries
    printf("\nRange sum queries:\n");
    printf("Sum of range [1, 3]: %d\n", queryRangeFenwickTree(&ft, 1, 3));
    printf("Sum of range [2, 5]: %d\n", queryRangeFenwickTree(&ft, 2, 5));
    
    // Update operation
    printf("\nUpdating index 2 by adding 2\n");
    updateFenwickTree(&ft, 2, 2);
    
    printf("Sum of first 3 elements after update: %d\n", queryFenwickTree(&ft, 2));
    
    printf("\n");
}

void demonstrateLRUCache() {
    printf("=== LRU CACHE ===\n");
    
    LRUCache cache;
    initLRUCache(&cache, 3); // Capacity of 3
    
    // Put operations
    put(&cache, 1, 100);
    put(&cache, 2, 200);
    put(&cache, 3, 300);
    
    printf("Cache after inserting (1,100), (2,200), (3,300):\n");
    printf("Get 1: %d\n", get(&cache, 1));
    printf("Get 2: %d\n", get(&cache, 2));
    printf("Get 3: %d\n", get(&cache, 3));
    
    // This should evict key 2 (least recently used)
    put(&cache, 4, 400);
    
    printf("\nAfter inserting (4,400) (evicts key 2):\n");
    printf("Get 1: %d\n", get(&cache, 1));
    printf("Get 2: %d\n", get(&cache, 2)); // Should return -1 (not found)
    printf("Get 3: %d\n", get(&cache, 3));
    printf("Get 4: %d\n", get(&cache, 4));
    
    // Access key 1 (makes it most recently used)
    get(&cache, 1);
    
    // Insert key 5 (should evict key 3)
    put(&cache, 5, 500);
    
    printf("\nAfter accessing key 1 and inserting (5,500):\n");
    printf("Get 1: %d\n", get(&cache, 1));
    printf("Get 3: %d\n", get(&cache, 3)); // Should return -1 (not found)
    printf("Get 4: %d\n", get(&cache, 4));
    printf("Get 5: %d\n", get(&cache, 5));
    
    freeLRUCache(&cache);
    printf("\n");
}

int main() {
    printf("Advanced Data Structures\n");
    printf("=========================\n\n");
    
    demonstrateTrie();
    demonstrateDisjointSet();
    demonstrateSegmentTree();
    demonstrateFenwickTree();
    demonstrateLRUCache();
    
    printf("All advanced data structures demonstrated!\n");
    return 0;
}
