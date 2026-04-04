#include <stdio.h>

void main();

int tic_tac_toe()
{	
	char square[10] = { 'o', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    int player = 1, i, choice;

    char mark;
    do
    {
        system("cls");
    	printf("\n\n\tTic Tac Toe\n\n");

    	printf("Player 1 (X)  -  Player 2 (O)\n\n\n");


    	printf("\t      |     |     \n");
    	printf("\t   %c  |  %c  |  %c \n", square[1], square[2], square[3]);

    	printf("\t _____|_____|_____\n");
    	printf("\t      |     |     \n");

    	printf("\t   %c  |  %c  |  %c \n", square[4], square[5], square[6]);

    	printf("\t _____|_____|_____\n");
    	printf("\t      |     |     \n");

   		printf("\t   %c  |  %c  |  %c \n", square[7], square[8], square[9]);

    	printf("\t      |     |     \n\n\n");
    
        if(player % 2 == 1){
        	player = 1;
		}
		else{
			player = 2;
		}

        printf("Player %d, enter a number:  ", player);
        scanf("%d", &choice);

        mark = (player == 1) ? 'X' : 'O';

        if (choice == 1 && square[1] == '1')
            square[1] = mark;
            
        else if (choice == 2 && square[2] == '2')
            square[2] = mark;
            
        else if (choice == 3 && square[3] == '3')
            square[3] = mark;
            
        else if (choice == 4 && square[4] == '4')
            square[4] = mark;
            
        else if (choice == 5 && square[5] == '5')
            square[5] = mark;
            
        else if (choice == 6 && square[6] == '6')
            square[6] = mark;
            
        else if (choice == 7 && square[7] == '7')
            square[7] = mark;
            
        else if (choice == 8 && square[8] == '8')
            square[8] = mark;
            
        else if (choice == 9 && square[9] == '9')
            square[9] = mark;
            
        else
        {
            printf("Invalid move ");

            player = player - 1;
            getch();
        }
        if (square[1] == square[2] && square[2] == square[3]){
		i = 1;
		}
        
    	else if (square[4] == square[5] && square[5] == square[6]){
		i = 1;
		}
        
    	else if (square[7] == square[8] && square[8] == square[9]){
		i = 1;
		}
        
    	else if (square[1] == square[4] && square[4] == square[7]){
		i = 1;
		}
        
    	else if (square[2] == square[5] && square[5] == square[8]){
		i = 1;
		}
        
    	else if (square[3] == square[6] && square[6] == square[9]){
		i = 1;
		}
        
    	else if (square[1] == square[5] && square[5] == square[9]){
		i = 1;
		}
        
    	else if (square[3] == square[5] && square[5] == square[7]){
		i = 1;
		}
		
    	else if (square[1] != '1' && square[2] != '2' && square[3] != '3' &&
        square[4] != '4' && square[5] != '5' && square[6] != '6' && square[7] 
        != '7' && square[8] != '8' && square[9] != '9'){
		i = 0;
		}
		
    	else{
        i = - 1;
		}
	
    player = player + 1;
    
	}while (i ==  - 1);
    
            system("cls");
    	printf("\n\n\tTic Tac Toe\n\n");

    	printf("Player 1 (X)  -  Player 2 (O)\n\n\n");


    	printf("\t      |     |     \n");
    	printf("\t   %c  |  %c  |  %c \n", square[1], square[2], square[3]);

    	printf("\t _____|_____|_____\n");
    	printf("\t      |     |     \n");

    	printf("\t   %c  |  %c  |  %c \n", square[4], square[5], square[6]);

    	printf("\t _____|_____|_____\n");
    	printf("\t      |     |     \n");

   		printf("\t   %c  |  %c  |  %c \n", square[7], square[8], square[9]);

    	printf("\t      |     |     \n\n\n");
    
    if (i == 1)
        printf("==>\aPlayer %d is the Winner ", --player);
    else
        printf("==>\aGame draw");
	
	getch();
	
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


