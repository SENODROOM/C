# Linked Lists

A linked list is a linear data structure where elements are stored as nodes, each containing data and a pointer to the next node.

## Key Characteristics:

- **Dynamic Memory**: Memory allocated at runtime
- **Non-contiguous Storage**: Nodes can be scattered in memory
- **Sequential Access**: Must traverse from head to find an element
- **Flexible Size**: Can grow or shrink easily

## Advantages:

- Dynamic memory allocation
- Efficient insertion/deletion at known positions
- Memory space is allocated only when needed

## Disadvantages:

- No direct access by index
- Extra memory for pointers
- Slower search compared to arrays
- Cache unfriendly

## Common Operations:

- **Insert**: Add a node (beginning, end, or middle)
- **Delete**: Remove a node
- **Search**: Find a node with specific data
- **Traverse**: Visit all nodes
- **Reverse**: Reverse the list order

## Types of Linked Lists:

1. **Singly Linked List**: Each node points to the next node only
2. **Doubly Linked List**: Each node points to both next and previous
3. **Circular Linked List**: Last node points back to first node

## Time Complexity:

| Operation | Time |
|-----------|------|
| Search | O(n) |
| Insert (middle) | O(n) |
| Delete (middle) | O(n) |
| Insert at beginning | O(1) |
| Delete from beginning | O(1) |

## Real-world Applications:

- Browser navigation (forward/back buttons)
- Undo/Redo functionality
- Music playlist
- Assistant system memory

See Linked_Lists.c for a complete implementation.
