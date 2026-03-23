#include <stdio.h>

int main(void) {
    int a, b;
    printf("Enter two integers: ");
    scanf("%d %d", &a, &b);

    printf("Sum: %d\n", a + b);
    printf("Difference: %d\n", a - b);
    printf("Product: %d\n", a * b);

    if (b != 0) {
        printf("Quotient: %.2f\n", (float)a / b);
    } else {
        printf("Quotient: undefined (division by zero)\n");
    }

    return 0;
}
