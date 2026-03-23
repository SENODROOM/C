# Expense Tracker Project

A program to track daily expenses, categorize them, and generate reports.

## Features:

- Add expenses with description and category
- View all expenses
- Calculate total by category
- Sort expenses by amount or date
- Save expenses to a file
- Load expenses from a file

## Data Structure:

```c
struct Expense {
    char date[11];        // DD/MM/YYYY
    char category[50];    // Food, Transport, Entertainment, etc.
    char description[100];
    float amount;
};
```

## Operations:

1. **Add Expense**: Record a new expense
2. **View All**: Display all stored expenses
3. **View by Category**: Filter expenses by category
4. **Statistics**: Calculate totals and averages
5. **Save/Load**: Persist data to file

## Learning Concepts:

- Arrays and structures
- File input/output (fopen, fread, fwrite, fclose)
- Dynamic memory allocation
- String handling
- Sorting algorithms
- Data organization and retrieval

## Compilation:

```bash
gcc -o expense_tracker Expense_Tracker.c
./expense_tracker
```

See Expense_Tracker.c for the implementation.
