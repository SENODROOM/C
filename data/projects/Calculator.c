/* Basic Calculator Program */
#include <stdio.h>

void add(float a, float b) {
    printf("%.2f + %.2f = %.2f\n", a, b, a + b);
}

void subtract(float a, float b) {
    printf("%.2f - %.2f = %.2f\n", a, b, a - b);
}

void multiply(float a, float b) {
    printf("%.2f * %.2f = %.2f\n", a, b, a * b);
}

void divide(float a, float b) {
    if (b == 0) {
        printf("Error: Division by zero!\n");
    } else {
        printf("%.2f / %.2f = %.2f\n", a, b, a / b);
    }
}

int main() {
    int choice;
    float num1, num2;
    int continueLoop = 1;
    
    printf("=== Simple Calculator ===\n\n");
    
    while (continueLoop) {
        printf("\nMenu:\n");
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n");
        printf("4. Division\n");
        printf("5. Exit\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);
        
        if (choice == 5) {
            printf("Thank you for using the calculator!\n");
            break;
        }
        
        if (choice >= 1 && choice <= 4) {
            printf("Enter first number: ");
            scanf("%f", &num1);
            printf("Enter second number: ");
            scanf("%f", &num2);
            
            printf("Result: ");
            switch (choice) {
                case 1:
                    add(num1, num2);
                    break;
                case 2:
                    subtract(num1, num2);
                    break;
                case 3:
                    multiply(num1, num2);
                    break;
                case 4:
                    divide(num1, num2);
                    break;
            }
        } else {
            printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}
