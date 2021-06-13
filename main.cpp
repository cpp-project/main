#include <ncurses.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include "map.h"

using namespace std;
int map[30][60];

int count_growth; // growth 갯수
int count_poison; // poison 갯수
int count_gate; // gate 갯수

int max_count; // MAX 도달 횟수getsize
int num_1; // 맵에서 1 갯수

char com_1; //v표시 해줄거
char com_2;
char com_3;
char com_4;

int mission_1; // 미션 설정
int mission_2;
int mission_3;
int mission_4;

time_t nowGameT;
time_t startAT;
time_t nowAT;
time_t startPT;
time_t nowPT;
time_t startgameT;
time_t overgameT;

int itemT = 6;
void printScore(WINDOW*, int, int, int);
void printMission(WINDOW*, int, int);

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
      vector<Vector> body;
      Vector growth; //growth 추가
      Vector poison; //poison 추가
      Vector wall1[1000];
        Vector gate[2];
      Vector direction;
      int points;
      int level;
      int best;
      const int height;
      const int width;
      bool exit;
      int speed;
      char* table;

   public:
      Snake(int _height, int _width)
      : height(_height), width(_width)
        {   init(map);
         growth_item(map);
         poison_item(map);
            check_wall(map);
            get_gate(wall1);
         body.push_back(Vector(width/2, height/2));
            body.push_back(Vector(width/2+1, height/2));
            body.push_back(Vector(width/2+2, height/2));
         set_direction(0);
         exit = false;
         speed = 100000;
         level = 1;
         mission_1 = 1;
         mission_4 = 5;
         mission_2 = 1;
         mission_3 = 1;
         points = 0;
         srand(time(NULL));
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


        void change_map(int next_map[30][60]){ //맵 change
            for(int i=0; i<30; i++){
                for(int j=0; j<60; j++){
                    map[i][j] = next_map[i][j];
                }
            }

            check_wall(next_map);

        }

        void get_gate(Vector wall[]){ //gate 위치 정해주기
            int A = rand()%num_1;
            int B = rand()%num_1;
            if(B == A) get_gate(wall1);
            gate[0] = wall1[A];
            gate[1] = wall1[B];
        }

      void growth_item(int map[30][60]){ // growth 놓기
         startAT = time(NULL);

         int x = rand()%width;
         int y = rand()%height;
         growth = Vector(x,y);
         for(unsigned int i=0;i<body.size();++i)
            if(growth==body[i])growth_item(map);
            if(map[y][x]!=0) growth_item(map);

      }


      void poison_item(int map[30][60]){  // poison 위치 설정
         startPT = time(NULL);

      int x,y;
         x = rand()%width;
         y = rand()%height;
         poison = Vector(x,y);
         for(unsigned int i=0;i<body.size();++i)
            if(poison==body[i])poison_item(map);

            if(map[y][x]!=0) poison_item(map);

      }

      void game_end(){ //3이하로 뱀 크기 떨어지면 죽음
         if(body.size()<3){exit = true;}
      }


    void check_wall(int map[30][60]){ //벽 위치 벡터배열로 생성
         num_1 =0;
         for(int i=0; i<30; i++){
            for(int j=0; j<60; j++)
                if(map[i][j]==1){
                wall1[num_1++] = Vector(j,i);
            }
        }
         }

      void set_direction(int d){ //방향 설정
         switch(d)
         {
            case 0:direction = Vector(0, -1); break;
            case 1:direction = Vector(1, 0); break;
            case 2:direction = Vector(0, 1); break;
            case 3:direction = Vector(-1, 0); break;
         }
      }

      //맵 다음단계 되면 다시 3개로 줄여줌
      void change_body(){

         for(int i=0; i<body.size()-3; i++){
            reduceBodySize(body.size());
         }

      }

      void make_move(){ // 이동
         if(!exit)body[0]+=direction;

         for(unsigned int i = 1; i<body.size(); ++i)
            if(body[i]==body[0]){exit = true; break;}
      }

      void body_move(){
         for(unsigned int i=body.size()-1; i>0; --i)
            increaseBodySize(i);
      }

      void checkForGrowth(){

         nowAT = time(NULL);

         if(nowAT - startAT > itemT){
            growth_item(map);
         }
         if(body[0]==growth)
         {
            count_growth++; //수정
            points++;
            if(points>best)best = points;
            growth_item(map);
            increaseBodySize(body.size());
         }
      }

      void checkForPoison(){ //길이 줄이고 포인트 감소

         nowPT = time(NULL);

         if(nowPT - startPT > itemT){
            poison_item(map);
         }

         if(body[0]==poison)
         {   count_poison++; //수정
            points--;
            poison_item(map);
            reduceBodySize(body.size());
         }
      }

       void checkForwall(){ // 벽에 닿으면 죽음
          for(int i=0; i<num_1; i++){
              if(body[0]==wall1[i] && body[0] != gate[0] && body[0]!= gate[1]){
         exit = true;
                  }

          }
      }

        void check_gate(){ // 몸이 다 통과할때까지 머리 방향 설정
            if(body[0] == gate[0]){
                body[0] = gate[1];
                count_gate++;
                set_direction(gate_direction(gate[1]));
            }

            else if(body[0] == gate[1]){
            count_gate++;
                body[0] = gate[0];
                set_direction(gate_direction(gate[0]));
            }

        }

      void change_gate(){ // 다 통과해야 게이트 바뀌도록
         Vector A = direction;
         for(int i=0; i<body.size(); i++){
            A = A + direction;

         }
            if(body[0] == gate[0]+A){
            get_gate(wall1);
            }

            else if(body[0] == gate[1]+A){
            get_gate(wall1);
            }

    }


        int gate_direction(Vector gate){ //벽 없는 쪽으로 머리 방향 설정
            Vector A = gate + Vector(0,-1) ;
            if(map[A.getY()][A.getX()]==0) return 0;
            Vector B = gate + Vector(1,0) ;
            if(map[B.getY()][B.getX()]==0) return 1;
            Vector C = gate + Vector(0,1) ;
            if(map[C.getY()][C.getX()]==0) return 2;
            Vector D = gate + Vector(-1,0) ;
            if(map[D.getY()][D.getX()]==0) return 3;

            return -1;
        }

      void mission(){ // 미션 완료시 v표시
         if(mission_1 <= count_growth){
            com_1 = 'v';
         }
         else com_1 = ' ';
         if(mission_2 <= count_poison){

            com_2 = 'v';
         }
         else com_2 = ' ';
         if(mission_3 <= count_gate){

            com_3 = 'v';
         }
         else com_3 = ' ';
         if(mission_4 <= body.size()){

            com_4 = 'v';
         }
         else com_4 = ' ';
      }
      int mission_complete(){
         if(com_2 == 'v' && com_1 == 'v' && com_3 == 'v' && com_4 == 'v' ) {
            level++;
            return level;
         }
         return level;
      }

      void checkPoints(){ // 미션 완료시 맵 바꾸고 미션 스코어 변경
         switch(mission_complete()){
            case 2: {
               mission_1 =2;
               mission_4 =4;
               mission_2 =2;
               mission_3 =2;
               change_map(map2);
               break;}
            case 3: {
               mission_1 =3;
               mission_4 =5;
               mission_2 =3;
               mission_3 =3;
               change_map(map3); break;}
            case 4: {
               mission_1 =4;
               mission_4 =6;
               mission_2 =4;
               mission_3 =4;
               change_map(map4); break;}
            case 5: {
               mission_1 =5;
               mission_4 =7;
               mission_2 =5;
               mission_3 =5;
               change_map(map5); break;}
         }
      }

        char* check_map(){ // 맵바꾸기
            if(level == 1){return get_table(map1);}
            if(level == 2){return get_table(map2);}
         if(level == 3){return get_table(map3);}
            if(level == 4){return get_table(map4);}
         if(level == 5){return get_table(map5);}
            return get_table(map5);
        }

      void increaseBodySize(uint i){
         if(i >= body.size())
            body.push_back(body.back());
         else
            body[i] = body[i - 1];
      }

      void reduceBodySize(uint i){
         if(i >= body.size())
            body.pop_back();
         else
            body[i] = body[i - 1];
      }

      int getBodySize(){return body.size();}
      int get_points() {return points;}
      int get_level() {return level;}
      bool get_exit() {return exit;}
      void set_exit(bool e) {exit = e;}
      int get_speed() {return speed;}
      int get_height() {return height;}
      int get_width() {return width;}
      char* get_table(int map[30][60])
      {
         std::memset(table, ' ', height*width);

         table[body[0].getY()*width+body[0].getX()] = 'h';
         for(unsigned int i=1; i<body.size(); ++i)
            table[body[i].getY()*width+body[i].getX()] = 'b';

         table[growth.getY()*width+growth.getX()] = 'a';
         table[poison.getY()*width+poison.getX()] = 'x';  //poison

            for(unsigned int i=0; i<30; i++)
            {
                for(int j=0; j<60; j++){
                    if(map[i][j]==1) table[i*width+j] = '1' ;
                    else if(map[i][j]==2) table[i*width+j] = '2' ;
                    else if (map[i][j]==3) table[i*width+j] = '3';
                }

            }
             table[gate[0].getY()*width+ gate[0].getX()] = '-';
            table[gate[1].getY()*width+ gate[1].getX()] = '-';

         return table;
      }

      int get_direction(){
         if(direction.getX()==1)
            return 1;
         else if(direction.getX()==-1)
            return 3;
         else if(direction.getY()==-1)
            return 0;
         else
            return 2;
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
				case 'a':
					mvwprintw(game, 1+y,1+x, "*");
					break;
				case 'x':
					mvwprintw(game, 1+y,1+x, "X");
					break;
				case '1':
						mvwprintw(game, 1+y,1+x, "=");
						break;
				case '2':
						mvwprintw(game, 1+y,1+x, "+");
						break;
				case '3':
						mvwprintw(game, 1+y,1+x, " " );
						break;
				case 'G':
					mvwprintw(game, 1+y,1+x, " ");
					break;
				case 'h':
					mvwprintw(game, 1+y,1+x, "O");
					break;
				case 'b':
					mvwprintw(game, 1+y,1+x, "o");
					break;
				}
		}
	}
	wrefresh(game);
}
void printScore(WINDOW* w, int score, int level, int sz)
{
	werase(w);
	wbkgd(w, COLOR_PAIR(1));
    wattron(w, COLOR_PAIR(1));
	wborder(w, '|','|','-','-','+','+','+','+');
	 mvwprintw(w, 1, 2, "스코어 보드");
    mvwprintw(w, 4, 2, "B: 현재 길이 / 최대 길이 %d/%d",sz,mission_4);
    mvwprintw(w, 6, 2, "+: Growth 아이템 개수 %d",count_growth);
    mvwprintw(w, 8, 2, "-: Poison 아이템 개수%d",count_poison);
    mvwprintw(w, 10, 2, "G: 게이트 사용 횟수 %d",count_gate);
	wrefresh(w);
}

