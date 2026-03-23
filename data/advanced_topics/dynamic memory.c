#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n = 5;
    int *arr = (int *)malloc(n * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed.\\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = (i + 1) * 10;
    }

    printf("Initial values: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\\n");

    n = 8;
    int *resized = (int *)realloc(arr, n * sizeof(int));
    if (resized == NULL) {
        free(arr);
        printf("Reallocation failed.\\n");
        return 1;
    }
    arr = resized;

    for (int i = 5; i < n; i++) {
        arr[i] = (i + 1) * 10;
    }

    printf("After realloc: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\\n");

    free(arr);
    return 0;
}
