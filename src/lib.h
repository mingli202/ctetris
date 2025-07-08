#ifndef LIB_H
#define LIB_H

#include <cJSON.h>
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
  init_pair(1, 1, 1); // red
  init_pair(2, 2, 2); // green
  init_pair(3, 3, 3); // yellow
  init_pair(4, 4, 4); // blue
  init_pair(5, 5, 5); // magenta
  init_pair(6, 6, 6); // cyan
}

void quit() {
  clear();
  refresh();
  curs_set(1);
  endwin(); // ends curses
  exit(0);
}

void get_highscore(int *highscore) {
  FILE *file = fopen("data.json", "r");

  if (file == NULL)
    return;

  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buff = malloc(length);

  if (!buff)
    return;

  size_t res = fread(buff, 1, length, file);
  cJSON *json = cJSON_Parse(buff);
  cJSON *highscore_value = cJSON_GetObjectItemCaseSensitive(json, "highscore");

  if (cJSON_IsNumber(highscore_value)) {
    *highscore = highscore_value->valueint;
  }

  cJSON_Delete(json);

  fclose(file);
}

#endif
