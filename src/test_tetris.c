#include <check.h>

#include "brick_game/tetris/tetris_backend.h"
#include "brick_game/tetris/tetris_lib.h"

START_TEST(alloc_dealloc_memory_test_1) {
  TetrisGame_t tetris_game = {0};
  allocMemory(&tetris_game.game_info.field, HEIGHT, WIDTH);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(0, tetris_game.game_info.field[i][j]);
    }
  }

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  ck_assert_ptr_null(tetris_game.game_info.field);
}
END_TEST

START_TEST(alloc_dealloc_memory_test_2) {
  TetrisGame_t tetris_game = {0};
  allocMemory(&tetris_game.game_info.next, SIZE_FIGURE, SIZE_FIGURE);

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      ck_assert_int_eq(0, tetris_game.game_info.next[i][j]);
    }
  }

  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(start_game_test_1) {
  TetrisGame_t tetris_game = {0};
  startGame(&tetris_game);
  ck_assert_int_eq(1, tetris_game.game_info.level);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(spawn_figure_test_1) {
  TetrisGame_t tetris_game = {0};
  startGame(&tetris_game);
  spawnFigure(&tetris_game);

  ck_assert_int_eq(1, tetris_game.figure.y);
  ck_assert_int_eq(4, tetris_game.figure.x);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(move_figure_test_1) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);

  moveLeft(&tetris_game);
  ck_assert_int_eq(START_POSITION_Y, tetris_game.figure.y);
  ck_assert_int_eq(START_POSITION_X - 1, tetris_game.figure.x);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(move_figure_test_2) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);

  moveRight(&tetris_game);

  ck_assert_int_eq(START_POSITION_Y, tetris_game.figure.y);
  ck_assert_int_eq(START_POSITION_X + 1, tetris_game.figure.x);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(move_figure_test_3) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);

  while (moveDown(&tetris_game)) {
    ;
  }

  ck_assert_int_eq(1, tetris_game.figure.y < HEIGHT);
  ck_assert_int_eq(START_POSITION_X, tetris_game.figure.x);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(no_collision_test_1) {
  TetrisGame_t tetris_game = {0};
  startGame(&tetris_game);
  spawnFigure(&tetris_game);

  moveDown(&tetris_game);

  ck_assert(!checkCollision(&tetris_game));

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(rotate_figure_o_test_1) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);
  tetris_game.figure.type_figure = 'o';

  Figure_t initial_figure = {0};

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      initial_figure.shape[i][j] = tetris_game.figure.shape[i][j];
    }
  }

  rotationFigure(&tetris_game);

  bool flag = false;
  for (int i = 0; i < SIZE_FIGURE && !flag; i++) {
    for (int j = 0; j < SIZE_FIGURE && !flag; j++) {
      if (initial_figure.shape[i][j] != tetris_game.figure.shape[i][j]) {
        flag = true;
        break;
      }
    }
  }

  ck_assert_int_eq(0, flag);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(rotate_figure_i_test_1) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);
  tetris_game.figure.type_figure = 'i';

  Figure_t initial_figure = {0};

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      initial_figure.shape[i][j] = tetris_game.figure.shape[i][j];
    }
  }

  rotationFigure(&tetris_game);

  bool flag = false;
  for (int i = 0; i < SIZE_FIGURE && !flag; i++) {
    for (int j = 0; j < SIZE_FIGURE && !flag; j++) {
      if (initial_figure.shape[i][j] != tetris_game.figure.shape[i][j]) {
        flag = true;
        break;
      }
    }
  }

  ck_assert_int_eq(1, flag);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(rotate_figure_s_test_1) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);
  tetris_game.figure.type_figure = 's';

  Figure_t initial_figure = {0};

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      initial_figure.shape[i][j] = tetris_game.figure.shape[i][j];
    }
  }

  rotationFigure(&tetris_game);

  bool flag = false;
  for (int i = 0; i < SIZE_FIGURE && !flag; i++) {
    for (int j = 0; j < SIZE_FIGURE && !flag; j++) {
      if (initial_figure.shape[i][j] != tetris_game.figure.shape[i][j]) {
        flag = true;
        break;
      }
    }
  }

  ck_assert_int_eq(1, flag);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(rotate_figure_z_test_1) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);
  tetris_game.figure.type_figure = 'z';

  Figure_t initial_figure = {0};

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      initial_figure.shape[i][j] = tetris_game.figure.shape[i][j];
    }
  }

  rotationFigure(&tetris_game);

  bool flag = false;
  for (int i = 0; i < SIZE_FIGURE && !flag; i++) {
    for (int j = 0; j < SIZE_FIGURE && !flag; j++) {
      if (initial_figure.shape[i][j] != tetris_game.figure.shape[i][j]) {
        flag = true;
        break;
      }
    }
  }

  ck_assert_int_eq(1, flag);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(rotate_figure_l_test_1) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);
  tetris_game.figure.type_figure = 'l';

  Figure_t initial_figure = {0};

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      initial_figure.shape[i][j] = tetris_game.figure.shape[i][j];
    }
  }

  rotationFigure(&tetris_game);

  bool flag = false;
  for (int i = 0; i < SIZE_FIGURE && !flag; i++) {
    for (int j = 0; j < SIZE_FIGURE && !flag; j++) {
      if (initial_figure.shape[i][j] != tetris_game.figure.shape[i][j]) {
        flag = true;
        break;
      }
    }
  }

  ck_assert_int_eq(1, flag);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(rotate_figure_j_test_1) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);
  tetris_game.figure.type_figure = 'j';

  Figure_t initial_figure = {0};

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      initial_figure.shape[i][j] = tetris_game.figure.shape[i][j];
    }
  }

  rotationFigure(&tetris_game);

  bool flag = false;
  for (int i = 0; i < SIZE_FIGURE && !flag; i++) {
    for (int j = 0; j < SIZE_FIGURE && !flag; j++) {
      if (initial_figure.shape[i][j] != tetris_game.figure.shape[i][j]) {
        flag = true;
        break;
      }
    }
  }

  ck_assert_int_eq(1, flag);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(rotate_figure_t_test_1) {
  TetrisGame_t tetris_game = {0};

  startGame(&tetris_game);
  spawnFigure(&tetris_game);
  tetris_game.figure.type_figure = 't';

  Figure_t initial_figure = {0};

  for (int i = 0; i < SIZE_FIGURE; i++) {
    for (int j = 0; j < SIZE_FIGURE; j++) {
      initial_figure.shape[i][j] = tetris_game.figure.shape[i][j];
    }
  }

  rotationFigure(&tetris_game);

  bool flag = false;
  for (int i = 0; i < SIZE_FIGURE && !flag; i++) {
    for (int j = 0; j < SIZE_FIGURE && !flag; j++) {
      if (initial_figure.shape[i][j] != tetris_game.figure.shape[i][j]) {
        flag = true;
        break;
      }
    }
  }

  ck_assert_int_eq(1, flag);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(action_figure_near_walls_test_1) {
  TetrisGame_t tetris_game = {0};
  startGame(&tetris_game);
  spawnFigure(&tetris_game);

  for (int i = 0; i < 10; i++) {
    moveLeft(&tetris_game);
    rotationFigure(&tetris_game);
  }

  ck_assert_int_eq(1, START_POSITION_X > -1);

  for (int i = 0; i < 10; i++) {
    moveRight(&tetris_game);
    rotationFigure(&tetris_game);
  }
  ck_assert_int_eq(1, START_POSITION_X < WIDTH);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(refresh_field_test_1) {
  TetrisGame_t tetris_game = {0};
  startGame(&tetris_game);
  spawnFigure(&tetris_game);

  while (moveDown(&tetris_game)) {
    ;
  }

  refreshField(&tetris_game);

  ck_assert_int_eq(tetris_game.game_info.field[HEIGHT - 1][5], 1);

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(check_lines_test_1) {
  TetrisGame_t tetris_game = {0};
  startGame(&tetris_game);
  spawnFigure(&tetris_game);

  int scores[4] = {100, 300, 700, 1500};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < i + 1; j++) {
      for (int k = 0; k < WIDTH; k++) {
        tetris_game.game_info.field[j][k] = 1;
      }
    }
    checkLines(&tetris_game);
    ck_assert_int_eq(tetris_game.game_info.score, scores[i]);
    tetris_game.game_info.score = 0;
  }

  deallocMemory(&tetris_game.game_info.field, HEIGHT);
  deallocMemory(&tetris_game.game_info.next, SIZE_FIGURE);

  ck_assert_ptr_null(tetris_game.game_info.field);
  ck_assert_ptr_null(tetris_game.game_info.next);
}
END_TEST

START_TEST(restart_exit_game_test_1) {
  TetrisGame_t tetris_game = {0};
  startGame(&tetris_game);
  spawnFigure(&tetris_game);

  restartGame(&tetris_game);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(0, tetris_game.game_info.field[i][j]);
    }
  }
  ck_assert_int_eq(1, tetris_game.game_info.level);

  exitGame(&tetris_game);
}
END_TEST

