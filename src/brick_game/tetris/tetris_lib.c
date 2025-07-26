#include "tetris_lib.h"

/**
 * @brief Обработка действий пользователя.
 *
 * @param action Действие пользователя.
 * @param hold Флаг, указывающий на нажатие клавиши.
 */

void userInput(UserAction_t action, bool hold) {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  tetris_game->keyboard_button = action;
  tetris_game->button_pressed = hold;
}

/**
 * @brief Получение информации о текущем состоянии игры.
 *
 * @return Указатель на структуру 'TetrisGame_t'.
 */

TetrisGame_t* getCurrentGameInfo() {
  static TetrisGame_t tetris_game = {0};
  return &tetris_game;
}

/**
 * @brief Получение состояния игры.
 *
 * @return объект 'GameInfo_t', который является полем структуры 'TetrisGame_t'
 */

GameInfo_t updateCurrentState() {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  return tetris_game->game_info;
}