void printMission(WINDOW* w, int score, int level)
{
	werase(w);
	wbkgd(w, COLOR_PAIR(1));
    wattron(w, COLOR_PAIR(1));
	wborder(w, '|','|','-','-','+','+','+','+');
    mvwprintw(w, 1, 2, "미션 보드");
    mvwprintw(w, 4, 2, "B: %d   (%c)",mission_4,com_4);
    mvwprintw(w, 5, 2, "+: %d   (%c)",mission_1,com_1);
    mvwprintw(w, 6, 2, "-: %d   (%c)",mission_2,com_2);
    mvwprintw(w, 7, 2, "G: %d   (%c)",mission_3,com_3);
	wrefresh(w);
}

bool writeEndAndGetInput()
{
	overgameT = time(NULL);
	int score = count_growth + max_count - count_poison + count_gate*2;
	WINDOW* endwin = newwin(32,110, 0, 0);
	box(endwin, 0, 0);
	nodelay(endwin, TRUE);
	keypad(endwin, TRUE);
	mvwprintw(endwin, 14, 40, "~게임 오버~");
	mvwprintw(endwin, 15, 40, "게임 점수 : %d", score);
	mvwprintw(endwin, 16, 40, "게임시간 : %d 초", overgameT - startgameT);
	mvwprintw(endwin, 17, 40, "총 점수 : %d", score + 1000 - (overgameT - startgameT));
	mvwprintw(endwin, 18, 40, "[q]");
	int c;
	do{
		c = wgetch(endwin);
	}while(c!=10 && c!=' ' && c!='q' && c!='Q');
	werase(endwin);
	wrefresh(endwin);
	delwin(endwin);
	return (c=='q' || c=='Q')?false:true;
}
void proccesInput(WINDOW* game, Snake& snake, int input)
{
	int d = snake.get_direction();
	switch(input)
	{
		case KEY_UP:
			if(d == 2) {
				snake.set_exit(true);
				break;}
			if(d!=0 && !(d==2 && snake.get_points() > 0))
			snake.set_direction(0);
			break;
		case KEY_DOWN:
			if(d == 0) {
				snake.set_exit(true);
				break;}
			if(d!=2 && !(d==0 && snake.get_points() > 0))
			snake.set_direction(2);
			break;
		case KEY_LEFT:
			if(d == 1) {
				snake.set_exit(true);
				break;}
			if(d!=3 && !(d==1 && snake.get_points() > 0))
			snake.set_direction(3);
			break;
		case KEY_RIGHT:
			if(d == 3) {
				snake.set_exit(true);
				break;}
			if(d!=1 && !(d==3 && snake.get_points() > 0))
			snake.set_direction(1);
			break;
		case 'Q':
		case 'q':
			snake.set_exit(true);
			break;
		case 'P':
		case 'p':
			char c;
			wattron(game, A_BOLD);
			mvwprintw(game, snake.get_height()/2, snake.get_width()/2, "PAUSE");
			wattroff(game, A_BOLD);
			nodelay(game, FALSE);
			do{
				c = wgetch(game);
			}while(c!='p' && c!='P');
			nodelay(game, TRUE);
			break;
	}
}

