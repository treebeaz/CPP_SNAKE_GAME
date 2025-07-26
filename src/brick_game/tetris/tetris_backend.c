#include "tetris_backend.h"

#include "fsm.h"

/**
 * @brief Реализация старта игры.
 *
 * Функция инициализирует игровое поле и область следующей фигуры.
 * Считывает с файла рекорд, создает следующую фигуру и устанавливает начальный
 * уровень.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'
 */

void startGame(TetrisGame_t* tetris_game) {
  allocMemory(&tetris_game->game_info.field, HEIGHT, WIDTH);
  allocMemory(&tetris_game->game_info.next, SIZE_FIGURE, SIZE_FIGURE);

  readHighScore(tetris_game);

  createNextFigure(tetris_game);

  tetris_game->game_info.level = 1;
  tetris_game->delay = 1.0;
}

/**
 * @brief Выделение памяти.
 *
 * @param matrix Указатель на матрицу, для которой выделяется память.
 * @param rows Количество строк.
 * @param cols Количество столбцов.
 */

void allocMemory(int*** matrix, int rows, int cols) {
  *matrix = (int**)malloc(rows * sizeof(int*));
  for (int i = 0; i < rows; i++) {
    (*matrix)[i] = (int*)malloc(cols * sizeof(int));
  }
  fillMatrix(*matrix, rows, cols);
}

/**
 * @brief Инициализация матрицы начальными значениями.
 *
 * @param matrix Указатель на матрицу.
 * @param rows Количество строк.
 * @param cols Количество столбцов.
 */

void fillMatrix(int** matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = 0;
    }
  }
}

/**
 * @brief Завершение игры.
 *
 * Функция завершает игру, освобождая выделенную память для игрового поля и
 * следующей фигуры.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'
 */

void exitGame(TetrisGame_t* tetris_game) {
  if (tetris_game->game_info.field != NULL) {
    deallocMemory(&tetris_game->game_info.field, HEIGHT);
    deallocMemory(&tetris_game->game_info.next, SIZE_FIGURE);
  }
}

/**
 * @brief Освобождение памяти.
 *
 * @param matrix Указатель на матрицу, для которой будет очищена память.
 * @param size Количество строк.
 */

void deallocMemory(int*** matrix, int size) {
  for (int i = 0; i < size; i++) {
    free((*matrix)[i]);
  }
  free(*matrix);
  *matrix = NULL;
}

/**
 * @brief Создание следующей фигуры.
 *
 * Функция случайным образом выбирает следующую фигуру и переносит ее матрицу
 * next.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'
 */

void createNextFigure(TetrisGame_t* tetris_game) {
  int temp_figure[7][4][4] = {
      {{0}, {1, 1, 1, 1}, {0}, {0}},           // i
      {{0, 1, 1, 0}, {0, 1, 1, 0}, {0}, {0}},  // o
      {{0, 1, 0, 0}, {1, 1, 1, 0}, {0}, {0}},  // t
      {{1, 0, 0, 0}, {1, 1, 1, 0}, {0}, {0}},  // j
      {{0, 0, 1, 0}, {1, 1, 1, 0}, {0}, {0}},  // l
      {{0, 1, 1, 0}, {1, 1, 0, 0}, {0}, {0}},  // s
      {{1, 1, 0, 0}, {0, 1, 1, 0}, {0}, {0}}   // z
  };
  int type_figure = rand() % 7;
  copyFigure(tetris_game, temp_figure[type_figure]);
  fillingShapeType(tetris_game, type_figure);
}

/**
 * @brief Установка типа фигуры.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 * @param type_figure Тип фигуры.
 */

void fillingShapeType(TetrisGame_t* tetris_game, int type_figure) {
  switch (type_figure) {
    case 0:
      tetris_game->figure.next_type_figure = 'i';
      break;
    case 1:
      tetris_game->figure.next_type_figure = 'o';
      break;
    case 2:
      tetris_game->figure.next_type_figure = 't';
      break;
    case 3:
      tetris_game->figure.next_type_figure = 'j';
      break;
    case 4:
      tetris_game->figure.next_type_figure = 'l';
      break;
    case 5:
      tetris_game->figure.next_type_figure = 's';
      break;
    case 6:
      tetris_game->figure.next_type_figure = 'z';
      break;
    default:
      break;
  }
}

/**
 * @brief Копирование фигуры.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'
 * @param temp_figure Матрица, представляющая фиугру, которую необходимо
 * скопировать в next.
 */

