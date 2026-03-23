// ============================================================
// FILE: scanf_format_specifiers.c
// TOPIC: scanf() — Every Format Specifier with Live Examples
//
// HOW TO COMPILE:
//   gcc scanf_format_specifiers.c -o scanf_demo
//
// HOW TO RUN:
//   ./scanf_demo
//
// Each section demonstrates ONE format specifier.
// Follow the prompt and enter the requested input.
// ============================================================

#include <stdio.h>

// Helper to flush leftover input between tests
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ============================================================
int main() {

    printf("============================================\n");
    printf("       scanf() Format Specifier Demo        \n");
    printf("============================================\n\n");


    // ----------------------------------------------------------
    // 1. %d — int (decimal integer)
    // ----------------------------------------------------------
    printf("--- 1. %%d : Read a decimal integer ---\n");
    int age;
    printf("Enter your age: ");
    scanf("%d", &age);
    printf("You entered: %d\n\n", age);
    clearBuffer();


    // ----------------------------------------------------------
    // 2. %i — int (auto-detect base: decimal, octal, hex)
    // ----------------------------------------------------------
    printf("--- 2. %%i : Auto-detect base (decimal/octal/hex) ---\n");
    int numI;
    printf("Enter a number (try: 255, 0377, or 0xFF): ");
    scanf("%i", &numI);
    printf("Stored decimal value: %d\n\n", numI);
    clearBuffer();


    // ----------------------------------------------------------
    // 3. %f — float
    // ----------------------------------------------------------
    printf("--- 3. %%f : Read a float ---\n");
    float price;
    printf("Enter a price (e.g. 29.99): ");
    scanf("%f", &price);
    printf("You entered: %.2f\n\n", price);
    clearBuffer();


    // ----------------------------------------------------------
    // 4. %lf — double (MUST use %lf for double in scanf)
    // ----------------------------------------------------------
    printf("--- 4. %%lf : Read a double ---\n");
    double pi;
    printf("Enter a precise value (e.g. 3.14159265): ");
    scanf("%lf", &pi);
    printf("You entered: %.8lf\n\n", pi);
    clearBuffer();


    // ----------------------------------------------------------
    // 5. %Lf — long double
    // ----------------------------------------------------------
    printf("--- 5. %%Lf : Read a long double ---\n");
    long double ld;
    printf("Enter a long double (e.g. 1.23456789012345): ");
    scanf("%Lf", &ld);
    printf("You entered: %.10Lf\n\n", ld);
    clearBuffer();


    // ----------------------------------------------------------
    // 6. %c — single character
    // ----------------------------------------------------------
    printf("--- 6. %%c : Read a single character ---\n");
    char ch;
    printf("Enter a single character: ");
    scanf(" %c", &ch);   // leading space skips leftover whitespace
    printf("You entered: '%c' (ASCII: %d)\n\n", ch, ch);
    clearBuffer();


    // ----------------------------------------------------------
    // 7. %s — string (single word, stops at space)
    // ----------------------------------------------------------
    printf("--- 7. %%s : Read a word (string) ---\n");
    char word[50];
    printf("Enter one word: ");
    scanf("%49s", word);         // width limit for safety
    printf("You entered: \"%s\"\n\n", word);
    clearBuffer();


    // ----------------------------------------------------------
    // 8. %[^\n] — scanset: read full line including spaces
    // ----------------------------------------------------------
    printf("--- 8. %%[^\\n] : Read a full line with spaces ---\n");
    char sentence[100];
    printf("Enter a full sentence: ");
    scanf(" %[^\n]", sentence);  // reads everything until Enter
    printf("You entered: \"%s\"\n\n", sentence);
    clearBuffer();


    // ----------------------------------------------------------
    // 9. %[a-z] — scanset: only lowercase letters
    // ----------------------------------------------------------
    printf("--- 9. %%[a-z] : Read only lowercase letters ---\n");
    char lower[50];
    printf("Enter lowercase letters only (stops at uppercase/space): ");
    scanf("%49[a-z]", lower);
    printf("Captured: \"%s\"\n\n", lower);
    clearBuffer();


    // ----------------------------------------------------------
    // 10. %o — octal integer
    // ----------------------------------------------------------
    printf("--- 10. %%o : Read an octal integer ---\n");
    int octalNum;
    printf("Enter an octal number (e.g. 17 = decimal 15): ");
    scanf("%o", &octalNum);
    printf("Octal input stored as decimal: %d\n\n", octalNum);
    clearBuffer();


    // ----------------------------------------------------------
    // 11. %x — hexadecimal integer
    // ----------------------------------------------------------
    printf("--- 11. %%x : Read a hexadecimal integer ---\n");
    int hexNum;
    printf("Enter a hex number (e.g. ff or 1A): ");
    scanf("%x", &hexNum);
    printf("Hex input stored as decimal: %d\n\n", hexNum);
    clearBuffer();


    // ----------------------------------------------------------
    // 12. %u — unsigned integer
    // ----------------------------------------------------------
    printf("--- 12. %%u : Read an unsigned integer ---\n");
    unsigned int uNum;
    printf("Enter a positive integer: ");
    scanf("%u", &uNum);
    printf("You entered: %u\n\n", uNum);
    clearBuffer();


    // ----------------------------------------------------------
    // 13. %ld — long int
    // ----------------------------------------------------------
    printf("--- 13. %%ld : Read a long int ---\n");
    long int bigNum;
    printf("Enter a large integer (e.g. 2147483647): ");
    scanf("%ld", &bigNum);
    printf("You entered: %ld\n\n", bigNum);
    clearBuffer();


    // ----------------------------------------------------------
    // 14. %lld — long long int
    // ----------------------------------------------------------
    printf("--- 14. %%lld : Read a long long int ---\n");
    long long int veryBig;
    printf("Enter a very large integer (e.g. 9223372036854775807): ");
    scanf("%lld", &veryBig);
    printf("You entered: %lld\n\n", veryBig);
    clearBuffer();


    // ----------------------------------------------------------
    // 15. %lu — unsigned long
    // ----------------------------------------------------------
    printf("--- 15. %%lu : Read an unsigned long ---\n");
    unsigned long ul;
    printf("Enter an unsigned long integer: ");
    scanf("%lu", &ul);
    printf("You entered: %lu\n\n", ul);
    clearBuffer();


    // ----------------------------------------------------------
    // 16. %n — number of characters read so far (no input!)
    // ----------------------------------------------------------
    printf("--- 16. %%n : Count characters read so far ---\n");
    int numVal, charsRead;
    printf("Enter an integer: ");
    scanf("%d%n", &numVal, &charsRead);
    printf("You entered: %d\n", numVal);
    printf("Characters consumed by scanf: %d\n\n", charsRead);
    clearBuffer();


    // ----------------------------------------------------------
    // 17. Width specifier — limit input length
    // ----------------------------------------------------------
    printf("--- 17. Width specifier (e.g. %%5d reads max 5 digits) ---\n");
    int limited;
    printf("Enter a number (only first 5 digits will be read): ");
    scanf("%5d", &limited);
    printf("Value stored (max 5 digits): %d\n\n", limited);
    clearBuffer();


    // ----------------------------------------------------------
    // 18. Reading multiple values at once
    // ----------------------------------------------------------
    printf("--- 18. Multiple values in one scanf ---\n");
    int x, y;
    printf("Enter two integers separated by space: ");
    scanf("%d %d", &x, &y);
    printf("x = %d, y = %d\n", x, y);
    printf("Sum = %d\n\n", x + y);
    clearBuffer();


    // ----------------------------------------------------------
    // 19. scanf return value
    // ----------------------------------------------------------
    printf("--- 19. Return value of scanf ---\n");
    int a, b;
    printf("Enter two integers: ");
    int itemsRead = scanf("%d %d", &a, &b);
    printf("scanf returned: %d (items successfully read)\n", itemsRead);
    if (itemsRead == 2) {
        printf("Both values read: %d and %d\n\n", a, b);
    } else {
        printf("Could not read both values.\n\n");
    }
    clearBuffer();


    // ----------------------------------------------------------
    // 20. %p — pointer address
    // ----------------------------------------------------------
    printf("--- 20. %%p : Read a pointer/memory address ---\n");
    int variable = 42;
    void *ptr = &variable;
    printf("Address of 'variable' is: %p\n", ptr);
    printf("(%%p is used to print/read memory addresses)\n\n");


    // ----------------------------------------------------------
    // SUMMARY TABLE (printed to screen)
    // ----------------------------------------------------------
    printf("============================================\n");
    printf("         SCANF FORMAT SPECIFIER SUMMARY     \n");
    printf("============================================\n");
    printf("%-8s %-18s %-20s\n", "Spec",  "Type",           "Variable Declaration");
    printf("%-8s %-18s %-20s\n", "----",  "----",           "--------------------");
    printf("%-8s %-18s %-20s\n", "%%d",   "int",            "int n;");
    printf("%-8s %-18s %-20s\n", "%%i",   "int (any base)", "int n;");
    printf("%-8s %-18s %-20s\n", "%%f",   "float",          "float f;");
    printf("%-8s %-18s %-20s\n", "%%lf",  "double",         "double d;");
    printf("%-8s %-18s %-20s\n", "%%Lf",  "long double",    "long double ld;");
    printf("%-8s %-18s %-20s\n", "%%c",   "char",           "char c;");
    printf("%-8s %-18s %-20s\n", "%%s",   "string (word)",  "char s[50];");
    printf("%-8s %-18s %-20s\n", "%%[^\\n]","string (line)","char s[100];");
    printf("%-8s %-18s %-20s\n", "%%o",   "octal int",      "int n;");
    printf("%-8s %-18s %-20s\n", "%%x",   "hex int",        "int n;");
    printf("%-8s %-18s %-20s\n", "%%u",   "unsigned int",   "unsigned int n;");
    printf("%-8s %-18s %-20s\n", "%%ld",  "long int",       "long n;");
    printf("%-8s %-18s %-20s\n", "%%lld", "long long int",  "long long n;");
    printf("%-8s %-18s %-20s\n", "%%lu",  "unsigned long",  "unsigned long n;");
    printf("%-8s %-18s %-20s\n", "%%p",   "pointer",        "void *p;");
    printf("%-8s %-18s %-20s\n", "%%n",   "chars read",     "int n;");
    printf("============================================\n");

    return 0;
}
