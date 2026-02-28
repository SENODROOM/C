# C Input/Output (I/O) Statements

I/O in C is handled via the standard library `<stdio.h>`. There are two directions: **output** (sending data to screen/file) and **input** (reading data from keyboard/file).

---

## 1. Output Functions

### `printf()` — Formatted Output
Prints formatted text to the standard output (screen).

```c
printf("format string", arguments);
```

**Format Specifiers:**

| Specifier | Type        | Example              |
|-----------|-------------|----------------------|
| `%d`      | int         | `printf("%d", 10);`  |
| `%f`      | float       | `printf("%f", 3.14);`|
| `%c`      | char        | `printf("%c", 'A');` |
| `%s`      | string      | `printf("%s", "Hi");`|
| `%lf`     | double      | `printf("%lf", d);`  |
| `%o`      | octal       | `printf("%o", 8);`   |
| `%x`      | hexadecimal | `printf("%x", 255);` |

**Example:**
```c
int age = 20;
printf("Age is: %d\n", age);
```

---

### `puts()` — Print String with Newline
Prints a string followed by a newline automatically.

```c
puts("Hello, World!");
// Output: Hello, World!
```

---

### `putchar()` — Print Single Character
Prints one character at a time.

```c
putchar('A');  // Output: A
```

---

## 2. Input Functions

### `scanf()` — Formatted Input
Reads formatted input from the keyboard.

```c
scanf("format string", &variable);
```

> **Note:** Always use `&` (address-of operator) before variable names in `scanf`.

**Example:**
```c
int age;
scanf("%d", &age);
```

---

### `gets()` — Read a String (Unsafe)
Reads a full line of text (including spaces). **Avoid in modern code** — use `fgets()` instead.

```c
char name[50];
gets(name);  // deprecated / unsafe
```

---

### `fgets()` — Safe String Input
Reads a string safely with a size limit.

```c
char name[50];
fgets(name, 50, stdin);
```

---

### `getchar()` — Read Single Character
Reads one character from keyboard input.

```c
char ch = getchar();
```

---

## 3. File I/O

| Function     | Purpose                        |
|--------------|--------------------------------|
| `fopen()`    | Open a file                    |
| `fclose()`   | Close a file                   |
| `fprintf()`  | Write formatted data to file   |
| `fscanf()`   | Read formatted data from file  |
| `fputs()`    | Write string to file           |
| `fgets()`    | Read string from file          |

```c
FILE *fp = fopen("data.txt", "w");
fprintf(fp, "Hello File!\n");
fclose(fp);
```

---

## Summary

| Task               | Function         |
|--------------------|------------------|
| Print text         | `printf()`       |
| Print string       | `puts()`         |
| Print character    | `putchar()`      |
| Read number/data   | `scanf()`        |
| Read string safely | `fgets()`        |
| Read character     | `getchar()`      |
