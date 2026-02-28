# `scanf()` in C — Complete Guide with All Format Specifiers

## What is `scanf()`?

`scanf()` reads **formatted input** from the keyboard (standard input) and stores it into variables.

```c
scanf("format_string", &variable1, &variable2, ...);
```

> **Critical Rule:** Always use the `&` (address-of) operator before variable names — except for arrays/strings (they are already addresses).

---

## How `scanf()` Works Internally

1. Waits for the user to type input and press **Enter**
2. Reads the input and **converts it** according to the format specifier
3. Stores the converted value at the **memory address** of the variable
4. Returns the **number of items successfully read**

```c
int result = scanf("%d", &num);
// result = 1 if successful, 0 if failed, EOF if end of input
```

---

## All Format Specifiers

---

### 1. `%d` — Integer (int)

Reads a **decimal integer** (whole number, positive or negative).

```c
int age;
scanf("%d", &age);
```

| Input | Stored Value |
|-------|-------------|
| `25`  | `25`        |
| `-10` | `-10`       |
| `0`   | `0`         |

> Stops reading at whitespace or non-digit characters.

---

### 2. `%i` — Integer (auto-detect base)

Like `%d` but also accepts **octal** (prefix `0`) and **hexadecimal** (prefix `0x`).

```c
int num;
scanf("%i", &num);
```

| Input  | Base        | Stored Value |
|--------|-------------|-------------|
| `255`  | Decimal     | `255`       |
| `0377` | Octal       | `255`       |
| `0xFF` | Hexadecimal | `255`       |

---

### 3. `%f` — Float

Reads a **single-precision floating-point** number.

```c
float price;
scanf("%f", &price);
```

| Input    | Stored Value |
|----------|-------------|
| `3.14`   | `3.14`      |
| `10`     | `10.0`      |
| `-2.5`   | `-2.5`      |
| `1.5e3`  | `1500.0`    |

---

### 4. `%lf` — Double

Reads a **double-precision floating-point** number. You **must** use `%lf` (not `%f`) for `double` in `scanf`.

```c
double pi;
scanf("%lf", &pi);
```

> **Common Mistake:** Using `%f` for `double` in `scanf` gives wrong results. Always use `%lf` for `double`.

---

### 5. `%Lf` — Long Double

Reads an **extended-precision** floating-point number.

```c
long double val;
scanf("%Lf", &val);
```

---

### 6. `%c` — Character

Reads a **single character**.

```c
char ch;
scanf("%c", &ch);
```

| Input | Stored Value |
|-------|-------------|
| `A`   | `'A'`       |
| `5`   | `'5'` (the digit character, not integer 5) |
| ` `   | `' '` (space) |

> **Watch out:** After `scanf("%d")`, a leftover `\n` in the buffer will be consumed by the next `scanf("%c")`. Use `scanf(" %c", &ch)` — note the space before `%c` — to skip whitespace.

---

### 7. `%s` — String (word)

Reads a **sequence of non-whitespace characters** (a word). Stops at space, tab, or newline.

```c
char name[50];
scanf("%s", name);   // No & needed — array name is already an address
```

| Input         | Stored Value |
|---------------|-------------|
| `Hello`       | `"Hello"`   |
| `Hello World` | `"Hello"` (stops at space!) |

> **Safety tip:** Use width limiter to prevent buffer overflow: `scanf("%49s", name);`

---

### 8. `%[...]` — Scanset (custom character set)

Reads characters that **match a set** you define. Stops when a non-matching character is found.

```c
char str[50];
scanf("%[a-z]", str);         // reads only lowercase letters
scanf("%[A-Za-z ]", str);     // reads letters and spaces
scanf("%[^\n]", str);          // reads everything until newline (full line!)
```

| Pattern       | Reads                            |
|---------------|----------------------------------|
| `%[a-z]`      | lowercase letters only           |
| `%[0-9]`      | digits only                      |
| `%[A-Za-z ]`  | letters and spaces               |
| `%[^\n]`      | everything except newline (full line) |
| `%[^,]`       | everything except comma          |

> `%[^\n]` is the most common way to read a **full line with spaces** using `scanf`.

