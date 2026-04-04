#include <stdio.h>
#include <stdlib.h>

void main();
int mountain_ascend() {    
	system("cls");
	int x = 1, numofelements, randomIndex, randomValue;
    int octagon[] = {1, 2, 3, 4, 5, 6, 7, 8};
    char z;

    printf("Welcome to 'MOUNTAIN ASCEND AND SLIP STREAM'.\n");
    printf("The rules are pretty simple:\n");
    printf("You have an OCTAGON (an 8-sided shape) that you can roll to climb up the mountain.\n");
    printf("You have to take 100 steps in order to reach the top.\n");
    printf("The number that you get from 1-8 is equivalent to the steps you take\n");
    printf("BEWARE! THERE ARE A FEW SLIP STREAMS WHICH MAY DESCEND YOU, SO BE CAREFUL.\n");
    printf("\n");

    numofelements = sizeof(octagon) / sizeof(octagon[0]);

    while (x != 100) {
        printf("Press 'r' to roll the Octagon or any other key to exit the game: ");
        scanf(" %c", &z);

        if (z == 'r') {
            randomIndex = rand() % numofelements+1;
            randomValue = octagon[randomIndex];
            printf("%d\n", randomValue);

            if (randomValue >= 1 && randomValue <= 8) {
                x += randomValue;
                printf("You are at step %d\n", x);

                if (x == 24 || x == 37) {
                    x -= 20;
                    printf("Oops! You slipped to %dth step\n", x);
                }
                if (x == 45 || x == 74) {
                    x -= 10;
                    printf("Oops! You slipped to %dth step\n", x);
                }
                if (x == 84 || x == 93) {
                    x -= 8;
                    printf("Oops! You slipped to %dth step\n", x);
                }
                if (x == 98) {
                    x -= 2;
                    printf("Oops! You slipped to %dth step\n", x);
                }

                if (x == 100) {
                    printf("Congrats! You Won.\n");
                    break;
                }
                if(x>100){
                	x=99;
                	printf("Oops! You slipped to %dth step\n", x);
				}
            }
        } 
        else{
        	x = 100;
		}
        
	}
	
	char play;
    printf("\nDo you want to play again?[Y/N]: ");
    scanf("\n%c", &play);

	if(play == 'Y' || play == 'y'){
		mountain_ascend();
	}
	else{
		main();
	}
	
}
        
