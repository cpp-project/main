
#include <ncurses.h>
#include <iostream>
#include <locale.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <ctime>
#include <queue>
#include <vector>
#include <cstdlib>

using namespace std;

class Point {
public:
  int row;
  int col;
  Point(int num1=0, int num2=0);
  Point(const Point& pt);

  Point& operator=(const Point& pt);
  bool operator==(const Point& pt) const;
  bool operator!=(const Point& pt) const;
  bool isInclude(int len, const std::vector<Point> pt) const;
  friend std::ostream& operator<<(std::ostream &os, const Point& pt);
};
class SnakeGame{
  Point first_gate;// 첫번째 게이트의 좌표
  Point second_gate;// 두번째 게이트의 좌표
  int size_map;// map의 한 변의 길이(기본 21x21, 정사각형)
  int len_wall = 3;// map 내부에 생성되는 wall의 길이(기본 3)
  int count_wall = 3;// map 내부에 생성되는 wall의 개수(기본 3)
  // int height = w.ws_row;
  // int width = w.ws_col;
  int height_win;
  int width_win;
  int **map;
  bool len_mission, grow_mission, poison_mission, gate_mission;
  int snake_speed = 500*1000;// sec: 뱀 속도
  int num_growth=0;// growth item을 먹은 개수
  int num_poison=0;// poison item을 먹은 개수
  int num_gate=0;// gate를 통과한 횟수
  char play_time[3];// 종료 시간 - 시작시간. seconds로 계산한다.
  //int direction = 1; // 키보드 받아서 처리할 예정
  int current_head_row;// 뱀 머리
  int current_head_col;
  int tail_row;
  int tail_col;

  int len_snake = 4;// 뱀 길이
  int max_len = 4;
  std::vector<Point> snake;
  std::queue<Point> items; // poison_item Point  모음
  // vector<int> sna(len_snake);
  WINDOW *win;
  int stage=1;
  bool started = true;
  bool stage_clear = false;
  double total_playtime=0;

public:
char after_game[39]="00000000   0000   000     000  000000";
char after_game2[39]="00        00  00  0 00   00 0  00    ";
char after_game3[39]="00  0000  00  00  0  0   0  0  000000";
char after_game4[39]="00   00   000000  0   0 0   0  00    ";
char after_game5[39]="00000000  00  00  00  000   0  000000";
char after_game6[42] ="  00000  00      000000   0000   000000  ";
char after_game7[42] =" 00      00      00      00  00  00   00 ";
char after_game8[42] ="00       00      000000  00  00  000000  ";
char after_game9[42] =" 00      00      00      000000  00   00 ";
char after_game10[42] ="  00000  000000  000000  00  00  00    00";
char after_game11[18] = "total_play_time: ";
char after_game12[3];

  SnakeGame(WINDOW *w1, int h, int w);
  void make_snake();
  void snake_move(int dir, int *pre);
  void dash_move(int dir, int *pre);
  void goHead(int dir, int row, int col, int c=0);
  bool map_include(int row, int col);
  void make_stage();
  /*
  void meet_item(int row, int col, int dir);
  void make_item();
  void delete_item();
  void next_stage();
  int get_stage();
  void make_gate();
  void set_speed(double sec);
  int get_speed();
  void score_show(WINDOW *w);
  void mission_show(WINDOW *w);
  Point search_output(Point output_gate, int *pre);
  void meet_gate(int row, int col, int dir, int *pre);
  void set_playtime(double r);
  void set_totalplaytime(double r);
  double get_totalplaytime();
  bool get_stage_clear();
  bool get_started();
  void inc_stage();
  void game_end();
  */
};

