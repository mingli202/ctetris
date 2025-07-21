#include "actions.h"
#include "util.h"

void place_block(Matrix *grid, Block current, int placement) {
  int offset_y = placement;
  int offset_x = (current.pos.x - 1) / 2;

  for (int i = 0; i < current.shape.m; i++) {
    for (int k = 0; k < current.shape.n; k++) {
      if (matrix_get(current.shape, i, k) == 1) {
        matrix_set(grid, i + offset_y, k + offset_x, current.color);
      }
    }
  }
}

int handle_rotate(Matrix grid, Block *current, Matrix standby, clock_t *now) {
  int offset_y = current->pos.y - 1;
  int offset_x = (current->pos.x - 1) / 2;

  int tries[] = {0, -1, 1};

  int grid_placement;

  for (int i = 0; i < 3; i++) {
    if (!is_block_overlap(grid, standby, offset_y, offset_x + tries[i])) {
      current->pos.x = (offset_x + tries[i]) * 2 + 1;
      current->shape = standby;

      grid_placement = get_grid_placement(grid, *current);

      if (current->pos.y == grid_placement + 1) {
        *now = clock();
      }

      return grid_placement;
    }
  }

  offset_y++;

  for (int i = 0; i < 3; i++) {
    if (!is_block_overlap(grid, standby, offset_y, offset_x + tries[i])) {
      current->pos.x = (offset_x + tries[i]) * 2 + 1;
      current->pos.y = offset_y + 1;
      current->shape = standby;

      grid_placement = get_grid_placement(grid, *current);

      if (current->pos.y == grid_placement + 1) {
        *now = clock();
      }

      return grid_placement;
    }
  }

  offset_y--;

  grid_placement = get_grid_placement(grid, *current);

  if (current->pos.y == grid_placement + 1) {
    offset_y--;
    if (!is_block_overlap(grid, standby, offset_y, offset_x)) {
      current->pos.y = offset_y + 1;
      current->shape = standby;

      grid_placement = get_grid_placement(grid, *current);

      if (current->pos.y == grid_placement + 1) {
        *now = clock();
      }

      return grid_placement;
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
  int offset_y = current.pos.y - 1;
  int offset_x = (current.pos.x - 1) / 2 - 1;

  return !is_block_overlap(grid, current.shape, offset_y, offset_x);
}

bool can_move_right(Matrix grid, Block current) {
  int offset_y = current.pos.y - 1;
  int offset_x = (current.pos.x - 1) / 2 + 1;

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
      current->pos.x += 2;
      grid_placement = get_grid_placement(grid, *current);

      if (current->pos.y == grid_placement + 1) {
        *now = clock();
      }
    }
    break;
  case MOVE_LEFT:
    if (can_move_left(grid, *current)) {
      current->pos.x -= 2;
      grid_placement = get_grid_placement(grid, *current);

      if (current->pos.y == grid_placement + 1) {
        *now = clock();
      }
    }
    break;
  case MOVE_DOWN:
    if (current->pos.y != grid_placement + 1) {
      current->pos.y++;
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