START_TEST(read_write_highscore_test_1) {
  TetrisGame_t tetris_game = {0};

  readHighScore(&tetris_game);
  int result_1 = tetris_game.game_info.high_score;
  tetris_game.game_info.score = result_1;

  writeHighScore(&tetris_game);
  readHighScore(&tetris_game);

  int result_2 = tetris_game.game_info.high_score;

  ck_assert_int_eq(result_1, result_2);

  exitGame(&tetris_game);
}
END_TEST

START_TEST(fsm_start_test_1) {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  tetris_game->control_action = START;
  userInput(Start, true);
  fsm();

  ck_assert_ptr_nonnull(tetris_game->game_info.field);
  ck_assert_ptr_nonnull(tetris_game->game_info.next);
  ck_assert_int_eq(SPAWN, tetris_game->control_action);

  tetris_game->control_action = START;
  userInput(Terminate, true);
  fsm();
  ck_assert_ptr_null(tetris_game->game_info.field);
  ck_assert_ptr_null(tetris_game->game_info.next);
}
END_TEST

START_TEST(fsm_spawn_test_1) {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  tetris_game->control_action = START;
  userInput(Start, true);

  fsm();
  fsm();

  ck_assert_int_eq(MOVING, tetris_game->control_action);
  ck_assert_int_eq(START_POSITION_Y, tetris_game->figure.y);
  ck_assert_int_eq(START_POSITION_X, tetris_game->figure.x);

  tetris_game->control_action = START;
  userInput(Terminate, true);
  fsm();
}
END_TEST