SnakeGame::SnakeGame(WINDOW *w1, int h, int w){
  win = w1;
  height_win = h;
  width_win = w;
  len_mission= false; grow_mission= false; poison_mission = false; gate_mission=false;
  num_growth = 0; num_poison=0; len_snake=4;num_gate=0; stage_clear = false;

  make_stage();
  make_snake();
}
void SnakeGame::make_snake(){
    snake.clear();
    if(map[height_win/2][width_win/2 - 2] == 1||map[height_win/2][width_win/2 - 1] == 1||map[height_win/2][width_win/2] == 1 || map[height_win/2][width_win/2 + 1] == 1// 벽생성된 곳에 뱀이 생성되지 않도록
      ||map[height_win/2][width_win/2 + 2] == 1 || map[height_win/2][width_win/2 + 3] == 1){
        int i = 0;
        while(map[height_win/2][width_win/2 - 2  - i] == 1||map[height_win/2][width_win/2 - 1 - i] == 1||map[height_win/2][width_win/2 - i] == 1 || map[height_win/2][width_win/2 + 1 - i] == 1
          ||map[height_win/2][width_win/2 + 2- i] == 1 || map[height_win/2][width_win/2 + 3 - i] == 1){
            i++;
          }

        // snake.push_back(Point(height_win/2,width_win/2-i));
        // snake.push_back(Point(height_win/2, width_win/2+1-i));
        // snake.push_back(Point(height_win/2, width_win/2+2-i));
        // snake.push_back(Point(height_win/2, width_win/2+3-i));
        for(int k=0; k<4; k++){
           snake.push_back(Point(height_win/2,width_win/2+k-i));
        }

      }
      else{
        for(int k=0; k<4; k++){
           snake.push_back(Point(height_win/2,width_win/2+k));
        }
      }
      current_head_row = snake[0].row;
      current_head_col = snake[0].col;
      tail_row = snake[len_snake-1].row; //row
      tail_col = snake[len_snake-1].row;

      init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
      init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
      init_pair(3,COLOR_WHITE,COLOR_RED);//미션
      init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
      init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
      init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
      init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate

      wattron(win, COLOR_PAIR(4));


      for (int i=0;i<len_snake;i++){
        if (i==0){
          mvwprintw(win, snake[i].row, snake[i].col, "O");
        }
        else{
          mvwprintw(win, snake[i].row,snake[i].col,"o");
        }
      }
      /*
      while (!items.empty()){
        items.pop();
      }
      */
      //make_item();
      wrefresh(win);

}//end make_snake
void SnakeGame::snake_move(int dir,int *pre){
  int crow = snake[0].row;
  int ccol = snake[0].col;
  /*
  if(map[crow][ccol] == 9){
    meet_gate(crow, ccol, dir, pre);
  }

  else if (map[crow][ccol] == 5 ||map[crow][ccol]==6){
    meet_item(crow,ccol, dir);
  }
  else{
    goHead(dir, current_head_row, current_head_col);
  }
*/
  goHead(dir, current_head_row, current_head_col);
  Point *tmp = new Point[len_snake-1];
  tail_row = snake[len_snake-1].row;
  tail_col = snake[len_snake-1].col;

  for(int i=0; i<len_snake-1; i++){
      tmp[i] = snake[i];
  }

  snake[0] = Point(current_head_row, current_head_col);

  for(int i=1; i<len_snake; i++){
      snake[i] = tmp[i-1];
  }

  len_snake = snake.size();
  // init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
  // init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
  // init_pair(3,COLOR_WHITE,COLOR_RED);//미션
  init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
  // init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
  // init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
  // init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate
  wattron(win, COLOR_PAIR(4));

  for (int i=0;i<len_snake;i++){
    if (map[snake[i].row][snake[i].col]==0){
      if (i==0){
        mvwprintw(win,snake[i].row,snake[i].col,"O");
      }
      else{
        mvwprintw(win,snake[i].row,snake[i].col,"o");
      }
    }
  }

  if(map[tail_row][tail_col] != 9){
    mvwprintw(win,tail_row,tail_col," ");
  }
  wattroff(win,COLOR_PAIR(4));

  wrefresh(win);
}// end snake move
void SnakeGame::dash_move(int dir, int *pre){
  for (int i=0;i<5;i++){
    if(1>current_head_row||current_head_row>height_win-2||1>current_head_col||current_head_col>width_win-2
    || map[current_head_row][current_head_col]==1)
      break;
    else
      snake_move(dir, pre);
  }
}
void SnakeGame::goHead(int dir, int row, int col, int c){
  if (c==0){

    if (dir == 1){ //위쪽
      current_head_row = row-1;
    }
    if (dir == 2){ //아래
      current_head_row = row+1;
    }
    if (dir == 3){ //오른쪽
      current_head_col = col+1;
    }
    if (dir == 4){ //왼쪽
      current_head_col = col-1;
    }
  }
  else{
    current_head_row = row;
    current_head_col = col;
  }
}
void SnakeGame::make_stage(){

  map = new int*[height_win];
  for (int i=0;i<height_win;i++){
    map[i] = new int[width_win];
  }//벽 크기 생성(화면 높이 * 화면 너비)

  for(int i=0; i<height_win; i++){
    for(int j = 0; j<width_win; j++){
      if((i == 0 && (j == 0|| j == width_win -1))
      ||(i == height_win -1 &&(j == 0 || j == width_win -1))){
        map[i][j] = 2;//4개의 꼭짓점은 2
      }
      else if(i == 0 || i == height_win - 1){
        map[i][j] = 1;
      }
      else if(j == 0|| j == width_win -1){
        map[i][j] = 1;
      }
      else{
        map[i][j]=0;
      }

    }
  }//map 값
  // init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
  init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
  // init_pair(3,COLOR_WHITE,COLOR_RED);//미션
  // init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
  init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
  init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
  // init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate
  wattron(win, COLOR_PAIR(2));

  for(int i = 0; i<height_win; i++){
    for(int j = 0; j<width_win; j++){
      if(map[i][j] == 2){
        wattron(win, COLOR_PAIR(6));
        mvwprintw(win, i, j, "#");
      }
      else if(map[i][j] == 1){
        wattron(win, COLOR_PAIR(5));
        mvwprintw(win, i, j, "#");
      }
      else{
        mvwprintw(win, i, j, " ");
      }
    }
  }//화면에 표시
  wrefresh(win);
  getch();

  int numrow;
  int numcol;
  srand(static_cast<unsigned int>(time(NULL)));
  int plus = 1;

  for (int i=0;i<count_wall;++i){
        int num1 = rand();
        int random1 = (int)num1%height_win; // num1은 0~width_win-1까지의 난수
        int num2 = rand();
        int random2 = (int)num2%width_win;

        if(random1 <= height_win - (len_wall + 2) && 2 <= random1 && random1 != height_win/2
          &&random2 <= width_win  - (len_wall + 2) && 2 <= random2
          &&map_include(random1, random2)){
            numrow = random1; numcol = random2;
            for (int k=0;k<count_wall;k++){
              if (map[numrow][numcol] == 0){
               for(int h = 0; h < len_wall; h++){
                 wattron(win, COLOR_PAIR(5));
                 if(plus < 0){
                   mvwprintw(win,numrow +h,numcol,"#");
                   map[numrow+h][numcol] = 1;
                 }else{
                   mvwprintw(win,numrow,numcol+h,"#");
                   map[numrow][numcol +h] = 1;
                 }

                }
                plus *= -1;
             }

           }
        }else{
          i--;
        }

      }


 //교차하는 벽생성시 교차부분은 ImmuneWall로 바꿈///
  for(int i = 1; i<height_win - 1; i++){
    for(int j = 1; j<width_win - 1; j++){
      if(map[i][j] == 1){
        if(map[i - 1][j] == 1 &&map[i][j - 1] == 1
          && map[i][j + 1] == 1 && map[i + 1][j] == 1){
            wattron(win, COLOR_PAIR(6));
            mvwprintw(win,i, j,"#");
            map[i][j] == 2;
        }
      }
    }
  }

  wrefresh(win);

}

