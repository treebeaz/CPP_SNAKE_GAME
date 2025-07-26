#include "fsm.h"

/**
 * @brief Конечный автомат.
 *
 * Функция реализует логику конечного автомата для управления состоянием игры.
 * В зависимости от текущего действия (control_action) и состояния нажатой
 * кнопки (button_pressed) реализуются соответствующие функции для обработки
 * различных состояний игры.
 */

void fsm() {
  TetrisGame_t* tetris_game = getCurrentGameInfo();

  switch (tetris_game->control_action) {
    case START:
      if (tetris_game->button_pressed) {
        fsmStart(tetris_game);
      }
      break;
    case SPAWN:
      fsmSpawnFigure(tetris_game);
      break;
    case MOVING:
      if (tetris_game->button_pressed) {
        fsmMoving(tetris_game);
      }
      checkDelay(tetris_game);
      break;
    case SHIFTING:
      fsmShifting(tetris_game);
      break;
    case ATTACHING:
      fsmAttaching(tetris_game);
      break;
    case PAUSE:
      if (tetris_game->button_pressed) {
        fsmPause(tetris_game);
      }
      break;
    case GAMEOVER:
      if (tetris_game->button_pressed) {
        fsmGameOver(tetris_game);
      }
      break;
    default:
      break;
  }

  tetris_game->button_pressed = false;
}

/**
 * @brief Старт игры.
 *
 * В зависимости от нажатой кнопки пользователем, функция обрабатывает состояние
 * конечного автомата. Если нажата клавиша, отвечающая за начало игры ('s'), то
 * происходит запуск игры и автомат переходит к состоянию созданию фигуры
 * (SPAWN). В противном случае, игра завершается и сессия закрывается.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 */

void fsmStart(TetrisGame_t* tetris_game) {
  switch (tetris_game->keyboard_button) {
    case Start:
      startGame(tetris_game);
      tetris_game->control_action = SPAWN;
      break;
    case Terminate:
      tetris_game->game_info.terminate = 1;
      exitGame(tetris_game);
      break;
    default:
      break;
  }
}

/**
 * @brief Создание фигуры.
 *
 * Функция обрабатывает состояние конечного автомата, отвечающее за создание
 * фигуры. Вызывается функция для генерации новой фигуры и автомат переходит в
 * состояние движения (Moving).
 *
 * @param tetris_game  Указатель на текущую структуру 'TetrisGame_t'.
 */

void fsmSpawnFigure(TetrisGame_t* tetris_game) {
  spawnFigure(tetris_game);
  tetris_game->control_action = MOVING;
}

/**
 * @brief Движение фигуры.
 *
 * В зависимости от нажатой клавиши пользователем, выполняется движение фигуры
 * влево, вправо, вниз. Если фигура достигла самого низа поля, то автомат
 * переходит в состояние соприкосновения фигуры с полем (ATTACHING). Если нажата
 * клавиша 'w' (отвечающая за Action), то автомат переходит в состояние поворота
 * фигуры (SHIFTING). Если нажата клавиша паузы ('p'), то автомат переходит в
 * состояние паузы (PAUSE). Если нажата клавиша окончания игры ('q'), то игра
 * завершается, сессия закрывается.
 *
 *
 * @param tetris_game  Указатель на текущую структуру 'TetrisGame_t'.
 */

void fsmMoving(TetrisGame_t* tetris_game) {
  switch (tetris_game->keyboard_button) {
    case Left:
      moveLeft(tetris_game);
      break;
    case Right:
      moveRight(tetris_game);
      break;
    case Down:
      if (moveDown(tetris_game) == false) {
        tetris_game->control_action = ATTACHING;
      }
      break;
    case Up:
      break;
    case Action:
      tetris_game->control_action = SHIFTING;
      break;
    case Pause:
      tetris_game->game_info.pause = !tetris_game->game_info.pause;
      tetris_game->control_action = PAUSE;
      break;
    case Terminate:
      tetris_game->game_info.terminate = 1;
      exitGame(tetris_game);
      break;
    default:
      break;
  }
}

/**
 * @brief Состояние паузы.
 *
 * Эта функция управляет состоянием паузы игры.
 * Если нажата клавиша паузы ('p'), игра ставится или наоборот, снимается с
 * паузы. Если нажата клавиша окончания игры ('q'), игра завершается, сессия
 * закрывается.
 *
 * @param tetris_game  Указатель на текущую структуру 'TetrisGame_t'.
 */

void fsmPause(TetrisGame_t* tetris_game) {
  switch (tetris_game->keyboard_button) {
    case Pause:
      tetris_game->game_info.pause = !tetris_game->game_info.pause;
      tetris_game->control_action = MOVING;
      break;
    case Terminate:
      tetris_game->game_info.terminate = 1;
      exitGame(tetris_game);
    default:
      break;
  }
}

/**
 * @brief Вращение фигуры.
 *
 * Вызывается главная функция, отвечающая за вращение фигуры.
 * Далее автомат переходит в состояние движения.
 *
 * @param tetris_game  Указатель на текущую структуру 'TetrisGame_t'.
 */

void fsmShifting(TetrisGame_t* tetris_game) {
  rotationFigure(tetris_game);
  tetris_game->control_action = MOVING;
}

/**
 * @brief Соприкосновение фигуры с полем.
 *
 * Функция обновляет игровое поле после того, как фигура прикрепилась к нему.
 * Если игра завершена, автомат переходит в состояние завершения игры
 * (GAMEOVER), также поднимается флаг, указывающий на завершение игры. В
 * противном случае, автомат переходит в состояние создания фигуры (SPAWN).
 *
 * @param tetris_game  Указатель на текущую структуру 'TetrisGame_t'.
 */

void fsmAttaching(TetrisGame_t* tetris_game) {
  refreshField(tetris_game);
  if (checkGameOver(tetris_game)) {
    tetris_game->control_action = GAMEOVER;
    tetris_game->game_info.pause = 2;
  } else {
    tetris_game->control_action = SPAWN;
  }
}

/**
 * @brief Конец игры.
 *
 * У пользователя есть два варианта, либо закончить игру, либо начать все
 * сначала. Если нажата клавиша, отвечающая за начало игры ('s'), то игра
 * полностью начинается сначала и автомат переходит в состояние создания фигуры
 * (SPAWN). Если нажата клавиша окончания игры ('q'), то игра завершается,
 * сессия закрывается.
 *
 * @param tetris_game  Указатель на текущую структуру 'TetrisGame_t'.
 */

void fsmGameOver(TetrisGame_t* tetris_game) {
  switch (tetris_game->keyboard_button) {
    case Start:
      restartGame(tetris_game);
      tetris_game->control_action = SPAWN;
      break;
    case Terminate:
      tetris_game->game_info.terminate = 1;
      exitGame(tetris_game);
      break;
    default:
      break;
  }
}