START_TEST(fsm_moving_test_1) {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  tetris_game->control_action = START;
  userInput(Start, true);
  fsm();
  fsm();
  userInput(Up, true);
  fsm();
  userInput(Left, true);
  fsm();
  ck_assert_int_eq(START_POSITION_X - 1, tetris_game->figure.x);
  userInput(Right, true);
  fsm();
  ck_assert_int_eq(START_POSITION_X, tetris_game->figure.x);
  userInput(Action, true);
  fsm();
  ck_assert_int_eq(SHIFTING, tetris_game->control_action);
  tetris_game->control_action = MOVING;
  userInput(Pause, true);
  fsm();
  ck_assert_int_eq(PAUSE, tetris_game->control_action);

  tetris_game->control_action = MOVING;
  userInput(Terminate, true);
  fsm();
}
END_TEST

START_TEST(fsm_pause_test_1) {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  tetris_game->control_action = START;
  userInput(Start, true);
  fsm();
  fsm();
  userInput(Pause, true);
  fsm();
  ck_assert_int_eq(PAUSE, tetris_game->control_action);
  ck_assert_int_eq(1, tetris_game->game_info.pause);
  userInput(Pause, true);
  fsm();
  ck_assert_int_eq(0, tetris_game->game_info.pause);

  tetris_game->control_action = PAUSE;
  userInput(Terminate, true);
  fsm();
  ck_assert_ptr_null(tetris_game->game_info.field);
  ck_assert_ptr_null(tetris_game->game_info.next);
}
END_TEST

START_TEST(fsm_shifting_test_1) {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  tetris_game->control_action = START;
  userInput(Start, true);
  fsm();
  fsm();
  userInput(Action, true);
  fsm();
  ck_assert_int_eq(SHIFTING, tetris_game->control_action);
  fsm();
  ck_assert_int_eq(MOVING, tetris_game->control_action);

  tetris_game->control_action = START;
  userInput(Terminate, true);
  fsm();
  ck_assert_ptr_null(tetris_game->game_info.field);
  ck_assert_ptr_null(tetris_game->game_info.next);
}
END_TEST

