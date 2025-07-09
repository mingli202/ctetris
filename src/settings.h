#ifndef SETTINGS_H
#define SETTINGS_H

#include <ncurses.h>

typedef struct Dimensions {
  int height;
  int width;
  int x;
  int y;
  int box;
} Dimensions;

Dimensions dim_game = {.height = 22, .width = 22, .box = 2};

Dimensions dim_hold = {.height = 4, .width = 12, .box = 2};

Dimensions dim_next = {.height = 10, .width = 12, .box = 2};

#endif
