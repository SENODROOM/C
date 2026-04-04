#include <stdio.h>
#include <windows.h>
#define V 21
#define H 75

void main();
void loop(char map[V][H], int jini, int jfinal, int iaini, int iafinal, int px, int py);
void edge(char map[V][H]);
void player(char map[V][H], int jini, int jfinal);
void cpu(char map[V][H], int iaini, int iafinal);
void ball(char map[V][H], int py, int px);


void gameloop(char map[V][H], int jini, int jfinal, int iaini, int iafinal, int px, int py, int modx, int mody, int modia);
void draw(char map[V][H]);
void show(char map[V][H]);
void input(char map[V][H], int *jini, int *jfinal, int *iaini, int *iafinal, int *px, int *py, int *modx, int *mody, int *modia, int *gol);
void update(char map[V][H], int jini, int jfinal, int iaini, int iafinal, int py, int px);
void gotoxy(int x, int y);

int playerwin; 
int iawin;


int ping_pong()
{
	
	system("cls");
	playerwin = 0;
	iawin = 0;
	
	char map[V][H];

	
	int jini, jfinal, iaini, iafinal, py, px;

	jini = 8;
	jfinal = 12;

	iaini = 8;
	iafinal = 12;

	py = 10;
	px = 37;

	int modx = -1;
	int mody = -1;
	int modia = -1;

	loop(map, jini, jfinal, iaini, iafinal, px, py);
	gameloop(map, jini, jfinal, iaini, iafinal, px, py, modx, mody, modia);

	printf("\n \n");

	if (playerwin == 1)
	{

		printf("Congrats.\n");
	}

	if (iawin == 1)
	{

		printf("Lose\n");
	}
	
	char play = ' ';
	printf("\nDo you want to play again?[Y/N]: ");
    scanf("\n%c", &play);

	if(play == 'Y' || play == 'y'){
		ping_pong();
	}
	else{
		main();
	}
	return 0;
}

void loop(char map[V][H], int jini, int jfinal, int iaini, int iafinal, int py, int px)
{

	edge(map);
	player(map, jini, jfinal);
	cpu(map, iaini, iafinal);
	ball(map, px, py);
}

void edge(char map[V][H])
{

	int i, j;

	for (i = 0; i < V; i++)
	{

		for (j = 0; j < H; j++)
		{

			if (i == 0 || i == V - 1)
			{

				map[i][j] = '-';
			}
			else if (j == 0 || j == H - 1)
			{

				map[i][j] = '|';
			}
			else
			{

				map[i][j] = ' ';
			}
		}
	}
}

void show(char map[V][H])
{

	int i, j;

	for (i = 0; i < V; i++)
	{

		for (j = 0; j < H; j++)
		{

			printf("%c", map[i][j]);
		}

		printf("\n");
	}
}

void player(char map[V][H], int jini, int jfinal)
{

	int i, j;

	for (i = jini; i < jfinal; i++)
	{

		for (j = 2; j <= 3; j++)
		{

			map[i][j] = 'X';
		}
	}
}

void cpu(char map[V][H], int iaini, int iafinal)
{

	int i, j;

	for (i = iaini; i < iafinal; i++)
	{

		for (j = H - 4; j <= H - 3; j++)
		{

			map[i][j] = 'X';
		}
	}
}

void ball(char map[V][H], int py, int px)
{

	map[py][px] = 'O';
}

void gameloop(char map[V][H], int jini, int jfinal, int iaini, int iafinal, int px, int py, int modx, int mody, int modia)
{

	int gol;
	gol = 0;

	do
	{

		draw(map);
		input(map, &jini, &jfinal, &iaini, &iafinal, &px, &py, &modx, &mody, &modia, &gol);
		update(map, jini, jfinal, iaini, iafinal, px, py);
		Sleep(10);

	} while (gol == 0);
}

void draw(char map[V][H])
{
	gotoxy(0, 0);
	show(map);
}

void input(char map[V][H], int *jini, int *jfinal, int *iaini, int *iafinal, int *px, int *py, int *modx, int *mody, int *modia, int *gol)
{

	int i;
	char key;
	char key2;

	if (*py == 1 || *py == V - 2)
	{ 

		*mody *= -1;
	}

	if (*px == 1)
	{ // Verify GOL

		iawin = 1;
		*gol = 1;
	}

	if (*px == H - 2)
	{

		playerwin = 1;
		*gol = 1;
	}

	if (*px == 4)
	{

		for (i = (*jini); i <= (*jfinal); i++)
		{

			if (i == (*py))
			{

				*modx *= -1;
			}
		}
	}

	if (*px == H-5)
	{

		for (i = (*iaini); i <= (*iafinal); i++)
		{

			if (i == (*py))
			{

				*modx *= -1;
			}
		}
	}
	
	if (*iaini == 1 || *iafinal == V-1) {
		
		*modia *= -1;
		
	}

	// ball

	if (*gol == 0)
	{

		*px += (*modx);
		*py += (*mody);

		

		*iaini += (*modia);
		*iafinal += (*modia);


		if (kbhit() == 1)
		{

			key = getch();

			if (key == 'w')
			{

				if (*jini != 1)
				{

					*jini -= 1;
					*jfinal -= 1;
				}
			}

			if (key == 's')
			{

				if (*jini != V - 5)
				{

					*jini += 1;
					*jfinal += 1;
				}
			}
		}
	}
}

void update(char map[V][H], int jini, int jfinal, int iaini, int iafinal, int py, int px)
{

	edge(map);
	player(map, jini, jfinal);
	cpu(map, iaini, iafinal);
	ball(map, px, py);
}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
