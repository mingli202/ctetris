#ifndef BLOCK_H
#define BLOCK_H

#include "matrix/matrix.h"
#include "settings.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

enum BlockType {
  I = 1,
  J = 2,
  L = 3,
  O = 4,
  S = 5,
  T = 6,
  Z = 7,
};

typedef struct Point {
  int x;
  int y;
} Point;

typedef struct Block {
  int color;
  struct Point position;
  Matrix shape;
  enum BlockType type;
} Block;

Block block_new();
Matrix block_get_shape(enum BlockType type);
void block_wprint(WINDOW *win, Block block);
void block_wclear(WINDOW *win, Block block);
void block_center(int width, Block *block);

Matrix get_shape(enum BlockType type);
void ghost_wprint(WINDOW *game_win, Block block, int grid_placement);
void ghost_wclear(WINDOW *game_win, Block block, int grid_placement);

#endif
