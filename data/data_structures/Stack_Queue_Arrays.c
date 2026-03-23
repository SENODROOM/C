/* Stack and Queue implementations using arrays */
#include <stdio.h>
#include <limits.h>

#define MAX_SIZE 100

/* Stack implementation */
struct Stack {
    int items[MAX_SIZE];
    int top;
};

struct Stack* createStack() {
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    stack->top = -1;
    return stack;
}

int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

int isFull(struct Stack* stack) {
    return stack->top == MAX_SIZE - 1;
}

void push(struct Stack* stack, int value) {
    if (isFull(stack)) {
        printf("Stack Overflow!\n");
        return;
    }
    stack->items[++stack->top] = value;
    printf("Pushed %d\n", value);
}

int pop(struct Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack Underflow!\n");
        return INT_MIN;
    }
    return stack->items[stack->top--];
}

int peek(struct Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty!\n");
        return INT_MIN;
    }
    return stack->items[stack->top];
}

void displayStack(struct Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty!\n");
        return;
    }
    printf("Stack (top to bottom): ");
    for (int i = stack->top; i >= 0; i--) {
        printf("%d ", stack->items[i]);
    }
    printf("\n");
}

/* Queue implementation */
struct Queue {
    int items[MAX_SIZE];
    int front, rear;
};

struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

int isQueueEmpty(struct Queue* queue) {
    return queue->front == -1;
}

int isQueueFull(struct Queue* queue) {
    return queue->rear == MAX_SIZE - 1;
}

void enqueue(struct Queue* queue, int value) {
    if (isQueueFull(queue)) {
        printf("Queue Overflow!\n");
        return;
    }
    if (queue->front == -1) queue->front = 0;
    queue->items[++queue->rear] = value;
    printf("Enqueued %d\n", value);
}

int dequeue(struct Queue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Queue Underflow!\n");
        return INT_MIN;
    }
    int value = queue->items[queue->front];
    queue->front++;
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1;
    }
    return value;
}

/* Main program */
int main() {
    printf("=== Stack and Queue Demo ===\n\n");
    
    struct Stack* stack = createStack();
    printf("Stack Operations:\n");
    push(stack, 10);
    push(stack, 20);
    push(stack, 30);
    displayStack(stack);
    printf("Popped: %d\n", pop(stack));
    displayStack(stack);
    
    printf("\nQueue Operations:\n");
    struct Queue* queue = createQueue();
    enqueue(queue, 5);
    enqueue(queue, 15);
    enqueue(queue, 25);
    printf("Dequeued: %d\n", dequeue(queue));
    printf("Dequeued: %d\n", dequeue(queue));
    
    return 0;
}
