#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include "tetris_lib.h"

void startGame(TetrisGame_t* tetris_game);
void allocMemory(int*** matrix, int rows, int cols);
void fillMatrix(int** matrix, int rows, int cols);
void exitGame(TetrisGame_t* tetris_game);
void deallocMemory(int*** matrix, int size);
void createNextFigure(TetrisGame_t* tetris_game);
void fillingShapeType(TetrisGame_t* tetris_game, int type_figure);
void copyFigure(TetrisGame_t* tetris_game, int temp_figure[4][4]);
void spawnFigure(TetrisGame_t* tetris_game);
void moveLeft(TetrisGame_t* tetris_game);
void moveRight(TetrisGame_t* tetris_game);
bool checkCollision(TetrisGame_t* tetris_game);
bool moveDown(TetrisGame_t* tetris_game);
void fsmPause(TetrisGame_t* tetris_game);
void rotationFigure(TetrisGame_t* tetris_game);
Figure_t RotateFigure(Figure_t figure);
Figure_t RotateFigureStick(Figure_t figure);
void correctFigurePosition(Figure_t* figure);
bool canRotateFigure(TetrisGame_t* tetris_game, Figure_t* rotated_figure);
void refreshField(TetrisGame_t* tetris_game);
void checkLines(TetrisGame_t* tetris_game);
int countScore(int remote_lines);
long long currentTimeInMs();
void delayCounting(TetrisGame_t* tetris_game);
void checkDelay(TetrisGame_t* tetris_game);
void writeHighScore(TetrisGame_t* tetris_game);
void readHighScore(TetrisGame_t* tetris_game);
void restartGame(TetrisGame_t* tetris_game);
bool checkGameOver(TetrisGame_t* tetris_game);

#endif