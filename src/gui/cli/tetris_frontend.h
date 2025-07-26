#include <ncurses.h>

#include "../../brick_game/tetris/tetris_lib.h"

void DefaultSetting();

void DrawBoard();
void FillWindow(GameInfo_t game_info);
void DrawField(GameInfo_t game_info);
void DrawFigure();

void GetStatistics(GameInfo_t game_info);
void GetIstructions();

void GetPause();

void GameOver(GameInfo_t game_info);

void CloseWindow();