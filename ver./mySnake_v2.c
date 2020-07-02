#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 79
#define HEIGHT 23

bool gameOver;

int headX = WIDTH/4, headY = HEIGHT/2, score;

int foodX[5], foodY[5], foodValue[5], tailsX[1817], tailsY[1817], tailSize;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN} dir;

int knbit(void) {
  int ch = getch();
  if (ch != ERR) {
    ungetch(ch);
    return 1;
  } else return 0;
}

void rules() {
  tailsX[0] = headX;
  tailsY[0] = headY;
  int lastX = tailsX[0], lastY = tailsY[0];
  int tempX, tempY;
  for (int i = 1; i < tailSize*2; i++) {
    tempX = tailsX[i];
    tempY = tailsY[i];
    tailsX[i] = lastX;
    tailsY[i] = lastY;
    lastX = tempX;
    lastY = tempY; 
  } 
  switch (dir) {
    case LEFT:
      headX--;
      break;
    case RIGHT:
      headX++;
      break;
    case UP:
      headY--;
      break;
    case DOWN:
      headY++;
      break;
  }
  if (headX >= WIDTH+1) headX = 0;
  else if (headX < 0) headX = WIDTH;
  else if (headY >= HEIGHT+1) headY = 0;
  else if (headY < 0) headY = HEIGHT;
  for (int i = 0; i < tailSize; i++) if (tailsX[i] == headX && tailsY[i] == headY) gameOver = true;
  for (int i = 0; i < 5; i++) {
    if (headX == foodX[i] && headY == foodY[i]) {
      score += foodValue[i]; 
      srand(time(0));
      foodX[i] = (rand() % WIDTH) + 1;
      foodY[i] = (rand() % HEIGHT) + 1;
      foodValue[i] = (rand() % 10) + 1;
      tailSize++;
      if (score == 2) tailSize++;
    }
  }
}

int main() {
  dir = STOP;
  gameOver = false;
  for (int i = 0; i < 5; i++) {
    foodX[i] = (rand() % WIDTH) + 1;
    foodY[i] = (rand() % HEIGHT) + 1;
    foodValue[i] = (rand() % 10) + 1;
  }
  score = 0; 
  initscr();
  curs_set(0);
  noecho();
  halfdelay(1);
  while (!gameOver) {
    mvprintw(0,0,"score: %d", score);
    mvprintw(headY, headX, "@");
    for (int i = 0; i < 5; i++) mvprintw(foodY[i], foodX[i], "A");
    for (int i = 0; i < tailSize; i++) mvprintw(tailsY[i], tailsX[i], "s");
    if (knbit()) { 
      int ch = getch();
      if (ch == 'a') dir = LEFT;
      if (ch == 'd') dir = RIGHT;
      if (ch == 'w') dir = UP;
      if (ch == 's') dir = DOWN;
      if (ch == 'e') gameOver = true;
      erase();
      refresh();
    } else {
      erase();
      refresh();
    }
    rules(); 
  }
  cbreak();
  mvprintw(HEIGHT/2,WIDTH/3,"Game Over! - Your score: %d", score);
  refresh();
  getch();
  endwin();
  return 0;
}
