#include<stdio.h>
void main();
void mine(){

	char mine[6][6] = {
	
	{'X','X','X','X','X','X'},
	{'X','X','X','X','X','X'},
	{'X','X','X','X','X','X'},
	{'X','X','X','X','X','X'},
	{'X','X','X','X','X','X'},
	{'X','X','X','X','X','X'}
	
	};
	
	char script[6][6] = {
	
	{'E','E','E','E','E','B'},
	{'B','E','B','E','E','E'},
	{'B','E','E','E','B','B'},
	{'E','B','E','B','B','E'},
	{'B','E','B','E','E','B'},
	{'E','B','E','B','E','E'}
	
	};
    
    int x,y;
    int run = 1;
    
    while(run){
	
	system("cls");
	printf("\t      |     |     |     |     |     \n");
    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[0][0], mine[0][1], mine[0][2], mine[0][3], mine[0][4],mine[0][5]);
    printf("\t      |     |     |     |     |     \n");
    printf("\t-------------------------------------\n");
    printf("\t      |     |     |     |     |     \n");
    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[1][0], mine[1][1], mine[1][2], mine[1][3], mine[1][4],mine[1][5]);
    printf("\t      |     |     |     |     |     \n");
    printf("\t-------------------------------------\n");
    printf("\t      |     |     |     |     |     \n");
    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[2][0], mine[2][1], mine[2][2], mine[2][3], mine[2][4],mine[2][5]);
    printf("\t      |     |     |     |     |     \n");
    printf("\t-------------------------------------\n");
    printf("\t      |     |     |     |     |     \n");
    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[3][0], mine[3][1], mine[3][2], mine[3][3], mine[3][4],mine[3][5]);
    printf("\t      |     |     |     |     |     \n");
    printf("\t-------------------------------------\n");
    printf("\t      |     |     |     |     |     \n");
    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[4][0], mine[4][1], mine[4][2], mine[4][3], mine[4][4],mine[4][5]);
    printf("\t      |     |     |     |     |     \n");
    printf("\t-------------------------------------\n");
    printf("\t      |     |     |     |     |     \n");
    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[5][0], mine[5][1], mine[5][2], mine[5][3], mine[5][4],mine[5][5]);
    printf("\t      |     |     |     |     |     \n");
	
	
	printf("\n\t    Enter X value of location: ");
	scanf("%d", &x);
	printf("\t    Enter Y value of location: ");
	scanf("%d", &y);
	
	x = x-1;
	y = y-1;
	
	if(script[x][y] == 'E'){
		
		int count;
		count = 0;
		
		if(script[x-1][y+1] == 'B' && x-1 >= 0 && x-1 <= 5 && y+1 >= 0 && y+1 <= 5 ){
			count++;
		}
		if(script[x][y+1] == 'B' && x >= 0 && x <= 5 && y+1 >= 0 && y+1 <= 5 ){
			count++;
		}
		if(script[x+1][y+1] == 'B' && x+1 >= 0 && x+1 <= 5 && y+1 >= 0 && y+1 <= 5 ){
			count++;
		}
		if(script[x-1][y] == 'B' && x-1 >= 0 && x-1 <= 5 && y >= 0 && y <= 5){
			count++;
		}
		if(script[x+1][y] == 'B' && x+1 >= 0 && x+1 <= 5 && y >= 0 && y <= 5){
			count++;
		}
		if(script[x-1][y-1] == 'B' && x-1 >= 0 && x-1 <= 5 && y-1 >= 0 && y-1 <= 5){
			count++;
		}
		if(script[x][y-1] == 'B' && x >= 0 && x <= 5 && y-1 >= 0 && y-1 <= 5){
			count++;
		}
		if(script[x+1][y-1] == 'B' && x+1 >= 0 && x+1 <= 5 && y-1 >= 0 && y-1 <= 5){
			count++;
		}
		
		mine[x][y] = count + '0';
		
	}
	else if(script[x][y] == 'B'){
		
		run = 0;
		mine[x][y] = 'B';
		system("cls");
		printf("\t      |     |     |     |     |     \n");
	    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[0][0], mine[0][1], mine[0][2], mine[0][3], mine[0][4],mine[0][5]);
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t-------------------------------------\n");
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[1][0], mine[1][1], mine[1][2], mine[1][3], mine[1][4],mine[1][5]);
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t-------------------------------------\n");
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[2][0], mine[2][1], mine[2][2], mine[2][3], mine[2][4],mine[2][5]);
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t-------------------------------------\n");
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[3][0], mine[3][1], mine[3][2], mine[3][3], mine[3][4],mine[3][5]);
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t-------------------------------------\n");
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[4][0], mine[4][1], mine[4][2], mine[4][3], mine[4][4],mine[4][5]);
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t-------------------------------------\n");
	    printf("\t      |     |     |     |     |     \n");
	    printf("\t   %c  |  %c  |  %c  |  %c  |  %c  |  %c  \n", mine[5][0], mine[5][1], mine[5][2], mine[5][3], mine[5][4],mine[5][5]);
	    printf("\t      |     |     |     |     |     \n");
		printf("Oops! You hit a Bomb\n");
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
		
	}
	
	
	}
	
	
	
}