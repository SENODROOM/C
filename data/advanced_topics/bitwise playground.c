#include <stdio.h>

int main(void) {
    unsigned int a = 12; // 1100
    unsigned int b = 10; // 1010

    printf("a & b = %u\n", a & b);
    printf("a | b = %u\n", a | b);
    printf("a ^ b = %u\n", a ^ b);
    printf("~a = %u\n", ~a);
    printf("a << 1 = %u\n", a << 1);
    printf("b >> 1 = %u\n", b >> 1);
    return 0;
}