START_TEST(fsm_attaching_test_1) {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  tetris_game->control_action = START;
  userInput(Start, true);
  fsm();
  fsm();

  while (moveDown(tetris_game)) {
    ;
  }

  userInput(Down, true);
  fsm();
  ck_assert_int_eq(ATTACHING, tetris_game->control_action);
  fsm();
  ck_assert_int_eq(SPAWN, tetris_game->control_action);

  GameInfo_t game_info = updateCurrentState();
  ck_assert_int_eq(1, game_info.level);

  tetris_game->control_action = START;
  userInput(Terminate, true);
  fsm();
  ck_assert_ptr_null(tetris_game->game_info.field);
  ck_assert_ptr_null(tetris_game->game_info.next);
}
END_TEST

START_TEST(fsm_gameover_test_1) {
  TetrisGame_t* tetris_game = getCurrentGameInfo();
  tetris_game->control_action = START;
  userInput(Start, true);
  fsm();
  fsm();

  tetris_game->control_action = GAMEOVER;
  userInput(Start, true);
  fsm();
  ck_assert_int_eq(0, tetris_game->game_info.pause);
  ck_assert_int_eq(1, tetris_game->game_info.level);

  tetris_game->control_action = GAMEOVER;
  userInput(Terminate, true);
  fsm();

  tetris_game->control_action = START;
  userInput(Terminate, true);
  fsm();
  ck_assert_ptr_null(tetris_game->game_info.field);
  ck_assert_ptr_null(tetris_game->game_info.next);
}
END_TEST

Suite* s21_tetris(void) {
  Suite* suite = suite_create("s21_tetris");
  TCase* tcase_core = tcase_create("Core");

  // backend
  tcase_add_test(tcase_core, alloc_dealloc_memory_test_1);
  tcase_add_test(tcase_core, alloc_dealloc_memory_test_2);
  tcase_add_test(tcase_core, start_game_test_1);
  tcase_add_test(tcase_core, spawn_figure_test_1);
  tcase_add_test(tcase_core, move_figure_test_1);
  tcase_add_test(tcase_core, move_figure_test_2);
  tcase_add_test(tcase_core, move_figure_test_3);
  tcase_add_test(tcase_core, no_collision_test_1);
  tcase_add_test(tcase_core, rotate_figure_o_test_1);
  tcase_add_test(tcase_core, rotate_figure_i_test_1);
  tcase_add_test(tcase_core, rotate_figure_s_test_1);
  tcase_add_test(tcase_core, rotate_figure_z_test_1);
  tcase_add_test(tcase_core, rotate_figure_l_test_1);
  tcase_add_test(tcase_core, rotate_figure_j_test_1);
  tcase_add_test(tcase_core, rotate_figure_t_test_1);
  tcase_add_test(tcase_core, action_figure_near_walls_test_1);
  tcase_add_test(tcase_core, refresh_field_test_1);
  tcase_add_test(tcase_core, check_lines_test_1);
  tcase_add_test(tcase_core, restart_exit_game_test_1);
  tcase_add_test(tcase_core, read_write_highscore_test_1);

  // fsm
  tcase_add_test(tcase_core, fsm_start_test_1);
  tcase_add_test(tcase_core, fsm_spawn_test_1);
  tcase_add_test(tcase_core, fsm_moving_test_1);
  tcase_add_test(tcase_core, fsm_pause_test_1);
  tcase_add_test(tcase_core, fsm_shifting_test_1);
  tcase_add_test(tcase_core, fsm_attaching_test_1);
  tcase_add_test(tcase_core, fsm_gameover_test_1);

  suite_add_tcase(suite, tcase_core);

  return suite;
}

int main() {
  Suite* suite = s21_tetris();
  SRunner* suite_runner = srunner_create(suite);

  srunner_run_all(suite_runner, CK_VERBOSE);
  int failed_counter = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);

  return (failed_counter == 0) ? 0 : 1;
}