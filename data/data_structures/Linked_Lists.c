/* Linked List Implementation in C */
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

typedef struct Node Node;

/* Create a new node */
Node* createNode(int data) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

/* Insert at the beginning */
Node* insertBeginning(Node* head, int data) {
    Node* newNode = createNode(data);
    if (!newNode) return head;
    newNode->next = head;
    return newNode;
}

/* Insert at a specific position */
Node* insertAtPosition(Node* head, int data, int position) {
    if (position == 0) {
        return insertBeginning(head, data);
    }
    
    Node* temp = head;
    for (int i = 0; i < position - 1 && temp != NULL; i++) {
        temp = temp->next;
    }
    
    if (temp == NULL) {
        printf("Invalid position!\n");
        return head;
    }
    
    Node* newNode = createNode(data);
    if (!newNode) return head;
    newNode->next = temp->next;
    temp->next = newNode;
    return head;
}

/* Insert at the end */
Node* insertEnd(Node* head, int data) {
    Node* newNode = createNode(data);
    if (!newNode) return head;
    
    if (head == NULL) {
        return newNode;
    }
    
    Node* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    return head;
}

/* Delete a node */
Node* deleteNode(Node* head, int data) {
    if (head == NULL) return NULL;
    
    if (head->data == data) {
        Node* temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    
    Node* temp = head;
    while (temp->next != NULL && temp->next->data != data) {
        temp = temp->next;
    }
    
    if (temp->next != NULL) {
        Node* nodeToDelete = temp->next;
        temp->next = nodeToDelete->next;
        free(nodeToDelete);
    }
    
    return head;
}

/* Display the linked list */
void displayList(Node* head) {
    if (head == NULL) {
        printf("List is empty!\n");
        return;
    }
    
    printf("List: ");
    Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

/* Count nodes */
int countNodes(Node* head) {
    int count = 0;
    Node* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

/* Search for a node */
Node* search(Node* head, int data) {
    Node* temp = head;
    while (temp != NULL) {
        if (temp->data == data) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/* Free the linked list */
void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* Main program */
int main() {
    Node* head = NULL;
    
    printf("=== Linked List Demo ===\n\n");
    
    head = insertEnd(head, 10);
    head = insertEnd(head, 20);
    head = insertEnd(head, 30);
    head = insertEnd(head, 40);
    printf("After inserting 10, 20, 30, 40:\n");
    displayList(head);
    
    head = insertBeginning(head, 5);
    printf("\nAfter inserting 5 at beginning:\n");
    displayList(head);
    
    head = insertAtPosition(head, 25, 3);
    printf("\nAfter inserting 25 at position 3:\n");
    displayList(head);
    
    printf("\nTotal nodes: %d\n", countNodes(head));
    
    head = deleteNode(head, 20);
    printf("\nAfter deleting 20:\n");
    displayList(head);
    
    Node* found = search(head, 30);
    if (found != NULL) {
        printf("\nFound 30 in the list!\n");
    }
    
    freeList(head);
    printf("\nList freed from memory\n");
    
    return 0;
}
