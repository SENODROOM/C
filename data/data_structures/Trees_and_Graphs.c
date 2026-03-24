#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// =============================================================================
// BINARY SEARCH TREE
// =============================================================================

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Create new tree node
TreeNode* createTreeNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode) {
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

// Insert node into BST
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

// Search in BST
TreeNode* searchBST(TreeNode* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    
    if (data < root->data) {
        return searchBST(root->left, data);
    }
    
    return searchBST(root->right, data);
}

// Find minimum value node in BST
TreeNode* findMin(TreeNode* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Delete node from BST
TreeNode* deleteBST(TreeNode* root, int data) {
    if (root == NULL) return root;
    
    if (data < root->data) {
        root->left = deleteBST(root->left, data);
    } else if (data > root->data) {
        root->right = deleteBST(root->right, data);
    } else {
        // Node with only one child or no child
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }
        
        // Node with two children: get inorder successor
        TreeNode* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteBST(root->right, temp->data);
    }
    
    return root;
}

// Inorder traversal (sorted order)
void inorderTraversal(TreeNode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

// Preorder traversal
void preorderTraversal(TreeNode* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

// Postorder traversal
void postorderTraversal(TreeNode* root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf("%d ", root->data);
    }
}

// Calculate tree height
int treeHeight(TreeNode* root) {
    if (root == NULL) return 0;
    
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// Count nodes in tree
int countNodes(TreeNode* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Free tree memory
void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// =============================================================================
// HASH TABLE
// =============================================================================

#define HASH_TABLE_SIZE 10

typedef struct HashNode {
    int key;
    char value[100];
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *table[HASH_TABLE_SIZE];
} HashTable;

// Hash function
int hashFunction(int key) {
    return key % HASH_TABLE_SIZE;
}

// Create hash table
HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (ht) {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            ht->table[i] = NULL;
        }
    }
    return ht;
}

// Create hash node
HashNode* createHashNode(int key, const char* value) {
    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (node) {
        node->key = key;
        strcpy(node->value, value);
        node->next = NULL;
    }
    return node;
}

// Insert into hash table
void insertHash(HashTable* ht, int key, const char* value) {
    int index = hashFunction(key);
    
    HashNode* newNode = createHashNode(key, value);
    
    if (ht->table[index] == NULL) {
        ht->table[index] = newNode;
    } else {
        // Handle collision (chaining)
        HashNode* current = ht->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Search in hash table
char* searchHash(HashTable* ht, int key) {
    int index = hashFunction(key);
    
    HashNode* current = ht->table[index];
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    
    return NULL;
}

// Delete from hash table
int deleteHash(HashTable* ht, int key) {
    int index = hashFunction(key);
    
    HashNode* current = ht->table[index];
    HashNode* prev = NULL;
    
    while (current != NULL && current->key != key) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) return 0; // Key not found
    
    if (prev == NULL) {
        ht->table[index] = current->next;
    } else {
        prev->next = current->next;
    }
    
    free(current);
    return 1;
}

// Free hash table
void freeHashTable(HashTable* ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode* current = ht->table[i];
        while (current != NULL) {
            HashNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht);
}

// =============================================================================
// PRIORITY QUEUE (MAX HEAP)
// =============================================================================

#define MAX_HEAP_SIZE 100

typedef struct {
    int data[MAX_HEAP_SIZE];
    int size;
} MaxHeap;

// Initialize heap
void initHeap(MaxHeap* heap) {
    heap->size = 0;
}

// Get parent index
int parent(int i) {
    return (i - 1) / 2;
}

// Get left child index
int leftChild(int i) {
    return 2 * i + 1;
}

// Get right child index
int rightChild(int i) {
    return 2 * i + 2;
}

// Swap two elements
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify up
void heapifyUp(MaxHeap* heap, int index) {
    while (index > 0 && heap->data[parent(index)] < heap->data[index]) {
        swap(&heap->data[parent(index)], &heap->data[index]);
        index = parent(index);
    }
}

// Heapify down
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

// Insert into heap
int insertHeap(MaxHeap* heap, int value) {
    if (heap->size >= MAX_HEAP_SIZE) {
        printf("Heap overflow!\n");
        return 0;
    }
    
    heap->data[heap->size] = value;
    heapifyUp(heap, heap->size);
    heap->size++;
    return 1;
}

// Extract maximum from heap
int extractMax(MaxHeap* heap) {
    if (heap->size <= 0) {
        printf("Heap underflow!\n");
        return INT_MIN;
    }
    
    if (heap->size == 1) {
        heap->size--;
        return heap->data[0];
    }
    
    int max = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    
    return max;
}

// Get maximum element
int getMax(MaxHeap* heap) {
    if (heap->size <= 0) return INT_MIN;
    return heap->data[0];
}

// Check if heap is empty
int isHeapEmpty(MaxHeap* heap) {
    return heap->size == 0;
}

// Print heap
void printHeap(MaxHeap* heap) {
    printf("Heap: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateBinarySearchTree() {
    printf("=== BINARY SEARCH TREE ===\n");
    
    TreeNode* root = NULL;
    
    // Insert elements
    int elements[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) {
        root = insertBST(root, elements[i]);
        printf("Inserted: %d\n", elements[i]);
    }
    
    printf("\nInorder traversal (sorted): ");
    inorderTraversal(root);
    printf("\n");
    
    printf("Preorder traversal: ");
    preorderTraversal(root);
    printf("\n");
    
    printf("Postorder traversal: ");
    postorderTraversal(root);
    printf("\n");
    
    printf("\nTree height: %d\n", treeHeight(root));
    printf("Node count: %d\n", countNodes(root));
    
    // Search operations
    printf("\nSearching for 40: %s\n", searchBST(root, 40) ? "Found" : "Not found");
    printf("Searching for 99: %s\n", searchBST(root, 99) ? "Found" : "Not found");
    
    // Delete operations
    printf("\nDeleting 20, 70, 50\n");
    root = deleteBST(root, 20);
    root = deleteBST(root, 70);
    root = deleteBST(root, 50);
    
    printf("Inorder after deletions: ");
    inorderTraversal(root);
    printf("\n");
    
    freeTree(root);
    printf("\n");
}

void demonstrateHashTable() {
    printf("=== HASH TABLE ===\n");
    
    HashTable* ht = createHashTable();
    
    // Insert key-value pairs
    insertHash(ht, 1, "Apple");
    insertHash(ht, 2, "Banana");
    insertHash(ht, 11, "Orange"); // Same hash as 1
    insertHash(ht, 21, "Grape"); // Same hash as 1
    insertHash(ht, 5, "Mango");
    
    printf("Inserted key-value pairs:\n");
    printf("1: %s\n", searchHash(ht, 1));
    printf("2: %s\n", searchHash(ht, 2));
    printf("11: %s\n", searchHash(ht, 11));
    printf("21: %s\n", searchHash(ht, 21));
    printf("5: %s\n", searchHash(ht, 5));
    
    printf("\nSearching for key 3: %s\n", searchHash(ht, 3) ? "Found" : "Not found");
    
    printf("\nDeleting key 11\n");
    if (deleteHash(ht, 11)) {
        printf("Key 11 deleted successfully\n");
    }
    
    printf("Searching for key 11 after deletion: %s\n", searchHash(ht, 11) ? "Found" : "Not found");
    
    freeHashTable(ht);
    printf("\n");
}

void demonstratePriorityQueue() {
    printf("=== PRIORITY QUEUE (MAX HEAP) ===\n");
    
    MaxHeap heap;
    initHeap(&heap);
    
    // Insert elements
    int elements[] = {30, 20, 50, 10, 40, 60, 25};
    for (int i = 0; i < 7; i++) {
        insertHeap(&heap, elements[i]);
        printf("Inserted: %d\n", elements[i]);
    }
    
    printf("\nHeap after insertions: ");
    printHeap(&heap);
    
    printf("\nExtracting max elements:\n");
    while (!isHeapEmpty(&heap)) {
        int max = extractMax(&heap);
        printf("Extracted: %d, Remaining: ", max);
        printHeap(&heap);
    }
    
    printf("\n");
}

void demonstrateTreeApplications() {
    printf("=== TREE APPLICATIONS ===\n");
    
    // Build a sample BST
    TreeNode* root = NULL;
    int sampleData[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    
    for (int i = 0; i < 9; i++) {
        root = insertBST(root, sampleData[i]);
    }
    
    printf("Sample BST: ");
    inorderTraversal(root);
    printf("\n");
    
    // Find minimum and maximum
    TreeNode* minNode = findMin(root);
    printf("Minimum value: %d\n", minNode ? minNode->data : -1);
    
    // Find maximum (rightmost node)
    TreeNode* current = root;
    while (current && current->right) {
        current = current->right;
    }
    printf("Maximum value: %d\n", current ? current->data : -1);
    
    // Check if tree is balanced (simplified)
    int leftHeight = treeHeight(root->left);
    int rightHeight = treeHeight(root->right);
    printf("Left subtree height: %d\n", leftHeight);
    printf("Right subtree height: %d\n", rightHeight);
    printf("Tree is %sbalanced\n", abs(leftHeight - rightHeight) <= 1 ? "" : "not ");
    
    freeTree(root);
    printf("\n");
}

int main() {
    printf("Trees and Graphs Data Structures\n");
    printf("================================\n\n");
    
    demonstrateBinarySearchTree();
    demonstrateHashTable();
    demonstratePriorityQueue();
    demonstrateTreeApplications();
    
    printf("All tree and graph data structures demonstrated!\n");
    return 0;
}