void copyFigure(TetrisGame_t* tetris_game, int temp_figure[4][4]) {
  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      tetris_game->game_info.next[i][j] = temp_figure[i][j];
    }
  }
}

/**
 * @brief Размещение фигуры на игровом поле.
 *
 * Функция устанавливает начальные координаты фигуры и копирует ее форму из
 * матрицы next в структуру фигуры 'Figure_t'. Затем создается новая следующая
 * фигура.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 */

void spawnFigure(TetrisGame_t* tetris_game) {
  tetris_game->figure.y = START_POSITION_Y;
  tetris_game->figure.x = START_POSITION_X;

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      tetris_game->figure.shape[i][j] = tetris_game->game_info.next[i][j];
    }
  }
  tetris_game->figure.type_figure = tetris_game->figure.next_type_figure;
  createNextFigure(tetris_game);
}

/**
 * @brief Движение фигуры влево.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 */

void moveLeft(TetrisGame_t* tetris_game) {
  Figure_t figure = tetris_game->figure;
  bool check_board = true;
  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      if (figure.shape[i][j]) {
        if (figure.x + j <= 0 ||
            tetris_game->game_info.field[figure.y + i - 1][figure.x + j - 1]) {
          check_board = false;
        }
      }
    }
  }
  if (check_board) {
    tetris_game->figure.x--;
  }
}

/**
 * @brief Движение фигуры вправо.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 */

void moveRight(TetrisGame_t* tetris_game) {
  Figure_t figure = tetris_game->figure;

  bool check_board = true;
  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      if (figure.shape[i][j]) {
        if ((figure.x + j) >= WIDTH - 1 ||
            tetris_game->game_info.field[figure.y + i - 1][figure.x + j + 1]) {
          check_board = false;
        }
      }
    }
  }
  if (check_board) {
    tetris_game->figure.x++;
  }
}

/**
 * @brief Проверка на столкновение текущей фигуры с игровым полем.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 *
 * @return true, елси произошло столкновение, иначе false.
 */

bool checkCollision(TetrisGame_t* tetris_game) {
  Figure_t figure = tetris_game->figure;
  bool check_board = false;

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      if (figure.shape[i][j]) {
        if ((figure.y + i - 1) >= HEIGHT - 1 ||
            tetris_game->game_info.field[figure.y + i][figure.x + j]) {
          check_board = true;
        }
      }
    }
  }

  return check_board;
}

/**
 * @brief Движение фигуры вниз.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 *
 * @return true, если было перемещение, иначе false.
 */

bool moveDown(TetrisGame_t* tetris_game) {
  if (!checkCollision(tetris_game)) {
    tetris_game->figure.y++;
    return true;
  }
  return false;
}

/**
 * @brief Основная функция разворота текущей фигуры.
 *
 * Функция осуществляет разворот текущей фигуры.
 * Если тип фигуры не является квадратом, то функция выполняет разворот фигуры
 * и корректирует ее позиции.
 * После этого проверяет, можно ли развернуть фигуру в новом положении. Если
 * можно, то текущая фигура заменяется на новую развернутую фигуру.
 *
 * @param tetris_game 'TetrisGame_t'
 */

void rotationFigure(TetrisGame_t* tetris_game) {
  if (tetris_game->figure.type_figure != 'o') {
    Figure_t inverted_figure = {0};
    if (tetris_game->figure.type_figure != 'i')
      inverted_figure = RotateFigure(tetris_game->figure);
    else
      inverted_figure = RotateFigureStick(tetris_game->figure);

    correctFigurePosition(&inverted_figure);

    if (canRotateFigure(tetris_game, &inverted_figure))
      tetris_game->figure = inverted_figure;
  }
}

/**
 * @brief Разворот фигур на 90 градусов.
 *
 * Функция разворачивает все фигуры на 90 градусов, кроме палки (i) и кроме
 * квадрата (o).
 *
 * @param figure Исходная фигура типа 'Figure_t'
 *
 * @return Развернутая фигура типа 'Figure_t'
 */

Figure_t RotateFigure(Figure_t figure) {
  int temp_figure[SIZE_FIGURE][SIZE_FIGURE] = {0};

  Figure_t rotation_figure = {0};
  rotation_figure.x = figure.x;
  rotation_figure.y = figure.y;
  rotation_figure.type_figure = figure.type_figure;
  rotation_figure.next_type_figure = figure.next_type_figure;

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      int new_coordinate_x = j - 1;
      int new_coordinate_y = -(i - 1);
      temp_figure[new_coordinate_x + 1][new_coordinate_y + 1] =
          figure.shape[i][j];
    }
  }

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      rotation_figure.shape[i][j] = temp_figure[i][j];
    }
  }

  return rotation_figure;
}

