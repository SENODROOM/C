#include <stdio.h>

long long factorial(int n) {
    if (n < 0) {
        return -1;
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    return (long long)n * factorial(n - 1);
}

int main(void) {
    int n;
    printf("Enter n: ");
    scanf("%d", &n);

    long long ans = factorial(n);
    if (ans < 0) {
        printf("Factorial is not defined for negative numbers.\n");
        return 1;
    }

    printf("%d! = %lld\n", n, ans);
    return 0;
}
