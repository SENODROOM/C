# C Repetitive (Loop) Statements

Loops allow a block of code to be executed **repeatedly** until a condition is met or becomes false.

---

## 1. `for` Loop

Best used when the **number of iterations is known** in advance.

```c
for (initialization; condition; update) {
    // body
}
```

**Flow:**
1. Initialization runs once
2. Condition is checked
3. If true → body executes → update runs → back to step 2
4. If false → loop ends

**Example:**
```c
for (int i = 1; i <= 5; i++) {
    printf("%d ", i);
}
// Output: 1 2 3 4 5
```

**Nested for loop:**
```c
for (int i = 1; i <= 3; i++) {
    for (int j = 1; j <= 3; j++) {
        printf("%d ", i * j);
    }
    printf("\n");
}
```

---

## 2. `while` Loop

Best used when the **number of iterations is not known** — loop runs as long as a condition is true.

```c
while (condition) {
    // body
}
```

> Condition is checked **before** each iteration (entry-controlled loop).

**Example:**
```c
int i = 1;
while (i <= 5) {
    printf("%d ", i);
    i++;
}
// Output: 1 2 3 4 5
```

**Infinite while loop:**
```c
while (1) {
    // runs forever unless break is used
}
```

---

## 3. `do-while` Loop

Similar to `while`, but the body executes **at least once** — condition is checked **after** the first iteration.

```c
do {
    // body (executes at least once)
} while (condition);
```

**Example:**
```c
int i = 1;
do {
    printf("%d ", i);
    i++;
} while (i <= 5);
// Output: 1 2 3 4 5
```

**Key difference from `while`:**
```c
// Even if condition is false from the start:
int x = 100;
do {
    printf("Runs once!\n");  // This WILL print
} while (x < 10);           // false, but body ran once
```

---

## 4. Loop Control Statements

### `break` — Exit the loop immediately
```c
for (int i = 0; i < 10; i++) {
    if (i == 5) break;
    printf("%d ", i);
}
// Output: 0 1 2 3 4
```

### `continue` — Skip current iteration, move to next
```c
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) continue;  // skip even numbers
    printf("%d ", i);
}
// Output: 1 3 5 7 9
```

### `goto` — Jump to a label (use sparingly)
```c
int i = 0;
start:
    printf("%d ", i);
    i++;
    if (i < 5) goto start;
// Output: 0 1 2 3 4
```

---

## Comparison: `for` vs `while` vs `do-while`

| Feature             | `for`       | `while`     | `do-while`        |
|---------------------|-------------|-------------|-------------------|
| Condition check     | Before body | Before body | After body        |
| Min. iterations     | 0           | 0           | 1 (always)        |
| Best used when      | Known count | Unknown count | Must run once   |
| Init & update       | In header   | Separate    | Separate          |

---

## Common Loop Patterns

```c
// Sum of 1 to N
int sum = 0;
for (int i = 1; i <= N; i++) sum += i;

// Print array
for (int i = 0; i < n; i++) printf("%d ", arr[i]);

// Reverse loop
for (int i = n - 1; i >= 0; i--) printf("%d ", arr[i]);

// Factorial
int fact = 1;
for (int i = 1; i <= n; i++) fact *= i;
```
