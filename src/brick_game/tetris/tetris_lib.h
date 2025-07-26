#ifndef TETRIS_LIB_H
#define TETRIS_LIB_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define HEIGHT 20
#define WIDTH 10
#define SIZE_FIGURE 4
#define START_POSITION_Y 1
#define START_POSITION_X 4

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef enum {
  START,
  PAUSE,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER
} ControlAction_t;

typedef struct {
  int **field;
  int **next;
  int start_game;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  int terminate;
} GameInfo_t;

typedef struct {
  int x;
  int y;
  int shape[SIZE_FIGURE][SIZE_FIGURE];
  char type_figure;
  char next_type_figure;
} Figure_t;

typedef struct {
  GameInfo_t game_info;
  UserAction_t user_action;
  ControlAction_t control_action;
  Figure_t figure;

  int keyboard_button;
  bool button_pressed;

  time_t start;
  double delay;

} TetrisGame_t;

void userInput(UserAction_t action, bool hold);
TetrisGame_t *getCurrentGameInfo();
GameInfo_t updateCurrentState();
void fsm();

#endif