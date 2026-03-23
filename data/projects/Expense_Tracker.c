/* Expense Tracker Program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPENSES 100

struct Expense {
    char date[11];
    char category[50];
    char description[100];
    float amount;
};

int expenseCount = 0;
struct Expense expenses[MAX_EXPENSES];

void addExpense() {
    if (expenseCount >= MAX_EXPENSES) {
        printf("Cannot add more expenses!\n");
        return;
    }
    
    printf("\nAdd New Expense:\n");
    printf("Date (DD/MM/YYYY): ");
    scanf("%s", expenses[expenseCount].date);
    printf("Category (Food/Transport/Entertainment/Other): ");
    scanf("%s", expenses[expenseCount].category);
    printf("Description: ");
    scanf("%s", expenses[expenseCount].description);
    printf("Amount: ");
    scanf("%f", &expenses[expenseCount].amount);
    
    expenseCount++;
    printf("Expense added successfully!\n");
}

void viewAllExpenses() {
    if (expenseCount == 0) {
        printf("\nNo expenses to display!\n");
        return;
    }
    
    printf("\n=== All Expenses ===\n");
    printf("%-12s %-15s %-20s %8s\n", "Date", "Category", "Description", "Amount");
    printf("----------------------------------------------------------\n");
    
    float total = 0;
    for (int i = 0; i < expenseCount; i++) {
        printf("%-12s %-15s %-20s %8.2f\n", 
               expenses[i].date, 
               expenses[i].category, 
               expenses[i].description, 
               expenses[i].amount);
        total += expenses[i].amount;
    }
    printf("----------------------------------------------------------\n");
    printf("Total: %.2f\n", total);
}

void viewByCategory() {
    if (expenseCount == 0) {
        printf("\nNo expenses to display!\n");
        return;
    }
    
    char category[50];
    printf("\nEnter category to filter: ");
    scanf("%s", category);
    
    printf("\nExpenses in category '%s':\n", category);
    printf("%-12s %-20s %8s\n", "Date", "Description", "Amount");
    printf("-------------------------------------\n");
    
    float categoryTotal = 0;
    int found = 0;
    for (int i = 0; i < expenseCount; i++) {
        if (strcmp(expenses[i].category, category) == 0) {
            printf("%-12s %-20s %8.2f\n", 
                   expenses[i].date, 
                   expenses[i].description, 
                   expenses[i].amount);
            categoryTotal += expenses[i].amount;
            found = 1;
        }
    }
    
    if (!found) {
        printf("No expenses in this category.\n");
    } else {
        printf("-------------------------------------\n");
        printf("Category Total: %.2f\n", categoryTotal);
    }
}

int main() {
    int choice;
    int continueLoop = 1;
    
    printf("=== Expense Tracker ===\n\n");
    
    while (continueLoop) {
        printf("\nMenu:\n");
        printf("1. Add Expense\n");
        printf("2. View All Expenses\n");
        printf("3. View by Category\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addExpense();
                break;
            case 2:
                viewAllExpenses();
                break;
            case 3:
                viewByCategory();
                break;
            case 4:
                printf("Goodbye!\n");
                continueLoop = 0;
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}
