#include "lib.h"
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void init() {
  initscr(); // init curses

  if (COLS < 53) {
    endwin();
    fprintf(stderr, "Window size too small\n");
    fprintf(stderr, "Current: %i; required: 53\n", COLS);
    exit(1);
  }

  cbreak();              // don't wait for new line
  keypad(stdscr, true);  // enable arrow keys
  noecho();              // don't echo back inputs
  curs_set(0);           // hide cursor
  nodelay(stdscr, true); // getch is non-blocking
  clear();
  refresh();

  start_color();
  init_pair(8, COLOR_WHITE, COLOR_BLACK);
  init_pair(9, COLOR_BLACK, COLOR_WHITE);

  init_pair(0, 0, 0);
  init_pair(7, COLOR_RED, COLOR_RED);         // red
  init_pair(3, COLOR_RED, COLOR_RED);         // red
  init_pair(5, COLOR_GREEN, COLOR_GREEN);     // green
  init_pair(4, COLOR_YELLOW, COLOR_YELLOW);   // yellow
  init_pair(2, COLOR_BLUE, COLOR_BLUE);       // blue
  init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA); // magenta
  init_pair(1, COLOR_CYAN, COLOR_CYAN);       // cyan
}

void quit() {
  clear();
  refresh();
  curs_set(1);
  endwin(); // ends curses
  exit(0);
}

void print_help() {
  printf("Tetris in the terminal\n\n");
  printf("Usage:  ctetris [(-l | -level) <level>]\n");
  printf("        ctetris [(-i | --initial-level) <level>]\n");
  printf("        ctetris [-h | --help]\n");
  printf("\n");
  printf("Options:\n");
  printf("  -l, --level <level>          Set a constant level to play\n");
  printf("  -i, --initial-level <level>  Set the initial level to play\n");
  printf("  -h, --help                   Show this help message\n");
}

void parse_args(int argc, char **argv, int *level, bool *is_constant_level) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_help();
      exit(0);
    } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--level") == 0) {
      if (i + 1 < argc) {
        *is_constant_level = true;
        *level = atoi(argv[i + 1]) <= 0 ? 1 : atoi(argv[i + 1]);
      }
    } else if (strcmp(argv[i], "-i") == 0 ||
               strcmp(argv[i], "--initial-level") == 0) {
      if (i + 1 < argc) {
        *is_constant_level = false;
        *level = atoi(argv[i + 1]) <= 0 ? 1 : atoi(argv[i + 1]);
      }
    }
  }
}
