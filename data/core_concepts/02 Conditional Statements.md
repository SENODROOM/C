# C Conditional Statements

Conditional statements allow a program to make decisions and execute different code blocks based on conditions.

---

## 1. `if` Statement

Executes a block **only if** the condition is `true`.

```c
if (condition) {
    // code runs if condition is true
}
```

**Example:**
```c
int x = 10;
if (x > 5) {
    printf("x is greater than 5\n");
}
```

---

## 2. `if-else` Statement

Provides an **alternative block** when the condition is `false`.

```c
if (condition) {
    // runs if true
} else {
    // runs if false
}
```

**Example:**
```c
int age = 16;
if (age >= 18) {
    printf("Adult\n");
} else {
    printf("Minor\n");
}
```

---

## 3. `if-else if-else` Ladder

Tests **multiple conditions** one by one.

```c
if (condition1) {
    // ...
} else if (condition2) {
    // ...
} else {
    // default
}
```

**Example:**
```c
int marks = 75;
if (marks >= 90)       printf("Grade: A\n");
else if (marks >= 75)  printf("Grade: B\n");
else if (marks >= 60)  printf("Grade: C\n");
else                   printf("Grade: F\n");
```

---

## 4. Nested `if`

An `if` statement inside another `if`.

```c
if (outer_condition) {
    if (inner_condition) {
        // both conditions true
    }
}
```

**Example:**
```c
int a = 10, b = 20;
if (a > 0) {
    if (b > 0) {
        printf("Both are positive\n");
    }
}
```

---

## 5. `switch` Statement

Tests a variable against a list of **exact values** (cases). Best used when comparing one variable to many constant values.

```c
switch (expression) {
    case value1:
        // code
        break;
    case value2:
        // code
        break;
    default:
        // code if no case matches
}
```

> **`break`** is crucial — without it, execution falls through to the next case.

**Example:**
```c
int day = 3;
switch (day) {
    case 1: printf("Monday\n");    break;
    case 2: printf("Tuesday\n");   break;
    case 3: printf("Wednesday\n"); break;
    default: printf("Other day\n");
}
```

---

## 6. Ternary Operator `? :`

A shorthand for simple `if-else`.

```c
result = (condition) ? value_if_true : value_if_false;
```

**Example:**
```c
int num = 7;
char *type = (num % 2 == 0) ? "Even" : "Odd";
printf("%s\n", type);  // Output: Odd
```

---

## Comparison Operators

| Operator | Meaning               |
|----------|-----------------------|
| `==`     | Equal to              |
| `!=`     | Not equal to          |
| `>`      | Greater than          |
| `<`      | Less than             |
| `>=`     | Greater than or equal |
| `<=`     | Less than or equal    |

## Logical Operators

| Operator | Meaning     | Example             |
|----------|-------------|---------------------|
| `&&`     | AND         | `a > 0 && b > 0`    |
| `\|\|`   | OR          | `a > 0 \|\| b > 0`  |
| `!`      | NOT         | `!(a == b)`         |

---

## Summary

| Statement        | Use Case                              |
|-----------------|---------------------------------------|
| `if`            | Single condition check                |
| `if-else`       | Two-way decision                      |
| `if-else if`    | Multiple conditions, ordered          |
| `switch`        | One variable vs many constant values  |
| Ternary `? :`   | Simple one-line condition             |
