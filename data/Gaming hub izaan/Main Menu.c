#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
      
#include "./games/tic_tac_toe.c"  
#include "./games/hangman.c"
#include "./games/ping_pong.c"  
#include "./games/mountain_ascend.c"      
#include "./games/num_guesser.c"  
#include "./games/snake.c"   
#include "./games/minesweeper.c"  
#include "./games/rock_paper.c"   
#include "./games/maze.c"       

void main(){

    system("cls");
    printf("=========================================\n");
    printf("\t|| Welcome to C Gaming Hub");
    printf("\n");
    printf("%s", "\t|| Choose a Game:");
    printf("\n");
    printf("=========================================\n");
    printf("%s", "\t||  1. Tic Tac Toe");
    printf("\n");
    printf("%s", "\t||  2. Hangman");
    printf("\n");
    printf("%s", "\t||  3. Ping Pong");
    printf("\n");
    printf("%s", "\t||  4. Mountain Ascend And Slip Stream");
    printf("\n");
    printf("%s", "\t||  5. Random Number Guesser");
    printf("\n");
    printf("%s", "\t||  6. Snake Game");
    printf("\n");
    printf("%s", "\t||  7. Mine Sweeper");
    printf("\n");
    printf("%s", "\t||  8. Rock, Paper and Scissors");
    printf("\n");
    printf("%s", "\t||  9. Maze Escape");
    printf("\n");
    printf("=========================================\n");
    printf("%s", "\t||  0. Exit");
    printf("\n");
    printf("=========================================\n");
    
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
	
	switch (choice)
    {
    case 1: // tic tac toe
    system("cls");
    tic_tac_toe();
    break;
    case 2: // hang man
    system("cls");
    hangman();
    break;
    case 3: // ping pong
    system("cls");
    ping_pong();
    break;
    case 4: // mountain ascend
    system("cls");
    mountain_ascend();
    case 5: // number guesser
    system("cls");
    num();
    break;
    case 6: // snake
    system("cls");
    snake();
    case 7: // minesweeper
    system("cls");
    mine();
    break;
    case 8: // rock paper scissors
    system("cls");
    rockpaper();
    break;
    case 9: // rock paper scissors
    system("cls");
    maze();
    break;
    case 0: // exit
    system("cls");
    printf("Thanks for playing C Games.");
    return;
    break;
    default:
        system("cls");
        printf("Invalid Choice");
        getch();
        main();
        break;
    }    
        
       
}