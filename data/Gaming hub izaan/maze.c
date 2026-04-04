#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<conio.h>

#define MAX_SIZE 6

void main();

char matrix[MAX_SIZE][MAX_SIZE] = {

	{'S', '0', '0', '0', '0', '0'},
    {'1', '1', '1', '1', '0', '1'},
    {'0', '1', '0', '0', '0', '0'},
    {'1', '1', '0', '1', '1', '0'},
    {'0', '0', '0', '0', '0', '1'},
    {'1', '1', '0', '1', '0', 'F'}

};
int playerX, playerY;


void print_maze() {
    int i, j;
    system("cls");
    printf("\n");
    for (i = 0; i < MAX_SIZE; i++) {
    	printf("\t");
        for (j = 0; j < MAX_SIZE; j++) {

            if (i == playerY && j == playerX) {
                printf(" ");
				printf("P ");
            } else {

				printf(" ");
				printf("%c ", matrix[i][j]);
            }
            
        }
        printf("\n");
        printf("\n");
    }
}


void generate_maze() {
    int i, j;

    matrix[0][0] = 'S';


    playerX = 0;
    playerY = 0;
}

int move_player(int x, int y) {
    if (matrix[playerY + y][playerX + x] == 'F') {

        return 1;
    }
    
    if (matrix[playerY + y][playerX + x] == '1') {

        return -1;
    }

    if (matrix[playerY + y][playerX + x] == '0') {

        playerX += x;
        playerY += y;
        return 0;
    }


    return -1;
}

int maze() {


    generate_maze();


    while (1) {

        print_maze();


        char move;
        printf("\nPress any key from W(up),A(left),S(down),D(right) to move the player");
        move = getch();

        int result;

        switch (move) {
            case 'w':
                result = move_player(0, -1); // Move up
                break;
            case 's':
                result = move_player(0, 1);  // Move down
                break;
            case 'a':
                result = move_player(-1, 0); // Move left
                break;
            case 'd':
                result = move_player(1, 0);  // Move right
                break;
            default:
                result = -1; 
        }

        
        if (result == 1) {
            printf("\n\nCongratulations! You reached the end.\n");
            break;
        } 
    }
    
    char play;
    printf("\nDo you want to play again?[Y/N]: ");
    scanf("\n%c", &play);

	if(play == 'Y' || play == 'y'){
		system("cls");
		maze();
	}
	else{
		system("cls");
		main();
	}

    return 0;
}

