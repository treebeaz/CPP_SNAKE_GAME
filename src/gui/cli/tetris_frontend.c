#include "tetris_frontend.h"

/**
 * @brief Установка стандартных параметров для работы с ncurses.
 */

void DefaultSetting() {
  initscr();
  curs_set(0);
  cbreak();
  halfdelay(1);
  keypad(stdscr, TRUE);
  noecho();
}

/**
 * @brief Отображение игровой доски.
 */

void DrawBoard() {
  mvaddch(0, 0, ACS_ULCORNER);
  mvhline(0, 1, ACS_HLINE, WIDTH * 2 + 5);
  mvaddch(0, WIDTH * 2 + 5, ACS_URCORNER);

  mvvline(1, 0, ACS_VLINE, HEIGHT);
  mvvline(1, WIDTH * 2 + 5, ACS_VLINE, HEIGHT);
  mvvline(1, WIDTH + 1, ACS_VLINE, HEIGHT);

  mvaddch(HEIGHT + 1, 0, ACS_LLCORNER);
  mvhline(HEIGHT + 1, 1, ACS_HLINE, WIDTH * 2 + 5);
  mvaddch(HEIGHT + 1, WIDTH * 2 + 5, ACS_LRCORNER);

  mvaddch(0, WIDTH + 1, ACS_TTEE);
  mvaddch(HEIGHT + 1, WIDTH + 1, ACS_BTEE);

  refresh();
}

/**
 * @brief Отображение окна игры.
 *
 * @param game_info объект структуры 'GameInfo_t', содержащая информацию о
 * текущем состоянии игры.
 */

void FillWindow(GameInfo_t game_info) {
  clear();
  if (game_info.start_game == 0 && game_info.field == NULL) {
    GetIstructions();
  } else if (game_info.pause == 1) {
    GetPause();
  } else {
    if (game_info.field != NULL) {
      DrawBoard();
      DrawField(game_info);
      GetStatistics(game_info);
    }
    if (game_info.next != NULL) {
      DrawFigure();
    }
    if (game_info.pause == 2) {  // окно при окончании игры
      GameOver(game_info);
    }
  }

  refresh();
}

/**
 * @brief Отображение игрового поля.
 *
 * @param game_info объект структуры 'GameInfo_t', содержащая информацию о
 * текущем состоянии игры.
 */

void DrawField(GameInfo_t game_info) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (game_info.field[i][j] == 1) mvaddch(i + 1, j + 1, ACS_CKBOARD);
    }
  }
}

/**
 * @brief Отображение фигуры.
 */

void DrawFigure() {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  Figure_t figure = tetris_game->figure;

  for (int i = 0; i < SIZE_FIGURE; ++i) {
    for (int j = 0; j < SIZE_FIGURE; ++j) {
      if (figure.shape[i][j]) {
        mvaddch(figure.y + i, figure.x + j + 1, ACS_CKBOARD);
      }
    }
  }
}

/**
 * @brief Отображение статистики
 *
 * @param game_info объект структуры 'GameInfo_t', содержащая информацию о
 * текущем состоянии игры.
 */

void GetStatistics(GameInfo_t game_info) {
  mvaddstr(2, 13, "Score:");
  mvprintw(2, 20, "%d", game_info.score);

  mvaddstr(4, 13, "Level:");
  mvprintw(4, 20, "%d", game_info.level);

  mvaddstr(6, 13, "Record:");
  mvprintw(6, 20, "%d", game_info.high_score);

  mvaddstr(12, 15, "Next:");
  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      if (game_info.next[i][j] == 1) {
        mvaddch(i + 13, j + 16, ACS_CKBOARD);
      }
    }
  }
}

/**
 * @brief Отображение инструкций перед игрой.
 */

void GetIstructions() {
  mvaddstr(8, 2, "Press s to start");
  mvaddstr(9, 2, "Press q to quit");
  mvaddstr(10, 2, "Press p to pause(unpause)");
  mvaddstr(11, 2, "Press <- to move left");
  mvaddstr(12, 2, "Press -> to move right");
  mvaddstr(13, 2, "Press KEY_DOWN to move down");
}

/**
 * @brief Отображение паузы.
 */

void GetPause() { mvprintw(10, 6, "Game is paused.. Press p to continue"); }

/**
 * @brief Отображение окна при завершении игры.
 *
 * @param game_info объект структуры 'GameInfo_t', содержащая информацию о
 * текущем состоянии игры.
 */

void GameOver(GameInfo_t game_info) {
  clear();
  mvprintw(10, 10, "GAMEOVER!");
  mvaddstr(11, 6, "Your Record:");
  mvprintw(11, 20, "%d", game_info.high_score);
  mvprintw(12, 6, "Press s to restart game");
  mvprintw(13, 6, "Press q to quit ");
}

/**
 * @brief Завершение рабтоы с библиотекой ncurses.
 */

void CloseWindow() { endwin(); }
