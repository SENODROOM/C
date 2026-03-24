# Trees and Graphs Data Structures

This file contains implementations of fundamental tree and graph data structures in C, including Binary Search Trees, Hash Tables, and Priority Queues (Heaps).

## 📚 Data Structure Overview

### 🌳 Trees
Hierarchical data structures with parent-child relationships

### 🔀 Graphs
Network data structures with nodes and edges

### 🗃️ Hash Tables
Key-value pair data structures with O(1) average lookup

### 📊 Priority Queues
Data structures that maintain elements in priority order

## 🌳 Binary Search Tree (BST)

### Structure Definition
```c
typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;
```

### Key Properties
- **BST Property**: Left subtree < node < Right subtree
- **Time Complexity**: O(log n) average, O(n) worst case
- **Space Complexity**: O(n)
- **Operations**: Insert, Delete, Search, Traversal

### Core Operations

#### Insertion
```c
TreeNode* insertBST(TreeNode* root, int data) {
    if (root == NULL) {
        return createTreeNode(data);
    }
    
    if (data < root->data) {
        root->left = insertBST(root->left, data);
    } else if (data > root->data) {
        root->right = insertBST(root->right, data);
    }
    
    return root;
}
```

#### Searching
```c
TreeNode* searchBST(TreeNode* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    
    return (data < root->data) ? 
           searchBST(root->left, data) : 
           searchBST(root->right, data);
}
```

#### Deletion
```c
TreeNode* deleteBST(TreeNode* root, int data) {
    // Three cases:
    // 1. No child
    // 2. One child
    // 3. Two children (use inorder successor)
}
```

### Tree Traversals

#### Inorder (Left, Root, Right)
- **Result**: Sorted order for BST
- **Use Case**: Print elements in order

#### Preorder (Root, Left, Right)
- **Use Case**: Tree copying, prefix notation

#### Postorder (Left, Right, Root)
- **Use Case**: Tree deletion, postfix notation

### Applications
- **Binary Search**: Efficient searching
- **Expression Trees**: Mathematical expressions
- **File Systems**: Directory structures
- **Database Indexing**: B-trees and variants

## 🔀 Hash Table

### Structure Definition
```c
typedef struct HashNode {
    int key;
    char value[100];
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *table[HASH_TABLE_SIZE];
} HashTable;
```

### Key Components
- **Hash Function**: Maps keys to array indices
- **Collision Resolution**: Chaining (linked lists)
- **Load Factor**: Ratio of elements to table size

### Hash Function
```c
int hashFunction(int key) {
    return key % HASH_TABLE_SIZE;
}
```

### Collision Handling
```c
// Chaining implementation
if (ht->table[index] == NULL) {
    ht->table[index] = newNode;
} else {
    // Add to linked list at this index
    HashNode* current = ht->table[index];
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}
```

### Operations
- **Insert**: O(1) average, O(n) worst case
- **Search**: O(1) average, O(n) worst case
- **Delete**: O(1) average, O(n) worst case

### Applications
- **Symbol Tables**: Compiler implementation
- **Caching**: Fast lookup of computed values
- **Database Indexing**: Quick record access
- **Associative Arrays**: Key-value storage

## 📊 Priority Queue (Max Heap)

### Structure Definition
```c
typedef struct {
    int data[MAX_HEAP_SIZE];
    int size;
} MaxHeap;
```

### Heap Properties
- **Complete Binary Tree**: All levels filled except possibly last
- **Heap Property**: Parent ≥ children (max heap)
- **Array Representation**: Efficient storage

### Core Operations

#### Insertion
```c
void heapifyUp(MaxHeap* heap, int index) {
    while (index > 0 && heap->data[parent(index)] < heap->data[index]) {
        swap(&heap->data[parent(index)], &heap->data[index]);
        index = parent(index);
    }
}
```

#### Extraction
```c
void heapifyDown(MaxHeap* heap, int index) {
    int largest = index;
    int left = leftChild(index);
    int right = rightChild(index);
    
    if (left < heap->size && heap->data[left] > heap->data[largest]) {
        largest = left;
    }
    
    if (right < heap->size && heap->data[right] > heap->data[largest]) {
        largest = right;
    }
    
    if (largest != index) {
        swap(&heap->data[index], &heap->data[largest]);
        heapifyDown(heap, largest);
    }
}
```

### Performance
- **Insert**: O(log n)
- **Extract Max**: O(log n)
- **Get Max**: O(1)

### Applications
- **Priority Scheduling**: Task scheduling
- **Heap Sort**: Efficient sorting algorithm
- **Event Simulation**: Priority event processing
- **Graph Algorithms**: Dijkstra's, Prim's

## 💡 Implementation Details

### Memory Management
```c
// Always free allocated memory
void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}
```

### Error Handling
```c
// Check for allocation failures
TreeNode* createTreeNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    // Initialize node...
    return newNode;
}
```

### Edge Cases
- **Empty Tree**: Handle NULL root
- **Single Node**: Base case for recursion
- **Duplicate Keys**: Handle appropriately in BST
- **Full Hash Table**: Handle overflow

## 🚀 Advanced Topics

### 1. Self-Balancing Trees
- **AVL Trees**: Height-balanced BST
- **Red-Black Trees**: Color-balanced BST
- **Splay Trees**: Access-balanced BST

