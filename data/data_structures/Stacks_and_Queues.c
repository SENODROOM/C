#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// =============================================================================
// STACK IMPLEMENTATION
// =============================================================================

#define MAX_STACK_SIZE 100

typedef struct {
    int items[MAX_STACK_SIZE];
    int top;
} Stack;

// Initialize stack
void initStack(Stack *s) {
    s->top = -1;
}

// Check if stack is empty
int isStackEmpty(Stack *s) {
    return s->top == -1;
}

// Check if stack is full
int isStackFull(Stack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

// Push element onto stack
int push(Stack *s, int value) {
    if (isStackFull(s)) {
        printf("Stack overflow! Cannot push %d\n", value);
        return 0;
    }
    
    s->items[++s->top] = value;
    return 1;
}

// Pop element from stack
int pop(Stack *s) {
    if (isStackEmpty(s)) {
        printf("Stack underflow! Cannot pop\n");
        return INT_MIN;
    }
    
    return s->items[s->top--];
}

// Peek at top element
int peek(Stack *s) {
    if (isStackEmpty(s)) {
        printf("Stack is empty! Cannot peek\n");
        return INT_MIN;
    }
    
    return s->items[s->top];
}

// Get stack size
int stackSize(Stack *s) {
    return s->top + 1;
}

// Print stack contents
void printStack(Stack *s) {
    if (isStackEmpty(s)) {
        printf("Stack is empty\n");
        return;
    }
    
    printf("Stack (top to bottom): ");
    for (int i = s->top; i >= 0; i--) {
        printf("%d ", s->items[i]);
    }
    printf("\n");
}

// =============================================================================
// QUEUE IMPLEMENTATION
// =============================================================================

#define MAX_QUEUE_SIZE 100

typedef struct {
    int items[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
} Queue;

// Initialize queue
void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

// Check if queue is empty
int isQueueEmpty(Queue *q) {
    return q->count == 0;
}

// Check if queue is full
int isQueueFull(Queue *q) {
    return q->count == MAX_QUEUE_SIZE;
}

// Enqueue element
int enqueue(Queue *q, int value) {
    if (isQueueFull(q)) {
        printf("Queue overflow! Cannot enqueue %d\n", value);
        return 0;
    }
    
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->items[q->rear] = value;
    q->count++;
    return 1;
}

// Dequeue element
int dequeue(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("Queue underflow! Cannot dequeue\n");
        return INT_MIN;
    }
    
    int value = q->items[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    q->count--;
    return value;
}

// Peek at front element
int peekQueue(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("Queue is empty! Cannot peek\n");
        return INT_MIN;
    }
    
    return q->items[q->front];
}

// Get queue size
int queueSize(Queue *q) {
    return q->count;
}

// Print queue contents
void printQueue(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    
    printf("Queue (front to rear): ");
    for (int i = 0; i < q->count; i++) {
        int index = (q->front + i) % MAX_QUEUE_SIZE;
        printf("%d ", q->items[index]);
    }
    printf("\n");
}

// =============================================================================
// DYNAMIC STACK IMPLEMENTATION
// =============================================================================

typedef struct DynamicStackNode {
    int data;
    struct DynamicStackNode *next;
} DynamicStackNode;

typedef struct {
    DynamicStackNode *top;
    int size;
} DynamicStack;

// Initialize dynamic stack
void initDynamicStack(DynamicStack *s) {
    s->top = NULL;
    s->size = 0;
}

// Push onto dynamic stack
int dynamicPush(DynamicStack *s, int value) {
    DynamicStackNode *newNode = (DynamicStackNode*)malloc(sizeof(DynamicStackNode));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return 0;
    }
    
    newNode->data = value;
    newNode->next = s->top;
    s->top = newNode;
    s->size++;
    return 1;
}

// Pop from dynamic stack
int dynamicPop(DynamicStack *s) {
    if (s->top == NULL) {
        printf("Stack underflow!\n");
        return INT_MIN;
    }
    
    DynamicStackNode *temp = s->top;
    int value = temp->data;
    s->top = temp->next;
    free(temp);
    s->size--;
    return value;
}

// Peek at dynamic stack
int dynamicPeek(DynamicStack *s) {
    if (s->top == NULL) {
        printf("Stack is empty!\n");
        return INT_MIN;
    }
    
    return s->top->data;
}

