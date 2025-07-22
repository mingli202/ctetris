#include "actions.h"
#include "util.h"

void *init_listenter(void *args) {
  while (1) {
    int ch = getch();
  }
};

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

int dispatch(State *state, enum Action action, clock_t *now) {
  block_wclear(state->game_window, state->current);

  int grid_placement = state_get_grid_placement(state);
  ghost_wclear(state->game_window, state->current, grid_placement);

  switch (action) {
  case MOVE_RIGHT:
    if (can_move_right(state->grid, state->current)) {
      state->current.pos.x += 1;
      grid_placement = state_get_grid_placement(state);

      if (state_is_on_ground(state)) {
        *now = clock();
      }
    }
    break;
  case MOVE_LEFT:
    if (can_move_left(state->grid, state->current)) {
      state->current.pos.x -= 1;
      grid_placement = state_get_grid_placement(state);

      if (state_is_on_ground(state)) {
        *now = clock();
      }
    }
    break;
  case MOVE_DOWN:
    if (!state_is_on_ground(state)) {
      state->current.pos.y++;
      *now = clock();
    }
    break;
  case ROTATE_LEFT:
    grid_placement = handle_rotate_left(state->grid, &state->current, now);
    break;
  case ROTATE_RIGHT:
    grid_placement = handle_rotate_right(state->grid, &state->current, now);
    break;
  }

  ghost_wprint(state->game_window, state->current, grid_placement);

  block_wprint(state->game_window, state->current);
  wrefresh(state->game_window);

  return grid_placement;
}
