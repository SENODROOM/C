# Stacks and Queues Using Arrays

Stacks and Queues are fundamental data structures in C programming.

## Stack (LIFO - Last In First Out)

A stack follows the LIFO principle where the last element added is the first one to be removed.

### Key Operations:
- **Push**: Add element to the top
- **Pop**: Remove element from the top
- **Peek**: View the top element without removing it

### Common Uses:
- Function call stack in memory
- Undo/Redo functionality
- Expression evaluation
- Backtracking algorithms

## Queue (FIFO - First In First Out)

Aqueu follows the FIFO principle where the first element added is the first one to be removed.

### Key Operations:
- **Enqueue**: Add element at the rear
- **Dequeue**: Remove element from the front
- **Front**: View the front element

### Common Uses:
- Print job scheduling
- CPU task scheduling
- Breadth-first search (BFS)
- Message queues

## Implementation Notes

- **Array-based**: Fixed size, simple implementation
- **Dynamic**: Can use linked lists for flexible size
- **Time Complexity**: Push/Pop: O(1), Dequeue (array): O(n)

## Example

See the accompanying Stack_Queue_Arrays.c file for a complete working example with both stack and queue implementations.
