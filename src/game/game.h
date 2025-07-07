#ifndef GAME_H
#define GAME_H

#include "../blocks.h"
#include "../lib.h"
#include "../matrix/matrix.h"
#include "../settings.h"

#include <assert.h>
#include <ncurses.h>

enum Action { MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, ROTATE_LEFT, ROTATE_RIGHT };

void create_initial_queue(Block *queue) {
  int last_color = -1;

  for (int i = 0; i < 3; i++) {
    queue[i] = block_new(&last_color);
    queue[i].position.y = 3 * i + 1;
  }
}

void update_next_window(WINDOW *next_win, Block queue[]) {
  for (int i = 0; i < 3; i++) {
    Matrix block = get_shape(queue[i].type);
    wattron(next_win, COLOR_PAIR(queue[i].color));

    int offset_x = 2;
    int offset_y = 0;

    if (queue[i].type == I) {
      offset_y = 1;
      offset_x = 1;
    }
    if (queue[i].type == O) {
      offset_x = 3;
    }

    for (int row = 0; row < block.m; row++) {

      for (int col = 0; col < block.n; col++) {
        if (matrix_get(block, row, col) == 1) {
          mvwprintw(next_win, i * 3 + row + 1 + offset_y,
                    1 + col * 2 + offset_x, "  ");
        }
      }
    }
    wattroff(next_win, COLOR_PAIR(queue[i].color));
  }

  wrefresh(next_win);
}

void update_current(WINDOW *next_win, WINDOW *game_win, Block *queue,
                    Block *current) {
  // things related to queue window
  attrset(A_NORMAL);
  for (int i = 1; i < 9; i++) {
    mvwprintw(next_win, i, 1, "          ");
  }

  *current = queue[0];
  current->position.x = 5;
  current->position.y = 1;

  queue[0] = queue[1];
  queue[0].position.y -= 3;

  queue[1] = queue[2];
  queue[1].position.y -= 3;

  int last_color = queue[1].color;

  queue[2] = block_new(&last_color);
  queue[2].position.y = 2 * 3 + 1;

  update_next_window(next_win, queue);
}

bool is_block_overlap(Matrix grid, Matrix shape, int offset_y, int offset_x) {
  for (int i = 0; i < shape.m; i++) {
    for (int k = 0; k < shape.n; k++) {
      if (matrix_get(shape, i, k) == 1) {
        if (!(i + offset_y >= 0 && i + offset_y < grid.m) ||
            !(k + offset_x >= 0 && k + offset_x < grid.n) ||
            matrix_get(grid, i + offset_y, k + offset_x) != 0) {
          return true;
        }
      }
    }
  }

  return false;
}

int get_grid_placement(Matrix grid, Block current) {
  int offset_y = current.position.y;
  int offset_x = (current.position.x - 1) / 2;

  while (!is_block_overlap(grid, current.shape, offset_y, offset_x)) {
    offset_y++;
  }

  return offset_y - 1;
}

void place_block(Matrix *grid, Block current, int placement) {
  int offset_y = placement;
  int offset_x = (current.position.x - 1) / 2;

  for (int i = 0; i < current.shape.m; i++) {
    for (int k = 0; k < current.shape.n; k++) {
      if (matrix_get(current.shape, i, k) == 1) {
        matrix_set(grid, i + offset_y, k + offset_x, current.color);
      }
    }
  }
}

int update_grid(Matrix *grid) {
  int front = 19;
  int back = 19;

  for (; front >= 0; front--) {
    bool is_full = true;

    for (int i = 0; i < grid->n; i++) {
      if (matrix_get(*grid, front, i) == 0) {
        is_full = false;
        break;
      }
    }

    if (!is_full) {
      for (int i = 0; i < grid->n; i++) {
        matrix_set(grid, back, i, matrix_get(*grid, front, i));
      }
      back--;
    }
  }
  return back - front;
}

int handle_rotate(Matrix grid, Block *current, Matrix standby) {
  int offset_y = current->position.y - 1;
  int offset_x = (current->position.x - 1) / 2;

  int tries[] = {0, -1, 1};

  for (int i = 0; i < 3; i++) {
    if (!is_block_overlap(grid, standby, offset_y, offset_x + tries[i])) {
      current->position.x = (offset_x + tries[i]) * 2 + 1;
      current->shape = standby;
      break;
    }
  }

  int grid_placement = get_grid_placement(grid, *current);

  if (current->position.y == grid_placement + 1) {
    offset_y -= 1;
    if (!is_block_overlap(grid, standby, offset_y, offset_x)) {
      current->position.x = offset_x * 2 + 1;
      current->position.y = offset_y;
      current->shape = standby;
    }
  }

  return grid_placement;
}

int handle_rotate_left(Matrix grid, Block *current) {
  Matrix standby = matrix_rotate_left(current->shape);
  return handle_rotate(grid, current, standby);
}

int handle_rotate_right(Matrix grid, Block *current) {
  Matrix standby = matrix_rotate_right(current->shape);
  return handle_rotate(grid, current, standby);
}

bool can_move_left(Matrix grid, Block current) {
  int offset_y = current.position.y - 1;
  int offset_x = (current.position.x - 1) / 2 - 1;

  return !is_block_overlap(grid, current.shape, offset_y, offset_x);
}

bool can_move_right(Matrix grid, Block current) {
  int offset_y = current.position.y - 1;
  int offset_x = (current.position.x - 1) / 2 + 1;

  return !is_block_overlap(grid, current.shape, offset_y, offset_x);
}

void dispatch(WINDOW *game_win, enum Action action, Block *current,
              Matrix grid) {
  block_wclear(game_win, *current);

  int grid_placement = get_grid_placement(grid, *current);
  // save current block position and color
  int block_y = current->position.y;
  int block_color = current->color;

  // clear ghost
  current->position.y = grid_placement + 1;
  current->color = 9;
  block_wclear(game_win, *current);

  // restore current block position and color
  current->position.y = block_y;
  current->color = block_color;

  switch (action) {
  case MOVE_RIGHT:
    if (can_move_right(grid, *current)) {
      current->position.x += 2;
      grid_placement = get_grid_placement(grid, *current);
    }
    break;
  case MOVE_LEFT:
    if (can_move_left(grid, *current)) {
      current->position.x -= 2;
      grid_placement = get_grid_placement(grid, *current);
    }
    break;
  case MOVE_DOWN:
    current->position.y++;
    break;
  case ROTATE_LEFT:
    grid_placement = handle_rotate_left(grid, current);
    break;
  case ROTATE_RIGHT:
    grid_placement = handle_rotate_right(grid, current);
    break;
  }

  // save current block position and color
  block_y = current->position.y;
  block_color = current->color;

  // print ghost
  current->position.y = grid_placement + 1;
  current->color = 9;
  block_wprint(game_win, *current);

  // restore current block position and color
  current->position.y = block_y;
  current->color = block_color;

  block_wprint(game_win, *current);
  wrefresh(game_win);
}

#endif