bool SnakeGame::map_include(int row, int col){
  for (int i=0; i<len_wall; i++){
    if(map[row + i][col] == 1){
      return false;
    }
    if(map[row][col + i] == 1){
      return false;
    }
  }
  return true;
}// end map_include

Point::Point(int num1, int num2): row(num1),col(num2){
  // row=num1;
  // col=num2;
}
Point::Point(const Point& pt){
  row=pt.row;
  col=pt.col;
}

Point& Point::operator=(const Point& pt){
  this->row = pt.row;
  this->col = pt.col;
  return *this;
}

bool Point::operator==(const Point& pt) const{
  return (row==pt.row)&&(col==pt.col);
}
bool Point::operator!=(const Point& pt) const{
  return !(*this==pt);
}
bool Point::isInclude(int len, const vector<Point> pt) const{
 for (int i=0;i<len;i++){
   if (*this == pt[i]){
     return true;
   }
 }
 return false;
}
ostream& operator<<(ostream &os, const Point& pt){
  os << "(" << pt.row << "," << pt.col << ")";
  return os;
}

int main(){

  setlocale(LC_ALL, "ko_KR.utf8");
  setlocale(LC_CTYPE, "ko_KR.utf8");//한글 출력 설정
  ///////////////
  // struct winsize w;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  char key;
  char userName[8];
  WINDOW *win1;
  WINDOW *win2;
  WINDOW *win3;

  int height = w.ws_row;
  int width = w.ws_col;
  int height_win = w.ws_row-10;
  int width_win = w.ws_col/2;

  initscr();//curse 모드 시작
  keypad(stdscr, TRUE);
  curs_set(0);
  cbreak();
  echo();

  printw("User name: ");
  scanw("%s", userName);
  //noecho();
  clear(); //색 입히려면 지워야하나봄,,,
  noecho();
  start_color();
  init_color(COLOR_RED, 230*4, 100*4, 90*4);
  init_color(COLOR_CYAN, 70*4, 110*4, 110*4);
  init_color(COLOR_BLUE, 9*4, 100*4, 50*4);

  init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
  init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
  init_pair(3,COLOR_WHITE,COLOR_RED);//미션
  init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
  init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
  init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
  init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate

  init_color(COLOR_BLACK, 90*4, 120*4, 80*4);

// //숫자, 폰트 색, 폰트
  border('|','|','-','-','|','|','|','|');
  mvprintw(1,1, "SnakeGame");
  //mvprintw(2,1,"User : ");
  //mvprintw(2,2,userName);
  mvprintw(2,1,"User : ");
  mvprintw(2,8,userName);
  refresh();
  getch();
  noecho();
/////////////////////////////////////////
//게임 윈도우
//터미널 크기

  win1= newwin(height_win,width_win,3,3);
  wbkgd(win1,COLOR_PAIR(2));
  wattron(win1, COLOR_PAIR(2));
  SnakeGame s1 = SnakeGame(win1,height_win,width_win);
  //wborder(win1, '@','@','@','@','@','@','@','@');
  wrefresh(win1);
  //refresh(); //뭔지 모르겠
  //getch();
  noecho();

  win2 = newwin(height/3, width/3, 3, width/2 + 10);
  wbkgd(win2,COLOR_PAIR(1));
  wattron(win2, COLOR_PAIR(1));
  wborder(win2,'|','|','-','-','|','|','|','|');
  wrefresh(win2);
  //getch();
  noecho();

  win3 = newwin(height/3,width/3, height/2, width/2+10);
  wbkgd(win3,COLOR_PAIR(3));
  wattron(win3, COLOR_PAIR(3));
  wborder(win3,'|','|','-','-','|','|','|','|');
  wrefresh(win3);

  ////////////////
  int a = 4; //s1.get_stage()<=4
  while(a>=0){
    int pre = 4;
    getch();
    //s1.make_gate();
    wrefresh(win1);
    //s1.mission_show(win3);
    wrefresh(win3);
    //s1.score_show(win2);
    wrefresh(win2);
    getch();
    nodelay(stdscr, TRUE);

    //시간측정
    time_t start, end;
    double result;
    start = time(NULL);
    time_t first,mid;
    first = time(NULL);

    //본격적인 게임 시작
    while (true){
      int input;
      //int pre = 4;
      //mid = time(NULL);
      //s1.mission_show(win3);
      //wrefresh(win3);
      //double t = mid-start;
      //s1.score_show(win2);
      wrefresh(win2);

      /*5초 지나면 아이템 옮겨가도록 설정
      if (mid-first>=5.0){
        //s1.delete_item();
        //s1.make_item();
        first = time(NULL);
      }
*/
      input  = getch();
      switch (input){
        case KEY_LEFT:
          s1.snake_move(4,&pre);
          break;
        case KEY_RIGHT:
          s1.snake_move(3,&pre);
          break;
        case KEY_DOWN:
          s1.snake_move(2,&pre);
          break;
        case KEY_UP:
          s1.snake_move(1,&pre);
          break;
        case ' ':
          s1.dash_move(pre, &pre);
          break;
        case ERR:
          s1.snake_move(pre,&pre);
          break;
        }

      if(input == KEY_LEFT){
        pre = 4;
      }else if(input == KEY_RIGHT){
        pre = 3;
      }else if(input == KEY_DOWN){
        pre = 2;
      }else if(input == KEY_UP){
        pre = 1;
      }

      a--;
      //int snake_speed = 1000*1000;
      //usleep(snake_speed);
      //s1.game_end();
      wrefresh(win1);
      int snake_speed = 1000*1000;
      usleep(snake_speed);

    }
    //end = time(NULL);
    //result = (double)(end - start); // result == 플레이 타임
    //s1.set_playtime(result);

    nodelay(stdscr, false);

    //s1.score_show(win2);
    wrefresh(win2);
    getch();
  }
  ///////////////
  getch();
  noecho();
  delwin(win1);
  delwin(win2);
  delwin(win3);
  endwin();//curses 모드

  return 0;

}
