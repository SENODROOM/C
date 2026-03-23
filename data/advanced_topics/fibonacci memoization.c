#include <stdio.h>

long long fib(int n, long long memo[]) {
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n];
    }
    memo[n] = fib(n - 1, memo) + fib(n - 2, memo);
    return memo[n];
}

int main(void) {
    int n;
    printf("Find fibonacci(n), n = ");
    scanf("%d", &n);

    if (n < 0 || n > 92) {
        printf("Choose n between 0 and 92.\n");
        return 1;
    }

    long long memo[93];
    for (int i = 0; i <= 92; i++) {
        memo[i] = -1;
    }

    printf("fib(%d) = %lld\n", n, fib(n, memo));
    return 0;
}
