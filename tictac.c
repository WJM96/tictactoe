#include <ncurses.h>
static char grid[9];
static int cursor = 4;
#define PLAYERX 'x'
#define PLAYERO 'o'
#define NOWIN 0x0

void doMove(char player)
{
	
	int key = 0;
	int done = 0;
	do
	{
		key = getch();
		switch(key)
		{
		case KEY_UP: cursor = (cursor-3);  break;
		case KEY_DOWN: cursor = (cursor+3) % 9;  break;
		case KEY_LEFT: cursor = (cursor-1); break;
		case KEY_RIGHT: cursor = ++cursor % 9; break;
		case ' ': done = 1; break;
		default: ; break;
		}
		if(cursor < 0)
			cursor = 9 + cursor;
		move((cursor / 3)*2 + 2, (cursor % 3)*4 + 3);
	
		refresh();
	}while(!done || grid[cursor] != ' ');

	grid[cursor] = player;

}

//returns the char of the winning player at the moment
//returns NOWIN if no winner exists yet
char checkWinner()
{
	char winner;
	//check the horizontals
	for(int x = 0; x < 3; x+= 3)
	{
		if(grid[x] == grid[x + 1] && grid[x] == grid[x + 2])
			winner = grid[x];
	}
	//now the verticals
	for(int y = 0; y < 3; y+= 1)
	{
		if(grid[y] == grid[y + 3] && grid[y] == grid[y + 6])
			winner = grid[y];
	}

	//now the diaganols
	if(grid[0] == grid[4] && grid[0] == grid[8])
		winner = grid[0];
	if(grid[6] == grid[4] && grid[6] == grid[2])
		winner = grid[6];




	if(winner != PLAYERX && winner != PLAYERO)
		return NOWIN;
	return winner;
}


void drawGrid(char curPlayer)
{
	clear();
	printw("%c| 1 | 2 | 3\n", curPlayer);
	printw("-------------\n");
	printw("1| %c | %c | %c\n", grid[0], grid[1], grid[2]);
	printw("-------------\n");
	printw("2| %c | %c | %c\n", grid[3], grid[4], grid[5]);
	printw("-------------\n");
	printw("3| %c | %c | %c\n", grid[6], grid[7], grid[8]);
	move((cursor / 3)*2 + 2, (cursor % 3)*4 + 3);
	refresh();
}


void init()
{
	//so lazy
	for(int i = 0; i < 9; i++)
	{
		grid[i] = ' ';
	}
}


int main(int argc, char* argv)
{
	//set up ncurses
	initscr();
	noecho();
	keypad(stdscr, 1);
	curs_set(1);

	int playing = 1;
	while(playing)
	{
		init();

		drawGrid(PLAYERX);
		char players[2] = {PLAYERX, PLAYERO};
		int curPlayer = 0;
		char winner = NOWIN;
		for(int i = 0; i < 9 && winner == NOWIN; i++)
		{
			doMove(players[curPlayer]);
			
			drawGrid(players[curPlayer]);
			curPlayer = ++curPlayer%2;

			winner = checkWinner();
			
		}
		clear();	
		if(winner == NOWIN)
		{
			printw("No winner -- cats game\n");
		}
		else
		{
			printw("%c has won!! gg.\n", winner);
		}

	
		printw("\nagain?(y to replay) \n");
		int again = getch();

		if(again != 'y')
			playing = 0;
	}
	//leave ncurses
	endwin();

	return 0;
}




