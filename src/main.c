#include "main.h"

/**
 * @brief Основная функция
 *
 * Функция инициализирует окно терминала с помощью библиотеки ncurses и
 * устанавливает необходимые параметры.. Подключается генератор случайных чисел.
 * Запуск игры.
 * Очистка памяти выделенные для окна
 */

int main() {
  DefaultSetting();
  setlocale(LC_ALL, "");
  srand(time(0));
  RunGame();
  CloseWindow();
}

/**
 * @brief Запуск игры.
 *
 * Функция инициализирует структуру `GameInfo_t` и запускает основной игровой
 * цикл. Внутри цикла обновляется текущее состояние игры, заполняется окно игры,
 * обрабатывается ввод пользователя и выполняется конечный автомат.
 */

void RunGame() {
  GameInfo_t game_info = {0};

  while (!game_info.terminate) {
    game_info = updateCurrentState();
    FillWindow(game_info);
    GetUserInput(getch());
    fsm();
  }
}

/**
 * @brief Обработка ввода пользователя.
 *
 * @param input_number Клавиша, нажатая пользователем.
 */

void GetUserInput(int input_number) {
  switch (input_number) {
    case 's':
      userInput(Start, true);
      break;
    case 'p':
      userInput(Pause, true);
      break;
    case 'q':
      userInput(Terminate, true);
      break;
    case 'w':
      userInput(Action, true);
      break;
    case KEY_LEFT:
      userInput(Left, true);
      break;
    case KEY_RIGHT:
      userInput(Right, true);
      break;
    case KEY_UP:
      userInput(Up, true);
      break;
    case KEY_DOWN:
      userInput(Down, true);
      break;
    default:
      break;
  }
}