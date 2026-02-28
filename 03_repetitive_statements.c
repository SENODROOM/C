// ============================================================
// FILE: repetitive_statements.c
// TOPIC: Repetitive (Loop) Statements in C
// ============================================================

#include <stdio.h>

int main() {

    // --------------------------------------------------------
    // 1. for Loop - Basic
    // --------------------------------------------------------
    printf("=== for Loop ===\n");
    printf("Numbers 1 to 10: ");
    for (int i = 1; i <= 10; i++) {
        printf("%d ", i);
    }
    printf("\n");

    // for loop - sum
    int sum = 0;
    for (int i = 1; i <= 100; i++) {
        sum += i;
    }
    printf("Sum of 1 to 100: %d\n", sum);

    // for loop - reverse
    printf("Countdown: ");
    for (int i = 10; i >= 1; i--) {
        printf("%d ", i);
    }
    printf("\n");

    // Nested for loop - multiplication table
    printf("\n3x3 Multiplication Table:\n");
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            printf("%3d", i * j);
        }
        printf("\n");
    }

    // Pattern using nested for
    printf("\nStar Pattern:\n");
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }

    // --------------------------------------------------------
    // 2. while Loop
    // --------------------------------------------------------
    printf("\n=== while Loop ===\n");
    int i = 1;
    printf("Odd numbers up to 10: ");
    while (i <= 10) {
        if (i % 2 != 0) printf("%d ", i);
        i++;
    }
    printf("\n");

    // while loop - digit count
    int number = 123456;
    int digits = 0;
    int temp = number;
    while (temp != 0) {
        temp /= 10;
        digits++;
    }
    printf("Number of digits in %d: %d\n", number, digits);

    // while loop - reverse number
    int original = 1234;
    int reversed = 0;
    temp = original;
    while (temp != 0) {
        reversed = reversed * 10 + (temp % 10);
        temp /= 10;
    }
    printf("Reverse of %d: %d\n", original, reversed);

    // --------------------------------------------------------
    // 3. do-while Loop
    // --------------------------------------------------------
    printf("\n=== do-while Loop ===\n");

    // Runs at least once even if condition is false
    int x = 100;
    printf("do-while (condition false from start):\n");
    do {
        printf("This runs at least once! x = %d\n", x);
        x++;
    } while (x < 10);  // false, but body ran once

    // do-while for menu simulation
    int choice;
    printf("\nSimulated Menu (do-while):\n");
    int iteration = 0;
    do {
        printf("  1. Add  2. Delete  3. Exit\n");
        choice = 3;  // simulate choosing exit
        printf("  Choice selected: %d\n", choice);
        iteration++;
    } while (choice != 3);
    printf("Menu exited.\n");

    // --------------------------------------------------------
    // 4. break - exit loop early
    // --------------------------------------------------------
    printf("\n=== break Statement ===\n");
    printf("Printing until we find 5: ");
    for (int j = 0; j < 10; j++) {
        if (j == 5) {
            printf("(found 5, stopping!) ");
            break;
        }
        printf("%d ", j);
    }
    printf("\n");

    // --------------------------------------------------------
    // 5. continue - skip an iteration
    // --------------------------------------------------------
    printf("\n=== continue Statement ===\n");
    printf("Even numbers 1-10 (skip odds): ");
    for (int j = 1; j <= 10; j++) {
        if (j % 2 != 0) continue;  // skip odd
        printf("%d ", j);
    }
    printf("\n");

    // --------------------------------------------------------
    // 6. goto
    // --------------------------------------------------------
    printf("\n=== goto Statement ===\n");
    int count = 0;
loop_start:
    if (count < 5) {
        printf("%d ", count);
        count++;
        goto loop_start;
    }
    printf("\ngoto loop done.\n");

    // --------------------------------------------------------
    // 7. Practical: Factorial using for
    // --------------------------------------------------------
    printf("\n=== Factorial (for loop) ===\n");
    int n = 6;
    long long factorial = 1;
    for (int j = 1; j <= n; j++) {
        factorial *= j;
    }
    printf("%d! = %lld\n", n, factorial);

    // --------------------------------------------------------
    // 8. Practical: Fibonacci using while
    // --------------------------------------------------------
    printf("\n=== Fibonacci (while loop) ===\n");
    int terms = 10;
    long long a = 0, b = 1;
    printf("First %d Fibonacci numbers: ", terms);
    int t = 0;
    while (t < terms) {
        printf("%lld ", a);
        long long next = a + b;
        a = b;
        b = next;
        t++;
    }
    printf("\n");

    return 0;
}
