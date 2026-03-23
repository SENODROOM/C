// ============================================================
// FILE: conditional_statements.c
// TOPIC: Conditional Statements in C
// ============================================================

#include <stdio.h>

int main() {

    // --------------------------------------------------------
    // 1. if statement
    // --------------------------------------------------------
    printf("=== if Statement ===\n");
    int x = 10;
    if (x > 5) {
        printf("x (%d) is greater than 5\n", x);
    }

    // --------------------------------------------------------
    // 2. if-else statement
    // --------------------------------------------------------
    printf("\n=== if-else Statement ===\n");
    int age = 16;
    if (age >= 18) {
        printf("You are an Adult.\n");
    } else {
        printf("You are a Minor.\n");
    }

    // --------------------------------------------------------
    // 3. if - else if - else Ladder
    // --------------------------------------------------------
    printf("\n=== if-else if-else Ladder ===\n");
    int marks = 75;
    printf("Marks: %d\n", marks);
    if (marks >= 90) {
        printf("Grade: A (Excellent)\n");
    } else if (marks >= 75) {
        printf("Grade: B (Good)\n");
    } else if (marks >= 60) {
        printf("Grade: C (Average)\n");
    } else if (marks >= 40) {
        printf("Grade: D (Below Average)\n");
    } else {
        printf("Grade: F (Fail)\n");
    }

    // --------------------------------------------------------
    // 4. Nested if
    // --------------------------------------------------------
    printf("\n=== Nested if ===\n");
    int a = 10, b = 20, c = 15;
    if (a > 0) {
        if (b > a) {
            if (c > a && c < b) {
                printf("Order: a < c < b (%d < %d < %d)\n", a, c, b);
            }
        }
    }

    // --------------------------------------------------------
    // 5. switch Statement
    // --------------------------------------------------------
    printf("\n=== switch Statement ===\n");
    int day = 3;
    printf("Day number: %d\n", day);
    switch (day) {
        case 1:  printf("Monday\n");    break;
        case 2:  printf("Tuesday\n");   break;
        case 3:  printf("Wednesday\n"); break;
        case 4:  printf("Thursday\n");  break;
        case 5:  printf("Friday\n");    break;
        case 6:  printf("Saturday\n");  break;
        case 7:  printf("Sunday\n");    break;
        default: printf("Invalid day\n");
    }

    // switch with char
    printf("\nEnter a vowel or consonant check:\n");
    char ch = 'e';
    switch (ch) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
            printf("'%c' is a Vowel\n", ch);
            break;
        default:
            printf("'%c' is a Consonant\n", ch);
    }

    // --------------------------------------------------------
    // 6. Ternary Operator ? :
    // --------------------------------------------------------
    printf("\n=== Ternary Operator ===\n");
    int num = 7;
    char *type = (num % 2 == 0) ? "Even" : "Odd";
    printf("%d is %s\n", num, type);

    int p = 100, q = 200;
    int max = (p > q) ? p : q;
    printf("Maximum of %d and %d is: %d\n", p, q, max);

    // --------------------------------------------------------
    // 7. Logical Operators in conditions
    // --------------------------------------------------------
    printf("\n=== Logical Operators ===\n");
    int score = 85;
    int attendance = 80;

    if (score >= 60 && attendance >= 75) {
        printf("Eligible to pass (both conditions met)\n");
    }

    if (score >= 90 || attendance >= 90) {
        printf("Eligible for honors (at least one condition met)\n");
    } else {
        printf("Not eligible for honors\n");
    }

    return 0;
}
