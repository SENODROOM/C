#include <stdio.h>

int add(int a, int b);
void swap_by_pointer(int *x, int *y);

int main(void) {
    int x = 10;
    int y = 20;

    printf("add(%d, %d) = %d\\n", x, y, add(x, y));

    swap_by_pointer(&x, &y);
    printf("After swap: x=%d, y=%d\\n", x, y);
    return 0;
}

int add(int a, int b) {
    return a + b;
}

void swap_by_pointer(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}
