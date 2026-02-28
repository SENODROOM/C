// ============================================================
// FILE: io_statements.c
// TOPIC: Input/Output Statements in C
// ============================================================

#include <stdio.h>

int main() {

    // --------------------------------------------------------
    // 1. printf() - Formatted Output
    // --------------------------------------------------------
    printf("=== printf() Examples ===\n");

    int num = 42;
    float pi = 3.14159;
    char grade = 'A';
    char name[] = "Alice";

    printf("Integer  : %d\n", num);
    printf("Float    : %.2f\n", pi);      // 2 decimal places
    printf("Character: %c\n", grade);
    printf("String   : %s\n", name);
    printf("Octal    : %o\n", num);       // 42 in octal
    printf("Hex      : %x\n", num);       // 42 in hex

    // --------------------------------------------------------
    // 2. puts() - Print string with automatic newline
    // --------------------------------------------------------
    printf("\n=== puts() Example ===\n");
    puts("This is printed using puts().");

    // --------------------------------------------------------
    // 3. putchar() - Print a single character
    // --------------------------------------------------------
    printf("\n=== putchar() Example ===\n");
    putchar('H');
    putchar('i');
    putchar('\n');

    // --------------------------------------------------------
    // 4. scanf() - Read formatted input
    // --------------------------------------------------------
    printf("\n=== scanf() Example ===\n");
    int age;
    printf("Enter your age: ");
    scanf("%d", &age);
    printf("You entered: %d\n", age);

    // --------------------------------------------------------
    // 5. getchar() - Read single character
    // --------------------------------------------------------
    printf("\n=== getchar() Example ===\n");
    char ch;
    // clear input buffer after scanf
    while (getchar() != '\n');  
    printf("Enter a character: ");
    ch = getchar();
    printf("You entered: %c\n", ch);

    // --------------------------------------------------------
    // 6. fgets() - Safe string input
    // --------------------------------------------------------
    printf("\n=== fgets() Example ===\n");
    char fullName[50];
    // clear buffer
    while (getchar() != '\n');
    printf("Enter your full name: ");
    fgets(fullName, 50, stdin);
    printf("Hello, %s", fullName);

    // --------------------------------------------------------
    // 7. File I/O
    // --------------------------------------------------------
    printf("\n=== File I/O Example ===\n");

    // Writing to a file
    FILE *fp = fopen("output.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "Name: %s", fullName);
        fprintf(fp, "Age: %d\n", age);
        fclose(fp);
        printf("Data written to output.txt\n");
    } else {
        printf("Error opening file!\n");
    }

    // Reading from the file
    fp = fopen("output.txt", "r");
    if (fp != NULL) {
        char line[100];
        printf("Reading from file:\n");
        while (fgets(line, 100, fp)) {
            printf("  %s", line);
        }
        fclose(fp);
    }

    return 0;
}
