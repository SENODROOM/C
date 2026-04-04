#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void main();
void displayFile(FILE *fptr) {
    fptr = fopen("gamefile.txt", "r");
    if (fptr == NULL) {
        printf("File not found or cannot be opened.\n");
        return;
    }

    char ch;
    fseek(fptr, 0, SEEK_SET);
    while ((ch = fgetc(fptr)) != EOF) {
        printf("%c", ch);
    }
    fclose(fptr);
}

void printChoice(int choice, char *string) {
    switch (choice) {
        case 1:
            strcpy(string, "Rock");
            break;
        case 2:
            strcpy(string, "Paper");
            break;
        case 3:
            strcpy(string, "Scissors");
            break;
        default:
            printf("Invalid choice");
    }
}

int rockpaper() {
    char string[10];

    int userScore = 0, computerScore = 0, turns;

    srand(time(0));

    FILE *fptr;

    fptr = fopen("gamefile.txt", "w");

    for (turns = 0; turns < 5; turns++) {
        printf("Enter your choice (1 for Rock, 2 for Paper, 3 for Scissors): ");
        int userChoice;
        scanf("%d", &userChoice);

        int computerChoice = rand() % 3 + 1;

        fprintf(fptr, "Turn number: %d\n", turns + 1);
        fprintf(fptr, "User choice: ");
        printChoice(userChoice, string);
        fprintf(fptr, "%s\n", string);
        fprintf(fptr, "Computer choice: ");
        printChoice(computerChoice, string);
        fprintf(fptr, "%s\n", string);

        if (userChoice == computerChoice) {
            printf("It's a draw!\n");
            fprintf(fptr, "Result: It's a draw!\n");
        } else if ((userChoice == 1 && computerChoice == 3) ||
                   (userChoice == 2 && computerChoice == 1) ||
                   (userChoice == 3 && computerChoice == 2)) {
            printf("You win this round!\n");
            userScore++;
            fprintf(fptr, "Result: User wins this round!\n");
        } else {
            printf("Computer wins this round!\n");
            computerScore++;
            fprintf(fptr, "Result: Computer wins this round!\n");
        }

        char userInput[10];
        printf("Do you want to check the file for complete score? (yes/no): ");
        scanf("%s", userInput);
        

        if (strcmp(userInput, "yes") == 0) {
            fflush(fptr);
            displayFile(fptr);
        }

        if (userScore == 3 || computerScore == 3) {
            break;
        }
    }

    fclose(fptr);

    if (userScore == 3) {
        printf("Congratulations! You win the game.\n");
    } else if (computerScore == 3) {
        printf("Computer wins the game. Better luck next time!\n");
    } else {
        printf("It's a draw.\n");
    }
    
    char play;
    printf("\nDo you want to play again?[Y/N]: ");
    scanf("\n%c", &play);

	if(play == 'Y' || play == 'y'){
		system("cls");
		tic_tac_toe();
	}
	else{
		system("cls");
		main();
	}
	
	
	
    return 0;
}




