CC = gcc
FLAG_C = -c
FLAG_ER = -Wall -Werror -Wextra -std=c11

TET_LIB_C = ./brick_game/tetris/*.c
TET_LIB_H = ./brick_game/tetris/*.h
TET_LIB_O = tetris_backend.o fsm.o tetris_lib.o

CLI_C = ./gui/cli/*.c
CLI_H = ./gui/cli/*.h
CLI_O = frontend.o

OS = $(shell uname -s)
ifeq ($(shell uname), Linux)
	FLAG_TEST += -lcheck  -lrt -lm -lsubunit
endif

ifeq ($(OS), Darwin)
	CHECKFLAGS = -lcheck
endif

all: clean install

install: tetris_lib.a
	gcc -Wall -Wextra -Werror -Wpedantic -o tetris_game main.c ./gui/cli/tetris_frontend.c tetris_lib.a -lncurses

uninstall:
	-rm -rf tetris_game 

tetris_lib.a: clean
	$(CC) $(FLAG_C) $(FLAG_ER) $(TET_LIB_C)
	ar rc tetris_lib.a $(TET_LIB_O) -lm
	ranlib tetris_lib.a

test: clean tetris_lib.a
	gcc -g $(FLAG_ER) test_tetris.c -o test tetris_lib.a $(FLAG_TEST)
	./test

valgrind: test
	valgrind --tool=memcheck --leak-check=yes ./test

dvi:
	doxygen
	open ./html/index.html

dist:
	tar -czf tetris.install.tar.gz ./* 

gcov_report: test
	gcc --coverage $(TET_LIB_C) test_tetris.c -o greport $(FLAG_TEST)
	./greport
	lcov -t "test" -o tetris.info -c -d .
	genhtml -o ./report tetris.info
	open ./report/index.html

clang:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c *.h brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c gui/cli/*.h
	@rm .clang-format

clean:
	@rm -rf ./a.out ./*.o ./*.a ./test ./GcovReport ./report ./*.info ./*.gcno ./*gcda ./*.gcov ./report gcov_report* greport ./tetris_game
	-rm -rf html latex
	-rm -f tetris.install.tar.gz 