int main(){
	setlocale(LC_ALL, " ");
	setlocale(LC_CTYPE, "ko_KR.utf8");//한글 출력 설정
	count_growth = 0;
	count_poison = 0;
	count_gate = 0;
	com_1 = ' ';
	com_4 = ' ';
	com_3 = ' ';
	com_2 = ' ';
	int sz;
	int x,y;
	x = 100;
	y = 30;
	initscr();
	noecho();
	cbreak();

	resize_term(1000, 500);
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
	int coin =0;
	mvprintw(y/2-3,x/2-15,"스네이크 게임을 시작합니다!");
	mvprintw(y/2-2,x/2-15,"enter키를 눌러주세요"); refresh();

	getch();
	endwin();

	WINDOW *game = newwin(y+2, x-38, 0, 0); //height, width, startY, startX

	wbkgd(game, COLOR_PAIR(1));
    wattron(game, COLOR_PAIR(1));
	wborder(game, '|','|','-','-','+','+','+','+');

	nodelay(game, TRUE);
	keypad(game, TRUE);

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


	//game loop
	do
	{
		startgameT = time(NULL);
		Snake snake(y, x-40);
		refresh();
		wrefresh(game);
		sz = snake.getBodySize();
		printScore(score, 0, 1,sz);
		printMission(mission,0,1);
		count_growth = 0;
		count_poison = 0;
		count_gate = 0;

		while(!snake.get_exit())
		{
			char *tbl = snake.check_map();
			//process data
			//draw
			create(game, snake, tbl, snake.get_height(), snake.get_width());

			printScore(score, snake.get_points(), snake.get_level(), snake.getBodySize());
			printMission(mission, snake.get_points(), snake.get_level());

			int input = wgetch(game);
			proccesInput(game, snake, input);
			if(snake.get_exit())break;
			snake.checkPoints();
			snake.body_move();
			snake.make_move();
			snake.checkForGrowth();
			snake.checkForPoison();
			snake.checkForwall();
			snake.check_gate();
			snake.change_gate();
			snake.mission();
			snake.game_end(); //몸 길이 3 미만일때 죽음

			#ifdef WIN32
			#else
			usleep(snake.get_speed());
			#endif
		}

	}while(writeEndAndGetInput());

	delwin(score);
	delwin(game);
	delwin(mission);
	endwin();
}
