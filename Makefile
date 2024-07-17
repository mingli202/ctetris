all: compile run

compile:
	mkdir -p bin && clang -std=c2x -o bin/tetris main.c `pkgconf --libs --cflags ncurses`

run:
	./bin/tetris
