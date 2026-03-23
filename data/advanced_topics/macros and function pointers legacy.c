#include <stdio.h>

#define SQUARE(x) ((x) * (x))

typedef int (*operation_fn)(int, int);

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

int calculate(int x, int y, operation_fn op) {
    return op(x, y);
}

int main(void) {
    int a = 6;
    int b = 4;

    printf("SQUARE(%d) = %d\\n", a, SQUARE(a));
    printf("add: %d\\n", calculate(a, b, add));
    printf("multiply: %d\\n", calculate(a, b, multiply));
    return 0;
}
