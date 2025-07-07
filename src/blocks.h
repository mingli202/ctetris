#ifndef BLOCK_H
#define BLOCK_H

#include "matrix/matrix.h"
#include "settings.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

enum BlockType { I, J, L, O, S, T, Z };

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

Matrix get_shape(enum BlockType type) {
  Matrix R;

  switch (type) {
  case I: {
    int block[] = {1, 1, 1, 1};
    R = matrix_from(1, 4, block);
    break;
  }
  case J: {
    int block[] = {1, 0, 0, 1, 1, 1};
    R = matrix_from(2, 3, block);
    break;
  }
  case L: {
    int block[] = {0, 0, 1, 1, 1, 1};
    R = matrix_from(2, 3, block);
    break;
  }
  case O: {
    int block[] = {1, 1, 1, 1};
    R = matrix_from(2, 2, block);
    break;
  }
  case S: {
    int block[] = {0, 1, 1, 1, 1, 0};
    R = matrix_from(2, 3, block);
    break;
  }
  case T: {
    int block[] = {0, 1, 0, 1, 1, 1};
    R = matrix_from(2, 3, block);
    break;
  }
  case Z: {
    int block[] = {1, 1, 0, 0, 1, 1};
    R = matrix_from(2, 3, block);
    break;
  }
  }

  return R;
}

Matrix block_get_shape(enum BlockType type) {
  Matrix R;

  switch (type) {
  case I: {
    int block[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    R = matrix_from(4, 4, block);
    break;
  }
  case J: {
    int block[] = {1, 0, 0, 1, 1, 1, 0, 0, 0};
    R = matrix_from(3, 3, block);
    break;
  }
  case L: {
    int block[] = {0, 0, 1, 1, 1, 1, 0, 0, 0};
    R = matrix_from(3, 3, block);
    break;
  }
  case O: {
    int block[] = {1, 1, 1, 1};
    R = matrix_from(2, 2, block);
    break;
  }
  case S: {
    int block[] = {0, 1, 1, 1, 1, 0, 0, 0, 0};
    R = matrix_from(3, 3, block);
    break;
  }
  case T: {
    int block[] = {0, 1, 0, 1, 1, 1, 0, 0, 0};
    R = matrix_from(3, 3, block);
    break;
  }
  case Z: {
    int block[] = {1, 1, 0, 0, 1, 1, 0, 0, 0};
    R = matrix_from(3, 3, block);
    break;
  }
  }

  return R;
}

void block_wprint(WINDOW *win, Block block) {
  int x = block.position.x;
  int y = block.position.y;

  wattron(win, COLOR_PAIR(block.color));

  for (int i = block.shape.m - 1; i >= 0; i--) {
    for (int k = 0; k < block.shape.n; k++) {
      if (matrix_get(block.shape, i, k) == 1) {
        mvwprintw(win, y + i, x + k * 2, "  ");
      }
    }
  }

  wattroff(win, COLOR_PAIR(block.color));
}

void block_wclear(WINDOW *win, Block block) {
  int x = block.position.x;
  int y = block.position.y;

  wattron(win, A_NORMAL);

  for (int i = block.shape.m - 1; i >= 0; i--) {
    for (int k = 0; k < block.shape.n; k++) {
      if (matrix_get(block.shape, i, k) == 1) {
        mvwprintw(win, y + i, x + k * 2, "  ");
      }
    }
  }
}

Block block_new(int *last_color) {
  int type = arc4random_uniform(7);
  int color = arc4random_uniform(6) + 1;

  if (last_color != NULL) {
    while (color == *last_color) {
      color = arc4random_uniform(6) + 1;
    }

    *last_color = color;
  }

  int x = 3;

  if (type == I) {
    x = 2;
  } else if (type == O) {
    x = 4;
  }

  Block b = {.color = color,
             .position = {.x = x, .y = 1},
             .shape = block_get_shape(type),
             .type = type};

  return b;
}

void block_center(Dimensions dim, Block *block) {
  block->position.x = ((dim.width - dim.box) / 4 - block->shape.n / 2) * 2 + 1;
}

void ghost_wprint(WINDOW *game_win, Block block, int grid_placement) {
  block.position.y = grid_placement + 1;
  block.color = 9;
  block_wprint(game_win, block);
}

void ghost_wclear(WINDOW *game_win, Block block, int grid_placement) {
  block.position.y = grid_placement + 1;
  block.color = 9;
  block_wclear(game_win, block);
}

#endif
