#include <stdio.h>
#include <string.h>

int main(void) {
    char first[40] = "C";
    char second[] = " Programming";

    strcat(first, second);
    printf("Combined: %s\\n", first);
    printf("Length: %zu\\n", strlen(first));

    if (strcmp(first, "C Programming") == 0) {
        printf("Strings match.\\n");
    }
    return 0;
}
