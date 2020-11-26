#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

int X_DIM, Y_DIM;

struct player {
  int score=0;
  int *body;
  signed int x_dir;
  signed int y_dir;  
};

struct food {
  bool eaten=true;
  signed int x_coord;
  signed int y_coord;
};

struct player snake;
struct food apple;

static bool findAppCoords(int x,int y) {
  int j = 0;

  for(int i=1; i<=snake.score; i++) {
    if (snake.body[i*2] == x && snake.body[i*2+1] == y) {
      j++;
    }
  } 

  if (j > 1) {
    return true;
  }
  return false;
}

static void gen_food(void) {
  //mvprintw(0,0,"%d %d", apple.y_coord, apple.x_coord);

  if(apple.eaten==true) {
    int x_coord = 40;
    int y_coord = 40;  

    apple.x_coord = rand() % X_DIM;
    apple.y_coord = rand() % Y_DIM;
    apple.eaten=false;
  }

  mvprintw(apple.y_coord, apple.x_coord, "@");  
}

int check_position(void) {
  if(snake.body[0]  == apple.x_coord && snake.body[1] == apple.y_coord) {
    //eating
    snake.score += 1;
    snake.body = (int *)realloc((void *)snake.body, (snake.score+1)*2*sizeof(int));
    //snake.body[snake.score*2] = snake.body[snake.score*2-2]-1;
    //snake.body[snake.score*2+1] = snake.body[snake.score*2-1]-1;
    apple.eaten=true;
    apple.x_coord = 0;
    apple.y_coord = 0;
    return 0;
  } else if((snake.body[0] < 0 || snake.body[1] < 0) || (snake.body[0] > X_DIM || snake.body[1] > Y_DIM)) {
    return -1;
  }

  for(int i=0; i <=snake.score; i++) {
    if(findAppCoords(snake.body[i*2], snake.body[i*2+1])) {
      return -1;
    }
  }
}


static void update_coords(void) {

  mvprintw(0,0, "%d", snake.score);
 
  int x_new = snake.body[0];
  int y_new = snake.body[1];
  int x_prev;
  int y_prev;
 
  if (snake.score > 0){
    for(int i=1; i <= snake.score; i++){
      x_prev = snake.body[i*2];
      y_prev = snake.body[i*2+1];
      
      snake.body[i*2] = x_new; //snake.body[i*2-2];
      snake.body[i*2+1] = y_new; //snake.body[i*2-1]; 
      x_new = x_prev;
      y_new = y_prev;

      //mvprintw(i,0, "%d %d", snake.body[i*2], snake.body[i*2+1]);
    }
  }
  snake.body[0] += snake.x_dir;
  snake.body[1] += snake.y_dir;
}

static void print_snake(void) {
  for (int i=1; i <=snake.score;i++) {
    mvprintw(snake.body[i*2+1],snake.body[i*2],"#");
  }
  mvprintw(snake.body[1], snake.body[0],"#");
}

int main(void) {
  int c;
  int quit = false;
  
  srand(time(0));

  snake.score = 0;
  snake.body = (int *)malloc(2*sizeof(int));
  //snake.body[0] = X_DIM / 2;
  //snake.body[1] = Y_DIM / 2;   
  snake.x_dir = 0;
  snake.y_dir = 0;


  initscr(); 
  curs_set(0);
  cbreak();
  noecho();
  keypad(stdscr, true);
  nodelay(stdscr, true);

  getmaxyx(stdscr, Y_DIM, X_DIM);

  
  snake.body[0] = X_DIM / 2;
  snake.body[1] = Y_DIM / 2;   // TODO: optimize

  while(!quit) {
    clear();
    //update_coords();
    //print_snake();   

 
    c = getch();
  
    switch(c) {
      case ERR:
        napms(10);
        break;
      case 'q':
        quit = true;
        break;
      case 'l':
        if (snake.x_dir == 0){  
          snake.x_dir = 1;
          snake.y_dir = 0; 
        }
        break;
      case 'h':
        if (snake.x_dir == 0){
          snake.x_dir = -1;
          snake.y_dir = 0;
        }
        break;
      case 'k':
        if (snake.y_dir == 0){
          snake.y_dir = -1;
          snake.x_dir = 0;
        }
        break;
      case 'j':
        if (snake.y_dir == 0) {
          snake.y_dir = 1;
          snake.x_dir = 0;
        }
        break;
      default:
        
        break;
    }


    update_coords(); 
    if(check_position() == -1) {
      mvprintw(Y_DIM/2, X_DIM/2, "GAME OVER");
      break;
    }
    gen_food();
    print_snake();

    refresh();
    napms(150);
  }
  endwin();
  return 0; 

}
