# C Programming Tutorial

A complete beginner-to-intermediate guide covering the four core concepts of C programming, each explained in a markdown file with a corresponding working `.c` source file.

---

## 📁 File Structure

```
C_Tutorial/
│
├── README.md                        ← This file
│
├── 01_IO_Statements.md              ← Theory: Input/Output
├── 01_io_statements.c               ← Code: I/O examples
│
├── 02_Conditional_Statements.md     ← Theory: Conditionals
├── 02_conditional_statements.c      ← Code: if, switch, ternary
│
├── 03_Repetitive_Statements.md      ← Theory: Loops
├── 03_repetitive_statements.c       ← Code: for, while, do-while
│
└── 04_Functions.md                  ← Theory: Functions
    04_functions.c                   ← Code: all function types
```

---

## 📘 Topics Covered

### 1. I/O Statements (`01_*`)
- `printf()` with format specifiers (`%d`, `%f`, `%s`, `%c`, etc.)
- `puts()` and `putchar()` for output
- `scanf()` for reading input
- `fgets()` for safe string input
- `getchar()` for character input
- File I/O: `fopen`, `fprintf`, `fgets`, `fclose`

### 2. Conditional Statements (`02_*`)
- `if` — single condition
- `if-else` — two-way decision
- `if-else if-else` — multi-way ladder
- Nested `if`
- `switch-case` — value-based branching
- Ternary operator `? :`
- Logical operators: `&&`, `||`, `!`

### 3. Repetitive Statements (`03_*`)
- `for` loop — count-controlled
- `while` loop — condition-controlled
- `do-while` loop — runs at least once
- Nested loops
- `break` — exit loop early
- `continue` — skip an iteration
- `goto` — jump to label

### 4. Functions (`04_*`)
- `void` function — no params, no return
- `void` function — with parameters
- Function returning a value
- Function with params + return value
- Function prototypes/declarations
- Recursive functions (factorial, fibonacci)
- Call by value vs call by reference
- Returning multiple values via pointers
- Static variables
- Standard library functions (`math.h`, `string.h`, etc.)

---

## ⚙️ How to Compile and Run

Make sure you have `gcc` installed.

```bash
# Compile
gcc 01_io_statements.c -o io_demo
gcc 02_conditional_statements.c -o cond_demo
gcc 03_repetitive_statements.c -o loop_demo
gcc 04_functions.c -o func_demo -lm    # -lm links math library

# Run
./io_demo
./cond_demo
./loop_demo
./func_demo
```

> On Windows, replace `./` with the filename (e.g., `io_demo.exe`)

---

## 💡 Learning Tips

1. Read the `.md` file first to understand the concept
2. Then study the `.c` file to see it in action
3. Try modifying values and re-running to observe changes
4. Comment out sections you want to isolate and test
