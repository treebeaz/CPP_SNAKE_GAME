#ifndef FSM_H
#define FSM_H

#include "tetris_backend.h"
#include "tetris_lib.h"

void fsmStart(TetrisGame_t* tetris_game);
void fsmSpawnFigure(TetrisGame_t* tetris_game);
void fsmMoving(TetrisGame_t* tetris_game);
void fsmShifting(TetrisGame_t* tetris_game);
void fsmAttaching(TetrisGame_t* tetris_game);
void fsmGameOver(TetrisGame_t* tetris_game);

#endif