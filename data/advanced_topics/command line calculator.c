#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <num1> <op> <num2>\n", argv[0]);
        printf("Example: %s 10 + 5\n", argv[0]);
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[3]);
    char *op = argv[2];

    if (strcmp(op, "+") == 0) {
        printf("%.2f\n", a + b);
    } else if (strcmp(op, "-") == 0) {
        printf("%.2f\n", a - b);
    } else if (strcmp(op, "x") == 0 || strcmp(op, "*") == 0) {
        printf("%.2f\n", a * b);
    } else if (strcmp(op, "/") == 0) {
        if (b == 0) {
            printf("Division by zero error.\n");
            return 1;
        }
        printf("%.2f\n", a / b);
    } else {
        printf("Unsupported operation: %s\n", op);
        return 1;
    }

    return 0;
}
