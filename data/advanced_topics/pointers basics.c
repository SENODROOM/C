#include <stdio.h>

int main(void) {
    int value = 42;
    int *ptr = &value;

    printf("value: %d\\n", value);
    printf("address of value: %p\\n", (void *)&value);
    printf("ptr stores: %p\\n", (void *)ptr);
    printf("*ptr: %d\\n", *ptr);

    *ptr = 100;
    printf("updated value: %d\\n", value);
    return 0;
}