---

### 9. `%o` — Octal Integer

Reads an integer in **octal (base-8)** format.

```c
int num;
scanf("%o", &num);   // input: 17 → stored as 15 (decimal)
```

| Input | Octal | Decimal Stored |
|-------|-------|----------------|
| `17`  | base-8| `15`           |
| `10`  | base-8| `8`            |

---

### 10. `%x` / `%X` — Hexadecimal Integer

Reads an integer in **hexadecimal (base-16)** format.

```c
int num;
scanf("%x", &num);   // input: ff → stored as 255 (decimal)
```

| Input | Hex   | Decimal Stored |
|-------|-------|----------------|
| `ff`  | base-16 | `255`        |
| `1A`  | base-16 | `26`         |

---

### 11. `%u` — Unsigned Integer

Reads an **unsigned (non-negative)** integer.

```c
unsigned int count;
scanf("%u", &count);
```

---

### 12. `%ld` — Long Integer

Reads a **long int**.

```c
long int bigNum;
scanf("%ld", &bigNum);
```

---

### 13. `%lld` — Long Long Integer

Reads a **long long int** (very large integers).

```c
long long int veryBig;
scanf("%lld", &veryBig);
```

---

### 14. `%lu` — Unsigned Long

```c
unsigned long ul;
scanf("%lu", &ul);
```

---

### 15. `%p` — Pointer Address

Reads a **memory address** in hexadecimal and stores it as a pointer.

```c
void *ptr;
scanf("%p", &ptr);
```

---

### 16. `%n` — Character Count (no input consumed)

Stores the **number of characters read so far** into an integer variable. Does NOT consume any input.

```c
int num, count;
scanf("%d%n", &num, &count);
printf("Read %d characters so far\n", count);
```

---

## Width Specifier

You can limit how many characters `scanf` reads:

```c
scanf("%5d", &num);     // reads at most 5 digit characters
scanf("%10s", name);    // reads at most 10 characters (safer)
scanf("%49s", name);    // safe for char name[50]
```

---

## Reading Multiple Values

```c
int a, b;
scanf("%d %d", &a, &b);    // reads two integers separated by space/enter

float x, y;
scanf("%f %f", &x, &y);

char first[20], last[20];
scanf("%s %s", first, last);   // reads two words
```

---

## `scanf` Return Value

```c
int n = scanf("%d %f", &num, &fnum);
// n = 2 if both read successfully
// n = 1 if only first was read
// n = 0 if nothing matched
// n = EOF (-1) if end of input
```

---

## Common Pitfalls

| Problem | Cause | Fix |
|---------|-------|-----|
| `%c` reads a newline | Leftover `\n` from previous input | Use `scanf(" %c", &ch)` with a leading space |
| `%s` stops at space | By design | Use `%[^\n]` to read full line |
| Wrong value for `double` | Using `%f` instead of `%lf` | Always use `%lf` for `double` in `scanf` |
| Buffer overflow | No size limit on `%s` | Use `scanf("%49s", name)` |
| Input stuck in buffer | Failed `scanf` leaves input | Clear with `while(getchar()!='\n');` |

---

## Quick Reference Table

| Specifier | Type              | Example Variable    |
|-----------|-------------------|---------------------|
| `%d`      | int               | `int n;`            |
| `%i`      | int (any base)    | `int n;`            |
| `%f`      | float             | `float f;`          |
| `%lf`     | double            | `double d;`         |
| `%Lf`     | long double       | `long double ld;`   |
| `%c`      | char              | `char c;`           |
| `%s`      | string (word)     | `char s[50];`       |
| `%[...]`  | string (custom set) | `char s[50];`     |
| `%o`      | octal int         | `int n;`            |
| `%x`/`%X` | hex int          | `int n;`            |
| `%u`      | unsigned int      | `unsigned int n;`   |
| `%ld`     | long int          | `long n;`           |
| `%lld`    | long long int     | `long long n;`      |
| `%lu`     | unsigned long     | `unsigned long n;`  |
| `%p`      | pointer           | `void *p;`          |
| `%n`      | chars read so far | `int n;`            |
