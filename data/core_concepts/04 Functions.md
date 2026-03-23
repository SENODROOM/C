# C Functions

A **function** is a named, reusable block of code that performs a specific task. Functions help organize code, reduce repetition, and improve readability.

---

## Anatomy of a Function

```c
return_type function_name(parameter_list) {
    // body
    return value;  // if return_type is not void
}
```

| Part              | Description                                  |
|-------------------|----------------------------------------------|
| `return_type`     | Data type of value returned (or `void`)      |
| `function_name`   | Identifier for calling the function          |
| `parameter_list`  | Input variables (can be empty)               |
| `body`            | Code that runs when function is called       |
| `return`          | Sends a value back to the caller             |

---

## 1. Function with No Parameters, No Return Value (`void`)

Does a task and returns nothing.

```c
void greet() {
    printf("Hello, World!\n");
}

int main() {
    greet();  // function call
    return 0;
}
```

---

## 2. Function with Parameters, No Return Value

Accepts input but doesn't return a value.

```c
void printSquare(int n) {
    printf("Square of %d = %d\n", n, n * n);
}

int main() {
    printSquare(5);
    printSquare(9);
    return 0;
}
```

---

## 3. Function with No Parameters, With Return Value

Returns a value but doesn't take any input.

```c
int getYear() {
    return 2025;
}

int main() {
    int year = getYear();
    printf("Year: %d\n", year);
    return 0;
}
```

---

## 4. Function with Parameters and Return Value

The most common form — takes input, processes it, returns a result.

```c
int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(10, 20);
    printf("Sum = %d\n", result);
    return 0;
}
```

---

## 5. Function Declaration (Prototype)

If a function is defined **after** `main()`, you must declare it first.

```c
// Prototype / Declaration
int multiply(int, int);

int main() {
    printf("%d\n", multiply(4, 5));
    return 0;
}

// Definition (after main)
int multiply(int a, int b) {
    return a * b;
}
```

---

## 6. Recursive Functions

A function that **calls itself** to solve smaller sub-problems.

```c
int factorial(int n) {
    if (n == 0 || n == 1)
        return 1;               // base case
    return n * factorial(n - 1); // recursive call
}
```

**How `factorial(4)` works:**
```
factorial(4)
  → 4 * factorial(3)
        → 3 * factorial(2)
              → 2 * factorial(1)
                    → 1 (base case)
```
Result: `4 * 3 * 2 * 1 = 24`

---

## 7. Call by Value vs Call by Reference

### Call by Value (default in C)
A **copy** of the argument is passed — original is not modified.

```c
void doubleIt(int x) {
    x = x * 2;  // modifies only local copy
}
```

### Call by Reference (using pointers)
The **address** of the variable is passed — original IS modified.

```c
void doubleIt(int *x) {
    *x = (*x) * 2;  // modifies original
}

int main() {
    int n = 5;
    doubleIt(&n);     // pass address
    printf("%d\n", n);  // Output: 10
}
```

---

## 8. Returning Multiple Values (using pointers)

C functions can only return one value directly — use pointers for multiple outputs.

```c
void minMax(int arr[], int n, int *min, int *max) {
    *min = arr[0];
    *max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}
```

---

## 9. Scope of Variables

| Scope   | Declared    | Visible                          |
|---------|-------------|----------------------------------|
| Local   | Inside `{}` | Only within that block/function  |
| Global  | Outside all | Everywhere in the file           |
| Static  | With `static` | Persists between function calls |

```c
int globalVar = 100;  // global

void demo() {
    int localVar = 10;     // local, only in demo()
    static int count = 0;  // retains value between calls
    count++;
    printf("Called %d times\n", count);
}
```

---

## Summary: Types of Functions

| Type                        | Parameters | Return Value |
|-----------------------------|------------|--------------|
| Void, no params             | No         | No           |
| Void, with params           | Yes        | No           |
| Return value, no params     | No         | Yes          |
| Return value, with params   | Yes        | Yes          |
| Recursive                   | Yes        | Yes          |

---

## Standard Library Functions

C provides many built-in functions via headers:

| Header        | Functions                                   |
|---------------|---------------------------------------------|
| `<stdio.h>`   | `printf`, `scanf`, `fopen`, `fgets`         |
| `<math.h>`    | `sqrt`, `pow`, `abs`, `ceil`, `floor`       |
| `<string.h>`  | `strlen`, `strcpy`, `strcmp`, `strcat`      |
| `<stdlib.h>`  | `malloc`, `free`, `atoi`, `rand`, `exit`    |
| `<ctype.h>`   | `isalpha`, `isdigit`, `toupper`, `tolower`  |
