#ifndef LIB_H
#define LIB_H

#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

enum State { MENU, GAME };

void init() {
  initscr(); // init curses
  //
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

#endif
