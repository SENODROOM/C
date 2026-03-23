// ============================================================
// FILE: functions.c
// TOPIC: Functions in C — All Types with Examples
// ============================================================

#include <stdio.h>
#include <math.h>

// ============================================================
// FUNCTION PROTOTYPES (declarations before main)
// ============================================================
void greet();                              // type 1: no param, no return
void printSquare(int n);                   // type 2: param, no return
int getYear();                             // type 3: no param, with return
int add(int a, int b);                     // type 4: param, with return
int multiply(int a, int b);               // type 4 variant
float average(float a, float b);          // type 4 with float
int factorial(int n);                      // recursive
int fibonacci(int n);                      // recursive
void swap_value(int a, int b);            // call by value
void swap_reference(int *a, int *b);      // call by reference
void minMax(int arr[], int n, int *min, int *max); // multiple return via ptr
void counterDemo();                        // static variable demo
int isEven(int n);                         // utility
int isPrime(int n);                        // utility


// ============================================================
// MAIN FUNCTION
// ============================================================
int main() {

    // --------------------------------------------------------
    // 1. void function — no parameters, no return
    // --------------------------------------------------------
    printf("=== 1. void, no params ===\n");
    greet();

    // --------------------------------------------------------
    // 2. void function — with parameters
    // --------------------------------------------------------
    printf("\n=== 2. void, with params ===\n");
    printSquare(4);
    printSquare(7);

    // --------------------------------------------------------
    // 3. Function returning value — no params
    // --------------------------------------------------------
    printf("\n=== 3. return value, no params ===\n");
    int year = getYear();
    printf("Current Year: %d\n", year);

    // --------------------------------------------------------
    // 4. Function with parameters and return value
    // --------------------------------------------------------
    printf("\n=== 4. return value, with params ===\n");
    printf("add(10, 20)       = %d\n", add(10, 20));
    printf("multiply(6, 7)    = %d\n", multiply(6, 7));
    printf("average(15, 25)   = %.2f\n", average(15.0, 25.0));

    // --------------------------------------------------------
    // 5. Function Prototype Demo
    // --------------------------------------------------------
    // multiply() was defined AFTER main but declared at top
    printf("\n=== 5. Prototype (multiply defined after main) ===\n");
    printf("3 x 9 = %d\n", multiply(3, 9));

    // --------------------------------------------------------
    // 6. Recursive Functions
    // --------------------------------------------------------
    printf("\n=== 6. Recursive Functions ===\n");
    printf("Factorial of 5 = %d\n", factorial(5));
    printf("Factorial of 6 = %d\n", factorial(6));

    printf("Fibonacci sequence (first 8 terms):\n");
    for (int i = 0; i < 8; i++) {
        printf("F(%d) = %d\n", i, fibonacci(i));
    }

    // --------------------------------------------------------
    // 7. Call by Value
    // --------------------------------------------------------
    printf("\n=== 7. Call by Value ===\n");
    int a = 5, b = 10;
    printf("Before swap_value: a=%d, b=%d\n", a, b);
    swap_value(a, b);
    printf("After  swap_value: a=%d, b=%d (unchanged!)\n", a, b);

    // --------------------------------------------------------
    // 8. Call by Reference
    // --------------------------------------------------------
    printf("\n=== 8. Call by Reference ===\n");
    int p = 5, q = 10;
    printf("Before swap_reference: p=%d, q=%d\n", p, q);
    swap_reference(&p, &q);
    printf("After  swap_reference: p=%d, q=%d (swapped!)\n", p, q);

    // --------------------------------------------------------
    // 9. Returning Multiple Values via Pointers
    // --------------------------------------------------------
    printf("\n=== 9. Multiple Return Values (via pointers) ===\n");
    int arr[] = {3, 1, 7, 2, 9, 5, 4};
    int n = 7;
    int minVal, maxVal;
    minMax(arr, n, &minVal, &maxVal);
    printf("Array: 3 1 7 2 9 5 4\n");
    printf("Min = %d, Max = %d\n", minVal, maxVal);

    // --------------------------------------------------------
    // 10. Static Variable in Function
    // --------------------------------------------------------
    printf("\n=== 10. Static Variable (persists between calls) ===\n");
    counterDemo();
    counterDemo();
    counterDemo();

    // --------------------------------------------------------
    // 11. Utility functions
    // --------------------------------------------------------
    printf("\n=== 11. Utility Functions ===\n");
    for (int i = 1; i <= 10; i++) {
        printf("%2d is %s\n", i, isEven(i) ? "Even" : "Odd");
    }

    printf("\nPrime numbers from 1 to 30: ");
    for (int i = 2; i <= 30; i++) {
        if (isPrime(i)) printf("%d ", i);
    }
    printf("\n");

    // --------------------------------------------------------
    // 12. Math library functions (built-in)
    // --------------------------------------------------------
    printf("\n=== 12. Standard Math Functions ===\n");
    printf("sqrt(144)  = %.2f\n", sqrt(144));
    printf("pow(2, 10) = %.0f\n", pow(2, 10));
    printf("fabs(-3.7) = %.2f\n", fabs(-3.7));

    return 0;
}


// ============================================================
// FUNCTION DEFINITIONS
// ============================================================

// Type 1: No parameters, no return value
void greet() {
    printf("Hello! Welcome to C Functions.\n");
}

// Type 2: Parameters, no return value
void printSquare(int n) {
    printf("Square of %d = %d\n", n, n * n);
}

// Type 3: No parameters, returns a value
int getYear() {
    return 2025;
}

// Type 4: Parameters and return value
int add(int a, int b) {
    return a + b;
}

float average(float a, float b) {
    return (a + b) / 2.0;
}

// Defined AFTER main (prototype was given at top)
int multiply(int a, int b) {
    return a * b;
}

// Recursive: Factorial
int factorial(int n) {
    if (n == 0 || n == 1) return 1;        // base case
    return n * factorial(n - 1);           // recursive call
}

// Recursive: Fibonacci
int fibonacci(int n) {
    if (n == 0) return 0;                  // base case
    if (n == 1) return 1;                  // base case
    return fibonacci(n - 1) + fibonacci(n - 2);  // recursive call
}

// Call by Value — original variables NOT changed
void swap_value(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    printf("Inside swap_value: a=%d, b=%d\n", a, b);
}

// Call by Reference — original variables ARE changed
void swap_reference(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Multiple return values via pointers
void minMax(int arr[], int n, int *min, int *max) {
    *min = arr[0];
    *max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}

// Static variable — retains value between calls
void counterDemo() {
    static int count = 0;  // initialized only ONCE
    count++;
    printf("counterDemo() called %d time(s)\n", count);
}

// Utility: Check if even
int isEven(int n) {
    return n % 2 == 0;
}

// Utility: Check if prime
int isPrime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i <= (int)sqrt(n); i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}
