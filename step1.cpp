#include <ncurses.h>
#include <iostream>
#include <vector>
#include <cstring>
#include "map.h"

using namespace std;
int map[30][60];
int countW;
class Vector
{
	public:
		Vector() : x(0), y(0){}
		Vector(int x, int y) {setX(x); setY(y);}

		void setX(int x){this->x = x;}
		int getX() const {return this->x;}
		void setY(int y){this->y = y;}
		int getY() const {return this->y;}

		Vector& operator=(const Vector& vec)
		{setX(vec.getX()); setY(vec.getY()); return *this;}
		Vector& operator+=(const Vector& vec)
		{setX(getX()+vec.getX()); setY(getY()+vec.getY()); return *this;}
		Vector operator+(const Vector& vec)
		{Vector v(getX(),getY()); return v+=vec; }
		Vector& operator-=(const Vector& vec)
		{setX(getX()-vec.getX()); setY(getY()-vec.getY()); return *this;}
		Vector operator-(const Vector& vec)
		{Vector v(getX(),getY()); return v-=vec;}
		bool operator==(const Vector& vec)
		{return getX()==vec.getX() && getY()==vec.getY();}
		bool operator!=(const Vector& vec)
		{return !operator==(vec);}

	private:
		int x,y;
};

class Snake
{
	private:
		Vector wall1[1000];
		const int height;
		const int width;
		bool exit;
		int speed;
		char* table;

	public:
		Snake(int _height, int _width)
		: height(_height), width(_width)
  {   init(map);
		check_wall(map);
			table = new char[height*width];
		}
		~Snake()
		{delete [] table;}

    void init(int map[30][60]){
        for(int i=0; i<30; i++){
            for(int j=0; j<60; j++){
                map[i][j] = map1[i][j];
            }
        }
    }
	 void check_wall(int map[30][60]) //벽 위치 벡터배열로 생성
		{

            countW =0;
			for(int i=0; i<30; i++){
                for(int j=0; j<60; j++)
                    if(map[i][j]==1){
                    wall1[countW++] = Vector(j,i);
                }
            }
		}
		char* changeMap(){ // 맵바꾸기
				return getTable(map1);
		}
		int getHeight() {return height;}
		int getWidth() {return width;}
		char* getTable(int map[30][60])
		{
		  memset(table, ' ', height*width);
            for(unsigned int i=0; i<30; i++)
            {
                for(int j=0; j<60; j++){
                    if(map[i][j]==1) table[i*width+j] = '1' ;
                    else if(map[i][j]==2) table[i*width+j] = '2' ;
                    else if (map[i][j]==3) table[i*width+j] = '3';
                }

            }
			return table;
		}
};
void create(WINDOW* game, Snake& snake, char* table, int height, int width){
	werase(game);
	wbkgd(game, COLOR_PAIR(1));
	wattron(game, COLOR_PAIR(1));
	wborder(game, '|','|','-','-','+','+','+','+');


	for(int i=0; i<(height*width); ++i)
	{
		if(table[i]!=' ')
		{

			int y = i/width;
			int x = i-(y*width);
			int d;
			switch(table[i])
			{
				case '1': //map
					mvwprintw(game, 1+y,1+x, "-"); //"█"
					break;
				case '2':
					mvwprintw(game, 1+y,1+x, "+"); //"█"
					break;
				case '3':
					mvwprintw(game, 1+y,1+x, " " );
					break;
		}
	}
	wrefresh(game);
}

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
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
	int coin =0;
  mvprintw(y/2-3,x/2-25,"insert coin to play game.");
	mvprintw(y/2-2,x/2-25,"press button to insert a coin."); refresh();   //default window 테두리 *
	getch();

	WINDOW *game = newwin(y+2, x-38, 0, 0); //height, width, startY, startX
	wbkgd(game, COLOR_PAIR(1));
  	wattron(game, COLOR_PAIR(1));
	wborder(game, '|','|','-','-','+','+','+','+');
	keypad(game, TRUE);
	wrefresh(game);

	WINDOW *score = newwin(y/2 + 1, x-60, 0,x-37);
	wbkgd(score, COLOR_PAIR(1));
  	wattron(score, COLOR_PAIR(1));
	wborder(score, '|','|','-','-','+','+','+','+');
	wrefresh(score);
	nodelay(score, TRUE);

	WINDOW *mission = newwin(y/2 + 1, x-60, y/2 + 1,x-37);
	wbkgd(mission, COLOR_PAIR(1));
  	wattron(mission, COLOR_PAIR(1));
	wborder(mission, '|','|','-','-','+','+','+','+');
	wrefresh(mission);
	nodelay(mission, TRUE);

	Snake snake(y, x-40);
	char *tbl = snake.changeMap();
	create(game, snake, tbl, snake.getHeight(), snake.getWidth());
	getch();
	delwin(game);
	endwin();
	return 0;

}
