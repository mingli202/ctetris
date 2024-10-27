all: compile run

compile:
	mkdir -p bin && clang -std=c2x -o bin/tetris `pkg-config --libs --cflags ncurses libcjson` src/main.c -g

run:
	TERM=tmux-256color ./bin/tetris

FILE?=src/test/test.c

test:
	mkdir -p bin\
		&& clang -std=c2x -o src/test/bin/test `pkg-config --libs --cflags ncurses libcjson` $(FILE) -g \
		&& TERM=tmux-256color ./src/test/bin/test


