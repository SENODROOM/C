#include <stdio.h>
#include <string.h>

int main(void) {
    FILE *fp = fopen("students.csv", "r");
    if (fp == NULL) {
        printf("students.csv not found. Create one with: id,name,marks\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *id = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *marks = strtok(NULL, ",\n");

        if (id && name && marks) {
            printf("id=%s name=%s marks=%s\n", id, name, marks);
        }
    }

    fclose(fp);
    return 0;
}
