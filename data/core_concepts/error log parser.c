#include <stdio.h>
#include <string.h>

int main(void) {
    FILE *fp = fopen("app.log", "r");
    if (fp == NULL) {
        printf("app.log not found.\n");
        return 1;
    }

    char line[256];
    int error_count = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "ERROR") != NULL) {
            error_count++;
            printf("%s", line);
        }
    }

    fclose(fp);
    printf("Total ERROR lines: %d\n", error_count);
    return 0;
}
