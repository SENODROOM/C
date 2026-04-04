#include <stdio.h>
#include <stdlib.h>

void main();
int num() {
    

    int secretNumber = rand() % 100 + 1;
    int maxAttempts = 10; 
    int attempts = 0;

    printf("Welcome to the MYSTERY NUMBER CHALLANGE!\n");
    printf("A random number between 1 and 100 has been selected.\n");
    printf("You have %d attempts to guess the number.\n", maxAttempts);
	char play;
    while (attempts < maxAttempts) {
        attempts++;
        printf("Attempt #%d: Enter your guess: ", attempts);
        int guess;
        scanf("%d", &guess);

        if (guess == secretNumber) {
        printf("Congratulations! You guessed the number (%d) correctly in %d attempts!\n", secretNumber, attempts);
        
    	printf("\nDo you want to play again?[Y/N]: ");
    	scanf("\n%c", &play);

		if(play == 'Y' || play == 'y'){
			system("cls");
			num();
		}
		else{
			system("cls");
			main();
		}
        } else if (guess < 1 || guess > 100) {
            printf("Please enter a number between 1 and 100.\n");
        } else if (guess < secretNumber) {
            printf("Try higher!\n");
        } else {
            printf("Try lower!\n");
        }
    }

    printf("Sorry, you've run out of attempts. The secret number was %d.\n", secretNumber);
    
	printf("\nDo you want to play again?[Y/N]: ");
    scanf("\n%c", &play);

	if(play == 'Y' || play == 'y'){
		system("cls");
		num();
	}
	else{
		system("cls");
		main();
	}
    

    return 0;
}

