#include <stdio.h>

int main(void) {
    int n;
    printf("Enter n: ");
    scanf("%d", &n);

    if (n < 1) {
        printf("Enter n >= 1\n");
        return 1;
    }

    int sum_even = 0;
    int sum_odd = 0;

    for (int i = 1; i <= n; i++) {
        if (i % 2 == 0) {
            sum_even += i;
        } else {
            sum_odd += i;
        }
    }

    printf("Odd sum (1..n): %d\n", sum_odd);
    printf("Even sum (1..n): %d\n", sum_even);
    return 0;
}
