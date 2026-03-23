#include <stdio.h>

int main(void) {
    int id;
    char name[50];

    printf("Enter id: ");
    scanf("%d", &id);

    // Leading space consumes trailing newline from previous input.
    printf("Enter first name: ");
    scanf(" %49s", name);

    printf("Student -> id: %d, name: %s\\n", id, name);
    return 0;
}
