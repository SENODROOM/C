#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    printf("How many numbers? ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Invalid size\n");
        return 1;
    }

    int *arr = (int *)malloc((size_t)n * sizeof(int));
    if (arr == NULL) {
        printf("Allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("Value %d: ", i + 1);
        scanf("%d", &arr[i]);
    }

    int min = arr[0], max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    printf("Min: %d, Max: %d\n", min, max);
    free(arr);
    return 0;
}
