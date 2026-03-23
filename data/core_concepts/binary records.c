#include <stdio.h>

typedef struct {
    int id;
    float score;
} Record;

int main(void) {
    Record out[3] = {{1, 88.5f}, {2, 74.0f}, {3, 91.25f}};

    FILE *fp = fopen("records.bin", "wb");
    if (fp == NULL) {
        printf("Cannot open records.bin for writing.\n");
        return 1;
    }
    fwrite(out, sizeof(Record), 3, fp);
    fclose(fp);

    Record in[3];
    fp = fopen("records.bin", "rb");
    if (fp == NULL) {
        printf("Cannot open records.bin for reading.\n");
        return 1;
    }
    fread(in, sizeof(Record), 3, fp);
    fclose(fp);

    for (int i = 0; i < 3; i++) {
        printf("Record %d -> id=%d score=%.2f\n", i + 1, in[i].id, in[i].score);
    }

    return 0;
}
