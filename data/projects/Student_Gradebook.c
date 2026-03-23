/* Student Gradebook Program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 5

struct Student {
    int rollNumber;
    char name[50];
    float marks[MAX_SUBJECTS];
    float gpa;
    char grade;
};

int studentCount = 0;
struct Student students[MAX_STUDENTS];

float calculateGPA(struct Student* s) {
    float sum = 0;
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        sum += s->marks[i];
    }
    return sum / MAX_SUBJECTS;
}

char getGrade(float gpa) {
    if (gpa >= 90) return 'A';
    if (gpa >= 80) return 'B';
    if (gpa >= 70) return 'C';
    if (gpa >= 60) return 'D';
    return 'F';
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Cannot add more students!\n");
        return;
    }
    
    printf("\nAdd New Student:\n");
    printf("Roll Number: ");
    scanf("%d", &students[studentCount].rollNumber);
    printf("Name: ");
    scanf("%s", students[studentCount].name);
    
    printf("Enter marks for %d subjects:\n", MAX_SUBJECTS);
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("Subject %d: ", i + 1);
        scanf("%f", &students[studentCount].marks[i]);
    }
    
    students[studentCount].gpa = calculateGPA(&students[studentCount]);
    students[studentCount].grade = getGrade(students[studentCount].gpa);
    
    studentCount++;
    printf("Student added successfully!\n");
}

void viewAllStudents() {
    if (studentCount == 0) {
        printf("\nNo students to display!\n");
        return;
    }
    
    printf("\n=== Student Gradebook ===\n");
    printf("%-5s %-20s %8s %8s\n", "Roll", "Name", "GPA", "Grade");
    printf("----------------------------------------------\n");
    
    for (int i = 0; i < studentCount; i++) {
        printf("%-5d %-20s %8.2f %8c\n", 
               students[i].rollNumber,
               students[i].name,
               students[i].gpa,
               students[i].grade);
    }
}

void calculateClassStatistics() {
    if (studentCount == 0) {
        printf("\nNo students to analyze!\n");
        return;
    }
    
    float totalGPA = 0;
    float maxGPA = students[0].gpa;
    float minGPA = students[0].gpa;
    
    for (int i = 0; i < studentCount; i++) {
        totalGPA += students[i].gpa;
        if (students[i].gpa > maxGPA) maxGPA = students[i].gpa;
        if (students[i].gpa < minGPA) minGPA = students[i].gpa;
    }
    
    printf("\n=== Class Statistics ===\n");
    printf("Total Students: %d\n", studentCount);
    printf("Class Average GPA: %.2f\n", totalGPA / studentCount);
    printf("Highest GPA: %.2f\n", maxGPA);
    printf("Lowest GPA: %.2f\n", minGPA);
}

int main() {
    int choice;
    int continueLoop = 1;
    
    printf("=== Student Gradebook ===\n\n");
    
    while (continueLoop) {
        printf("\nMenu:\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Class Statistics\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewAllStudents();
                break;
            case 3:
                calculateClassStatistics();
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