### 2. Advanced Hashing
- **Open Addressing**: Linear probing, quadratic probing
- **Double Hashing**: Two hash functions
- **Cuckoo Hashing**: Displacement hashing

### 3. Heap Variants
- **Min Heap**: Smallest element at root
- **Fibonacci Heap**: Amortized O(1) operations
- **Binomial Heap**: Forest of heaps

### 4. Graph Representations
- **Adjacency Matrix**: O(V²) space
- **Adjacency List**: O(V + E) space
- **Edge List**: O(E) space

## 📊 Complexity Analysis

| Data Structure | Insert | Search | Delete | Space |
|----------------|--------|--------|--------|-------|
| BST | O(log n) | O(log n) | O(log n) | O(n) |
| Hash Table | O(1) | O(1) | O(1) | O(n) |
| Max Heap | O(log n) | O(1) | O(log n) | O(n) |

*Average case for balanced BST and hash table*

## 🧪 Testing Strategies

### 1. Unit Testing
```c
void testBSTInsertion() {
    TreeNode* root = NULL;
    root = insertBST(root, 50);
    assert(root != NULL);
    assert(root->data == 50);
    
    root = insertBST(root, 30);
    assert(root->left != NULL);
    assert(root->left->data == 30);
    
    freeTree(root);
}
```

### 2. Integration Testing
```c
void testHashTableOperations() {
    HashTable* ht = createHashTable();
    
    insertHash(ht, 1, "test");
    assert(strcmp(searchHash(ht, 1), "test") == 0);
    
    assert(deleteHash(ht, 1) == 1);
    assert(searchHash(ht, 1) == NULL);
    
    freeHashTable(ht);
}
```

### 3. Performance Testing
```c
void benchmarkBSTOperations() {
    TreeNode* root = NULL;
    clock_t start = clock();
    
    // Insert many elements
    for (int i = 0; i < 10000; i++) {
        root = insertBST(root, i);
    }
    
    clock_t end = clock();
    double time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("BST insertion time: %f seconds\n", time);
    
    freeTree(root);
}
```

## ⚠️ Common Pitfalls

### 1. Memory Leaks
```c
// Wrong - forgetting to free
TreeNode* root = createTreeNode(42);
// Use root but forget to free it

// Right - always free allocated memory
TreeNode* root = createTreeNode(42);
// Use root...
freeTree(root);
```

### 2. Null Pointer Dereference
```c
// Wrong - not checking for NULL
TreeNode* result = searchBST(root, key);
printf("%d\n", result->data); // Crash if result is NULL

// Right - check for NULL
TreeNode* result = searchBST(root, key);
if (result != NULL) {
    printf("%d\n", result->data);
}
```

### 3. Unbalanced BST
```c
// Wrong - inserting sorted data creates degenerate tree
for (int i = 0; i < 1000; i++) {
    root = insertBST(root, i); // Creates linked list
}

// Right - use self-balancing tree or random insertion
srand(time(NULL));
for (int i = 0; i < 1000; i++) {
    root = insertBST(root, rand() % 1000);
}
```

### 4. Hash Collisions
```c
// Wrong - poor hash function
int badHash(int key) {
    return key % 10; // Too many collisions
}

// Right - better hash function
int goodHash(int key) {
    return key % 101; // Prime number, better distribution
}
```

## 🔧 Real-World Applications

### 1. Database Systems
```c
// B-tree for database indexing
typedef struct BTreeNode {
    int keys[4]; // 4-way B-tree
    struct BTreeNode* children[5];
    int isLeaf;
} BTreeNode;
```

### 2. Compilers
```c
// Symbol table for identifiers
typedef struct Symbol {
    char name[50];
    int type;
    int scope;
    struct Symbol* next;
} Symbol;
```

### 3. Network Routing
```c
// Priority queue for Dijkstra's algorithm
typedef struct RouteNode {
    int vertex;
    int distance;
    int previous;
} RouteNode;
```

### 4. File Systems
```c
// Directory tree structure
typedef struct FileNode {
    char name[256];
    int isDirectory;
    struct FileNode* children;
    int childCount;
} FileNode;
```

## 🎓 Learning Path

### Beginner Level
1. **BST Basics**: Insert, search, delete
2. **Hash Tables**: Simple key-value storage
3. **Heaps**: Priority queue operations

### Intermediate Level
1. **Tree Traversals**: All three traversal methods
2. **Hash Functions**: Design and analysis
3. **Heap Applications**: Heap sort, priority queues

### Advanced Level
1. **Balanced Trees**: AVL, Red-Black trees
2. **Advanced Hashing**: Open addressing, double hashing
3. **Complex Applications**: Database indexing, compilers

## 🔄 Algorithm Selection Guide

### When to Use BST
- **Sorted Data**: Need elements in order
- **Range Queries**: Find elements in range
- **Dynamic Sets**: Frequent insertions/deletions

### When to Use Hash Table
- **Fast Lookup**: O(1) average access
- **Key-Value Mapping**: Simple association
- **Cache Implementation**: Quick data retrieval

### When to Use Heap
- **Priority Processing**: Always need max/min
- **Scheduling**: Task priority management
- **Streaming**: Process elements in priority order

These data structures form the foundation of efficient algorithms and are essential for building performant applications in C.
