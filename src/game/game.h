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
  block_center(dim_game, current);
  current->position.y = 2;

  if (current->type == I) {
    current->position.y = 1;
  }

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

int handle_rotate(Matrix grid, Block *current, Matrix standby, clock_t *now) {
  int offset_y = current->position.y - 1;
  int offset_x = (current->position.x - 1) / 2;

  int tries[] = {0, -1, 1};

  for (int i = 0; i < 3; i++) {
    if (!is_block_overlap(grid, standby, offset_y, offset_x + tries[i])) {
      current->position.x = (offset_x + tries[i]) * 2 + 1;
      current->shape = standby;
      *now = clock();
      break;
    }
  }

  int grid_placement = get_grid_placement(grid, *current);

  if (current->position.y == grid_placement + 1) {
    offset_y -= 1;
    if (!is_block_overlap(grid, standby, offset_y, offset_x)) {
      current->position.y = offset_y + 1;
      current->shape = standby;
      grid_placement = get_grid_placement(grid, *current);
      *now = clock();
    }
  }

  return grid_placement;
}

int handle_rotate_left(Matrix grid, Block *current, clock_t *now) {
  Matrix standby = matrix_rotate_left(current->shape);
  return handle_rotate(grid, current, standby, now);
}

int handle_rotate_right(Matrix grid, Block *current, clock_t *now) {
  Matrix standby = matrix_rotate_right(current->shape);
  return handle_rotate(grid, current, standby, now);
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

int dispatch(WINDOW *game_win, enum Action action, Block *current, Matrix grid,
             clock_t *now) {
  block_wclear(game_win, *current);

  int grid_placement = get_grid_placement(grid, *current);
  ghost_wclear(game_win, *current, grid_placement);

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
    if (current->position.y != grid_placement + 1) {
      current->position.y++;
      *now = clock();
    }
    break;
  case ROTATE_LEFT:
    grid_placement = handle_rotate_left(grid, current, now);
    break;
  case ROTATE_RIGHT:
    grid_placement = handle_rotate_right(grid, current, now);
    break;
  }

  ghost_wprint(game_win, *current, grid_placement);

  block_wprint(game_win, *current);
  wrefresh(game_win);

  return grid_placement;
}

void update_hold_window(WINDOW *hold_win, Block *hold) {
  int x = 3;
  if (hold->type == I) {
    x = 2;
  } else if (hold->type == O) {
    x = 4;
  }
  hold->position.y = 1;
  hold->position.x = x;
  hold->shape = block_get_shape(hold->type);
  block_wprint(hold_win, *hold);
  wrefresh(hold_win);
}

int swap_hold(WINDOW *hold_win, WINDOW *game_win, WINDOW *next_win,
              Block *current, Block *hold, Block queue[], Matrix grid) {
  int grid_placement = get_grid_placement(grid, *current);
  block_wclear(game_win, *current);
  ghost_wclear(game_win, *current, grid_placement);

  if (hold->type == -1) {
    *hold = block_new(NULL);
    hold->color = current->color;
    hold->shape = current->shape;
    hold->type = current->type;
    hold->position.y = 2;

    update_current(next_win, game_win, queue, current);
  } else {
    block_wclear(hold_win, *hold);
    Block tmp = *hold;
    *hold = *current;
    *current = tmp;
  }
  block_center(dim_game, current);
  if (current->type == I) {
    current->position.y = 0;
  }

  grid_placement = get_grid_placement(grid, *current);

  ghost_wprint(game_win, *current, grid_placement);
  block_wprint(game_win, *current);

  update_hold_window(hold_win, hold);
  wrefresh(game_win);

  return grid_placement;
}

double calculate_speed(int level) {
  level--;

  double speed_curve[] = {
      1.0,         0.793,       0.617796,    0.472729139, 0.355196928,
      0.26200355,  0.189677245, 0.134734731, 0.093882249, 0.064151585,
      0.042976258, 0.028217678, 0.018153329, 0.011439342, 0.007058616,
      0.004263557, 0.002520084, 0.001457139, 0.000823907, 0.000455398,
  };

  if (level > 19) {
    level = 19;
  }

  return speed_curve[level];
}

#endif