/**
 * @brief Разворот палки (тип фигуры 'i').
 *
 * @param figure Исходная фигура типа 'Figure_t'.
 *
 * @return Развернутая фигура ('i') типа 'Figure_t'
 */

Figure_t RotateFigureStick(Figure_t figure) {
  Figure_t rotation_figure = {0};
  int temp_figure[SIZE_FIGURE][SIZE_FIGURE] = {0};

  rotation_figure.x = figure.x;
  rotation_figure.y = figure.y;
  rotation_figure.type_figure = figure.type_figure;
  rotation_figure.next_type_figure = figure.next_type_figure;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp_figure[j][i] = figure.shape[i][j];
    }
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      rotation_figure.shape[i][j] = temp_figure[i][j];
    }
  }

  return rotation_figure;
}

/**
 * @brief Корректировка позиции фигуры по горизонтали.
 *
 * Функция проверяет и корректирует позицию фигуры по горизонтали, чтобы
 * она не выходила за границы игрового поля.
 *
 * @param figure Указатель на структуру фигуры 'Figure_t'.
 */

void correctFigurePosition(Figure_t* figure) {
  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      if (figure->shape[i][j]) {
        if (figure->x + j == WIDTH) {
          figure->x--;
        }
        if (figure->x + j < 0) {
          figure->x++;
        }
      }
    }
  }
}

/**
 * @brief Проверка разворота фигуры.
 *
 * Проверяется, можно ли развернуть фигуру в новом положении так, чтобы она не
 * выходила за границы и не пересекалась с другими фигурами на поле.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 * @param rotated_figure Указатель на структуру 'Figure_t', которую нужно
 * перевернуть.
 *
 * @return true, если разворот возможен, иначе false.
 */

bool canRotateFigure(TetrisGame_t* tetris_game, Figure_t* rotated_figure) {
  bool flag = true;

  int new_coordinate_x = 0;
  int new_coordinate_y = 0;

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      if (rotated_figure->shape[i][j]) {
        new_coordinate_x = rotated_figure->x + j;
        new_coordinate_y = rotated_figure->y + i;

        if (new_coordinate_y > HEIGHT - 1 || new_coordinate_x < 0 ||
            new_coordinate_x > WIDTH - 1 ||
            tetris_game->game_info
                    .field[new_coordinate_y - 1][new_coordinate_x] == 1) {
          flag = false;
        }
      }
    }
  }

  return flag;
}

/**
 * @brief Обновление поля после размещения фигуры.
 *
 * Функция обновляет поле, учитывая текущую позицию и форму фигуры.
 * Также проверяет, возникли ли полные линии, которые нужно удалить.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'
 */

void refreshField(TetrisGame_t* tetris_game) {
  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      if (tetris_game->figure.shape[i][j] == 1) {
        tetris_game->game_info
            .field[tetris_game->figure.y + i - 1][tetris_game->figure.x + j] =
            1;
      }
    }
  }
  checkLines(tetris_game);
}

/**
 * @brief Проверка и удаление полных линий.
 *
 * Функция проверяет каждую строку игрового поля на наличие полных линий.
 * Если такие строки найдены, они удаляются, а все строки, находящие выше
 * сдвигаются вниз. Также обновляется счет игры, вычисляется задержка и
 * записывается рекорд.
 *
 * @param tetris_game  указатель на текущую структуру 'TetrisGame_t'
 */

void checkLines(TetrisGame_t* tetris_game) {
  int remote_lines = 0;

  for (int i = HEIGHT - 1; i >= 0; i--) {
    bool flag = true;
    for (int j = 0; j < WIDTH; j++) {
      if (!(tetris_game->game_info.field[i][j])) flag = false;
    }
    if (flag) {
      remote_lines++;
      for (int j = i; j > 0; j--) {
        for (int k = 0; k < WIDTH; k++) {
          tetris_game->game_info.field[j][k] =
              tetris_game->game_info.field[j - 1][k];
        }
      }
      for (int j = 0; j < WIDTH; j++) {
        tetris_game->game_info.field[0][j] = 0;
      }
      i++;
    }
  }

  tetris_game->game_info.score += countScore(remote_lines);

  if (tetris_game->game_info.level < 10) {
    delayCounting(tetris_game);
  }

  if (tetris_game->game_info.score > tetris_game->game_info.high_score) {
    tetris_game->game_info.high_score = tetris_game->game_info.score;
    writeHighScore(tetris_game);
  }
}

