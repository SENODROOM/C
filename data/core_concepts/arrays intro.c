#include <stdio.h>

int main(void) {
    int arr[] = {3, 5, 7, 9, 11};
    int length = (int)(sizeof(arr) / sizeof(arr[0]));
    int sum = 0;

    for (int i = 0; i < length; i++) {
        sum += arr[i];
    }

    printf("Array length: %d\\n", length);
    printf("Array sum: %d\\n", sum);
    printf("Average: %.2f\\n", (float)sum / length);
    return 0;
}
