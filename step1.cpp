#include <ncurses.h>
#include <iostream>

time_t startGT;
time_t overGT;

using std::fstream; using std::ifstream; using std::ofstream;

int main()
{
	setlocale(LC_ALL, "ko_KR.utf8");
	setlocale(LC_CTYPE, "ko_KR.utf8");//한글 출력 설정

	int sz;
	int x,y;
	x = 100;
	y = 30;
	initscr();
	noecho();

  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	int coin =0;
  mvprintw(y/2-3,x/2-25,"insert coin to play game.");
	mvprintw(y/2-2,x/2-25,"press button to insert a coin."); refresh();   //default window 테두리 *

	getch();

	WINDOW *win = newwin(y+2, x-38, 0, 0); //height, width, startY, startX
	wbkgd(win, COLOR_PAIR(1));
  wattron(win, COLOR_PAIR(1));
	wborder(win, '|','|','-','-','+','+','+','+');
	keypad(win, TRUE);
	wrefresh(win);

	getch();
	delwin(win);
	endwin();
	return 0;
	
}
