# Student Gradebook Project

A program to manage student records, calculate grades, and generate reports.

## Features:

- Add student records with marks
- Calculate GPA and letter grades
- View grades by subject or student
- Sort students by GPA
- Save/load records from file
- Generate statistics (class average, highest/lowest scores)

## Data Structure:

```c
struct Student {
    int rollNumber;
    char name[50];
    float marks[5];  // 5 subjects
    float gpa;
    char grade;
};
```

## Operations:

1. **Add Student**: Enter student details
2. **View Records**: Display all student information
3. **Calculate Grades**: Compute GPA and letter grades
4. **Statistics**: Class average and extremes
5. **Search**: Find student by roll number or name

## Grading Scale:

- A: 90-100
- B: 80-89
- C: 70-79
- D: 60-69
- F: Below 60

## Learning Concepts:

- Structures and arrays of structures
- File handling (persistence)
- Sorting (bubble sort)
- Mathematical calculations (average, GPA)
- String operations
- Conditional logic

## Compilation:

```bash
gcc -o gradebook Student_Gradebook.c
./gradebook
```

See Student_Gradebook.c for the implementation.
