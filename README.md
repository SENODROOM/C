# C Development Hub

A comprehensive, well-organized C programming learning environment with paired code (.c) and documentation (.md) files organized by topic, matching the C++ structure for consistency.

## 📁 Project Structure

```
C/
├── 📄 README.md                 # This file - Main project overview
├── 📁 data/                     # All C content (code & docs paired together)
│   ├── 📄 README.md             # Data folder guide
│   ├── 📖 core_concepts/        # Fundamentals (I/O, variables, control flow, functions, arrays, strings)
│   │   ├── Input_Output_and_Control_Flow.c (paired with .md)
│   │   ├── Variables_and_Data_Types.c (with tutorials)
│   │   ├── Functions_and_Program_Organization.c
│   │   ├── File_Handling_and_Streams.c
│   │   └── ... more paired .c and .md files
│   ├── 🏗️ data_structures/      # Data structures with code & documentation
│   │   ├── Linked_Lists.c
│   │   ├── Linked_Lists.md
│   │   ├── Stack_Queue_Arrays.c
│   │   ├── Stacks_and_Queues.md
│   │   └── README.md
│   ├── 📖 algorithms/           # Algorithm implementations
│   │   └── ... (sorting, searching, etc.)
│   ├── 🚀 advanced_topics/      # Pointers, memory, recursion, macros
│   │   ├── Pointers_and_Memory_Management.c
│   │   ├── Recursion_and_Memoization.c
│   │   ├── Function_Pointers_and_Macros.c
│   │   └── ... more paired files
│   └── 🚀 projects/             # Complete application projects (paired .c and .md)
│       ├── Calculator.c
│       ├── Calculator.md
│       ├── Expense_Tracker.c
│       ├── Expense_Tracker.md
│       ├── Student_Gradebook.c
│       ├── Student_Gradebook.md
│       └── README.md
├── 📁 website/                  # Modern web documentation interface
└── 📁 .git/                    # Version control
```

## 🎯 Structure Overview

**Key Feature**: Each concept has paired files:
- **`.c` file**: Working C code implementation
- **`.md` file**: Detailed documentation and explanation

Both files are in the same folder for easy reference and learning.

### Main Folders:

- **core_concepts/** - Fundamentals: variables, I/O, control flow, functions, arrays, strings, file handling
- **data_structures/** - Implementations: linked lists, stacks, queues, structures
- **algorithms/** - Algorithms: sorting, searching, mathematical operations
- **advanced_topics/** - Advanced: pointers, memory management, recursion, macros, function pointers
- **projects/** - Complete applications: calculator, expense tracker, gradebook, and more

## 🚀 Quick Start

1. **Explore**: Browse the `data/` folder
2. **Read**: Start with the `.md` files for explanations
3. **Code**: Check the corresponding `.c` files for implementation
4. **Compile**: Use gcc to compile and run examples

### Compilation Example:

```bash
cd C/data/<folder_name>
gcc -Wall -Wextra -o output_name filename.c
./output_name
```

## 📚 What's Included

### Core C Programming

**Fundamentals**
- Input/Output statements and formatting
- Variables and data types
- Conditional statements (if, switch)
- Loops and repetitive statements
- Functions and modular programming
- Arrays and strings

**Intermediate**
- Pointers and memory management
- Dynamic memory allocation
- File processing and I/O
- Data structures (stacks, queues, linked lists)
- Sorting and searching algorithms

**Advanced Topics**
- Macros and preprocessor directives
- Function pointers and callbacks
- Multi-file projects
- Bitwise operations
- Command-line interfaces

### Projects & Exercises

**Mini Projects**
- Calculator with basic operations
- Expense tracker application
- Student gradebook system
- Text analyzer utility

**Practice Drills**
- Basics drills for fundamentals
- Control flow practice exercises
- Pointers and memory exercises

### Documentation

- 📖 **Learning Curriculum**: Structured pathway through C concepts
- 📚 **Tutorials**: Step-by-step guides for each topic
- 📖 **Reference**: Cheat sheets, compile commands, glossary
- 💡 **Concepts**: In-depth explanations of programming ideas
- 🚀 **Getting Started**: Environment setup and first programs

## 🔧 Compilation

### Basic Compilation

```bash
gcc -o output_name source.c
./output_name
```

### With Warnings and Debugging

```bash
gcc -Wall -Wextra -std=c99 -o output_name source.c
gcc -g -o output_name source.c  # For debugging
```

### Multiple Files

```bash
gcc -o output_name file1.c file2.c file3.c
```

See `docs/compile_cheat_sheet.md` in documentation/reference/ for more compilation options.

## 📖 Learning Path

1. **Start Here**: Getting started with C basics
2. **Fundamentals**: Input/output, variables, control flow
3. **Functions**: Modular programming and code organization
4. **Arrays & Strings**: Working with collections and text
5. **Pointers**: Memory management and advanced techniques
6. **Data Structures**: Implementing core data structures
7. **Algorithms**: Sorting, searching, and problem solving
8. **Projects**: Apply knowledge to complete projects
9. **Advanced**: Explore advanced C features

## 🏗️ Project Organization

All projects follow a consistent structure:
- `.c` files for C source code
- `.md` files for documentation and explanations
- `README.md` in each category describing its contents
- Examples and drills organized by topic

## 🎯 Use Cases

- **Students**: Follow the structured learning curriculum
- **Educators**: Use as teaching material and example source
- **Developers**: Reference for C concepts and implementations
- **Learners**: Browse through projects and exercises at your own pace

## 📋 Topics Covered

- ✅ Input/Output and Console Programming
- ✅ Variables, Data Types, and Operators
- ✅ Control Flow (Conditionals and Loops)
- ✅ Functions and Modular Programming
- ✅ Arrays, Strings, and Character Operations
- ✅ Pointers and Memory Management
- ✅ File Processing and Stream I/O
- ✅ Data Structures (Stack, Queue, Linked List)
- ✅ Sorting and Searching Algorithms
- ✅ Bitwise Operations
- ✅ Advanced Topics (Macros, Function Pointers)
- ✅ Complete Mini Projects

## 🤝 Contributing

To add new content:
1. Follow the existing folder structure
2. Include both `.c` code files and `.md` documentation
3. Add comprehensive comments in code
4. Update relevant README.md files

## 📝 License

This learning hub is provided for educational purposes. See individual folders for licensing details.

---

**Happy Learning!** Start with `data/documentation/getting_started/` for your first steps into C programming.