// Get dynamic stack size
int dynamicStackSize(DynamicStack *s) {
    return s->size;
}

// Free dynamic stack
void freeDynamicStack(DynamicStack *s) {
    while (s->top != NULL) {
        DynamicStackNode *temp = s->top;
        s->top = s->top->next;
        free(temp);
    }
    s->size = 0;
}

// =============================================================================
// DYNAMIC QUEUE IMPLEMENTATION (LINKED LIST)
// =============================================================================

typedef struct QueueNode {
    int data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int size;
} DynamicQueue;

// Initialize dynamic queue
void initDynamicQueue(DynamicQueue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

// Enqueue to dynamic queue
int dynamicEnqueue(DynamicQueue *q, int value) {
    QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return 0;
    }
    
    newNode->data = value;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        // First element
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    
    q->size++;
    return 1;
}

// Dequeue from dynamic queue
int dynamicDequeue(DynamicQueue *q) {
    if (q->front == NULL) {
        printf("Queue underflow!\n");
        return INT_MIN;
    }
    
    QueueNode *temp = q->front;
    int value = temp->data;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return value;
}

// Peek at dynamic queue
int dynamicPeekQueue(DynamicQueue *q) {
    if (q->front == NULL) {
        printf("Queue is empty!\n");
        return INT_MIN;
    }
    
    return q->front->data;
}

// Get dynamic queue size
int dynamicQueueSize(DynamicQueue *q) {
    return q->size;
}