/**
 * @brief Счет очков.
 *
 * @param remote_lines Количество удаленных линий.
 *
 * @return Количество очков.
 */

int countScore(int remote_lines) {
  int counter_score = 0;
  switch (remote_lines) {
    case 1:
      counter_score = 100;
      break;
    case 2:
      counter_score = 300;
      break;
    case 3:
      counter_score = 700;
      break;
    case 4:
      counter_score = 1500;
      break;
    default:
      break;
  }
  return counter_score;
}

/**
 * @brief Получение текущего времени в миллисекундах.
 *
 * @return Текущее время в миллисекундах.
 */

long long currentTimeInMs() {
  struct timeval current_time;
  gettimeofday(&current_time, NULL);

  long long second_in_ms = current_time.tv_sec * 1000LL;
  long long microsecond_in_ms = current_time.tv_usec / 1000;

  return second_in_ms + microsecond_in_ms;
}

/**
 * @brief Установка задержки падения фигур.
 *
 * Функция определяет уровень на основе набранных очков, и в следствии этого
 * определяет, как быстро будут падать фигуры. Чем выше уровень, тем быстрее
 * фигура будет падать фигура.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'
 */

void delayCounting(TetrisGame_t* tetris_game) {
  if (tetris_game->game_info.score / 100 + 1 > tetris_game->game_info.level) {
    tetris_game->game_info.level =
        (int)(tetris_game->game_info.score / 100) + 1;

    if (tetris_game->game_info.level > 10) {
      tetris_game->game_info.level = 10;
    }

    tetris_game->delay = 1.0 - tetris_game->game_info.level * (0.09);
  }
}

/**
 * @brief Время задержки.
 *
 * ФУнкция проверяет прошедшее время с момента последнего падения фигуры.
 * Если это время превысило текущую задержку, функция пермещает фигуру вниз.
 * Если фигура больше не может двигаться вниз, то автомат переходит к состоянию
 * прикрепления фигуры к полю (ATTACHING).
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'
 */

void checkDelay(TetrisGame_t* tetris_game) {
  time_t end = currentTimeInMs();
  double offset = end - tetris_game->start;
  if (offset >= tetris_game->delay * 1000) {
    if (!moveDown(tetris_game)) {
      tetris_game->control_action = ATTACHING;
    }
    tetris_game->start = end;
  }
}

/**
 * @brief Запись рекорда в файл.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 */

void writeHighScore(TetrisGame_t* tetris_game) {
  FILE* file = fopen("./brick_game/tetris/high_score.txt", "w");
  if (file != NULL) {
    fprintf(file, "%d", tetris_game->game_info.high_score);
  }
  fclose(file);
}

/**
 * @brief Чтение рекорда из файла.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 */

void readHighScore(TetrisGame_t* tetris_game) {
  FILE* file = fopen("./brick_game/tetris/high_score.txt", "r");
  if (file != NULL) {
    if (fscanf(file, "%d", &tetris_game->game_info.high_score) != 1) {
      tetris_game->game_info.high_score = 0;
    }
  } else
    tetris_game->game_info.high_score = 0;
  fclose(file);
}

/**
 * @brief Перезапуск игры.
 *
 * Функция обновляет параметры игры на начальные значения.
 *
 * @param tetris_game Указатель на текущую стркутуру 'TetrisGame_t'.
 */

void restartGame(TetrisGame_t* tetris_game) {
  fillMatrix(tetris_game->game_info.field, HEIGHT, WIDTH);
  fillMatrix(tetris_game->game_info.next, SIZE_FIGURE, SIZE_FIGURE);

  createNextFigure(tetris_game);

  readHighScore(tetris_game);

  tetris_game->game_info.level = 1;
  tetris_game->game_info.score = 0;
  tetris_game->game_info.pause = 0;
  tetris_game->delay = 1.0;
}

/**
 * @brief Проверка конца игры.
 *
 * @param tetris_game Указатель на текущую структуру 'TetrisGame_t'.
 *
 * @return true, если игра закончена, иначе false.
 */

bool checkGameOver(TetrisGame_t* tetris_game) {
  bool flag = false;
  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      if (tetris_game->figure.shape[i][j]) {
        if (tetris_game->game_info
                .field[START_POSITION_Y + i][START_POSITION_X + j]) {
          flag = true;
        }
      }
    }
  }
  return flag;
}