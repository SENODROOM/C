#include <stdio.h>

int main(void) {
    int age = 21;
    float score = 88.5f;
    char grade = 'B';

    printf("Age: %d\\n", age);
    printf("Score: %.1f\\n", score);
    printf("Grade: %c\\n", grade);

    age += 1;
    score = score + 5.0f;

    printf("Next year age: %d\\n", age);
    printf("Updated score: %.1f\\n", score);
    return 0;
}