// Free dynamic queue
void freeDynamicQueue(DynamicQueue *q) {
    while (q->front != NULL) {
        QueueNode *temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
    q->size = 0;
}

// =============================================================================
// APPLICATION EXAMPLES
// =============================================================================

// Check if parentheses are balanced
int isBalancedParentheses(const char *expr) {
    Stack s;
    initStack(&s);
    
    for (int i = 0; expr[i]; i++) {
        char ch = expr[i];
        
        if (ch == '(' || ch == '{' || ch == '[') {
            push(&s, ch);
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (isStackEmpty(&s)) {
                return 0; // No matching opening bracket
            }
            
            char top = pop(&s);
            if ((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '[')) {
                return 0; // Mismatched brackets
            }
        }
    }
    
    return isStackEmpty(&s); // Should be empty if balanced
}

// Reverse a string using stack
void reverseString(char *str) {
    Stack s;
    initStack(&s);
    
    // Push all characters onto stack
    for (int i = 0; str[i]; i++) {
        push(&s, str[i]);
    }
    
    // Pop characters to reverse string
    for (int i = 0; str[i]; i++) {
        str[i] = pop(&s);
    }
}

// Generate binary representation of a number
void generateBinary(int number) {
    Stack s;
    initStack(&s);
    
    if (number == 0) {
        printf("0");
        return;
    }
    
    // Push binary digits onto stack
    while (number > 0) {
        push(&s, number % 2);
        number /= 2;
    }
    
    // Pop digits to get binary representation
    while (!isStackEmpty(&s)) {
        printf("%d", pop(&s));
    }
}

// Hot potato game (queue application)
void hotPotatoGame(int numPeople, int passes) {
    Queue q;
    initQueue(&q);
    
    // Enqueue people numbered 1 to numPeople
    for (int i = 1; i <= numPeople; i++) {
        enqueue(&q, i);
    }
    
    printf("Hot potato game with %d people, %d passes:\n", numPeople, passes);
    
    int round = 1;
    while (queueSize(&q) > 1) {
        printf("Round %d: ", round);
        printQueue(&q);
        
        // Pass the potato
        for (int i = 0; i < passes; i++) {
            int person = dequeue(&q);
            enqueue(&q, person);
        }
        
        // Eliminate person
        int eliminated = dequeue(&q);
        printf("Person %d eliminated\n", eliminated);
        
        round++;
    }
    
    printf("Winner: Person %d\n", dequeue(&q));
}

// Print elements in reverse order using stack
void printReverse(Queue *q) {
    Stack s;
    initStack(&s);
    
    // Move all elements from queue to stack
    while (!isQueueEmpty(q)) {
        push(&s, dequeue(q));
    }
    
    // Print elements from stack (reversed order)
    printf("Reversed queue: ");
    while (!isStackEmpty(&s)) {
        printf("%d ", pop(&s));
    }
    printf("\n");
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateStackOperations() {
    printf("=== STACK OPERATIONS ===\n");
    
    Stack s;
    initStack(&s);
    
    printf("Pushing elements: 10, 20, 30, 40, 50\n");
    push(&s, 10);
    push(&s, 20);
    push(&s, 30);
    push(&s, 40);
    push(&s, 50);
    
    printStack(&s);
    printf("Stack size: %d\n", stackSize(&s));
    printf("Top element: %d\n", peek(&s));
    
    printf("\nPopping elements:\n");
    while (!isStackEmpty(&s)) {
        printf("Popped: %d\n", pop(&s));
    }
    
    printf("Final stack size: %d\n", stackSize(&s));
    printf("\n");
}

void demonstrateQueueOperations() {
    printf("=== QUEUE OPERATIONS ===\n");
    
    Queue q;
    initQueue(&q);
    
    printf("Enqueuing elements: 10, 20, 30, 40, 50\n");
    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);
    enqueue(&q, 40);
    enqueue(&q, 50);
    
    printQueue(&q);
    printf("Queue size: %d\n", queueSize(&q));
    printf("Front element: %d\n", peekQueue(&q));
    
    printf("\nDequeuing elements:\n");
    while (!isQueueEmpty(&q)) {
        printf("Dequeued: %d\n", dequeue(&q));
    }
    
    printf("Final queue size: %d\n", queueSize(&q));
    printf("\n");
}

void demonstrateDynamicStack() {
    printf("=== DYNAMIC STACK ===\n");
    
    DynamicStack s;
    initDynamicStack(&s);
    
    printf("Pushing elements onto dynamic stack: ");
    for (int i = 1; i <= 10; i++) {
        dynamicPush(&s, i * 10);
        printf("%d ", i * 10);
    }
    printf("\n");
    
    printf("Dynamic stack size: %d\n", dynamicStackSize(&s));
    printf("Top element: %d\n", dynamicPeek(&s));
    
    printf("\nPopping all elements: ");
    while (s.top != NULL) {
        printf("%d ", dynamicPop(&s));
    }
    printf("\n");
    
    freeDynamicStack(&s);
    printf("\n");
}

void demonstrateDynamicQueue() {
    printf("=== DYNAMIC QUEUE ===\n");
    
    DynamicQueue q;
    initDynamicQueue(&q);
    
    printf("Enqueuing elements into dynamic queue: ");
    for (int i = 1; i <= 10; i++) {
        dynamicEnqueue(&q, i * 10);
        printf("%d ", i * 10);
    }
    printf("\n");
    
    printf("Dynamic queue size: %d\n", dynamicQueueSize(&q));
    printf("Front element: %d\n", dynamicPeekQueue(&q));
    
    printf("\nDequeuing all elements: ");
    while (q.front != NULL) {
        printf("%d ", dynamicDequeue(&q));
    }
    printf("\n");
    
    freeDynamicQueue(&q);
    printf("\n");
}

void demonstrateApplications() {
    printf("=== STACK AND QUEUE APPLICATIONS ===\n");
    
    // Parentheses balancing
    printf("1. Parentheses Balancing:\n");
    const char *expressions[] = {
        "({[]})", "({[})", "((()))", "({[]})"
    };
    
    for (int i = 0; i < 4; i++) {
        printf("'%s' is %s\n", expressions[i], 
               isBalancedParentheses(expressions[i]) ? "balanced" : "not balanced");
    }
    
    // String reversal
    printf("\n2. String Reversal:\n");
    char str[] = "Hello, World!";
    printf("Original: %s\n", str);
    reverseString(str);
    printf("Reversed: %s\n", str);
    
    // Binary conversion
    printf("\n3. Binary Conversion:\n");
    int numbers[] = {10, 25, 42, 255};
    for (int i = 0; i < 4; i++) {
        printf("%d in binary: ", numbers[i]);
        generateBinary(numbers[i]);
        printf("\n");
    }
    
    // Hot potato game
    printf("\n4. Hot Potato Game:\n");
    hotPotatoGame(7, 3);
    
    printf("\n");
}

int main() {
    printf("Stacks and Queues\n");
    printf("==================\n\n");
    
    demonstrateStackOperations();
    demonstrateQueueOperations();
    demonstrateDynamicStack();
    demonstrateDynamicQueue();
    demonstrateApplications();
    
    printf("All stack and queue examples demonstrated!\n");
    return 0;
}
