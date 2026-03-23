# C Learning Resources

All C programming concepts organized with paired .c (code) and .md (documentation) files in the same folders, matching the C++ structure.

## 🎯 Structure Concept

Each folder represents a major topic area. Within each folder:
- **`.c` files**: Runnable C code implementations
- **`.md` files**: Documentation and detailed explanations

**Example**: 
- `data_structures/Linked_Lists.c` - Working code
- `data_structures/Linked_Lists.md` - Explanation and concepts

## 📂 Main Categories

### 🎓 core_concepts/
**Fundamentals of C Programming**

Topics covered:
- Input/Output and console programming
- Variables, data types, and operators
- Control flow (conditionals and loops)
- Functions and program organization
- Arrays and strings
- File handling and streams

**Start here if you're new to C**

### 🏗️ data_structures/
**Essential Data Structures**

Implementations include:
- Linked lists (singly, insertion, deletion)
- Stacks and queues
- Arrays and strings
- Structures and unions

**Paired files**: Code + detailed documentation together

### 📖 algorithms/
**Algorithm Implementations**

Covers:
- Sorting algorithms (bubble, quick, merge sort)
- Searching algorithms (linear, binary search)
- Mathematical algorithms
- Optimization techniques

### ⚡ advanced_topics/
**Advanced C Concepts**

Topics include:
- Pointers and memory management
- Dynamic memory allocation
- Recursion and memoization
- Function pointers and callbacks
- Preprocessor directives and macros
- Multi-file projects

### 🚀 projects/
**Complete Applications**

Working projects with documentation:
- **Calculator** - Basic arithmetic operations
- **Expense Tracker** - Data management and file I/O
- **Student Gradebook** - Structures and calculations
- ...and more

Each project folder contains both `.c` (code) and `.md` (documentation)

## 🔗 How to Use This Structure

1. **Pick a Concept**: Choose a folder that interests you
2. **Read the Documentation**: Start with the `.md` file
3. **Study the Code**: Review the corresponding `.c` file
4. **Compile and Run**: Execute the code to see it in action
5. **Experiment**: Modify the code and test your understanding

## Example Learning Path

```
Start: core_concepts/
  ↓
Learn: Variables, I/O, control flow
  ↓
Progress: data_structures/
  ↓
Apply: projects/
  ↓
Advanced: advanced_topics/
```

## 📊 File Organization

| Category | Purpose | Files | Examples |
|----------|---------|-------|----------|
| core_concepts/ | Learn C basics | `.c` + `.md` pairs | Input_Output_and_Control_Flow |
| data_structures/ | Understand data structures | `.c` + `.md` pairs | Linked_Lists, Stacks_and_Queues |
| algorithms/ | Study algorithms | Code implementations | Sorting algorithms |
| advanced_topics/ | Master advanced concepts | Code + explanations | Pointers_and_Memory_Management |
| projects/ | See real applications | Complete projects | Calculator, Expense_Tracker |

## 🎯 Learning Tips

- **Read first**: Get understanding from `.md` files
- **Study code**: See implementation in `.c` files
- **Experiment**: Modify and compile the code
- **Compare**: See how different concepts work together in projects
- **Reference**: Return to files when you need quick lookup

## 🔧 Compilation Notes

```bash
# Basic compilation
gcc -o output filename.c

# With warnings
gcc -Wall -Wextra -o output filename.c

# With debugging symbols
gcc -g -o output filename.c

# Multiple files
gcc -o output file1.c file2.c file3.c
```

---

**Start with core_concepts/ and progress through the topics at your own pace!**
Each paired file set is designed to teach and reinforce C concepts effectively.
