#include "state.h"
#include "game.h"
#include "util.h"
#include <string.h>

State state_new() {
  Block queue[3];
  State state = {
      .queue = queue,
      .current = block_new(),
      .hold = {.type = NONE},
      .grid = matrix_new(20, 10),
      .score = 0,
      .level = 1,
      .lines = 0,
      .combo_count = -1,
      .did_hold = false,
      .window = newwin(0, 0, 0, 0),
      .game_window = create_window_with_box(dim_game.height, dim_game.width,
                                            dim_game.y, dim_game.x, "Play"),
      .hold_window = create_window_with_box(dim_hold.height, dim_hold.width,
                                            dim_hold.y, dim_hold.x, "Hold"),
      .next_window = create_window_with_box(dim_next.height, dim_next.width,
                                            dim_next.y, dim_next.x, "Next"),
  };

  state_init_queue(&state);

  return state;
}

void state_init_queue(State *state) {
  for (int i = 0; i < 3; i++) {
    state->queue[i] = block_new();
    state->queue[i].pos.y = 3 * i + 1;
  }
}

int state_update_grid(State *state) {
  int front = 19;
  int back = 19;

  for (; front >= 0; front--) {
    bool is_full = is_row_full(state->grid, front);

    if (!is_full) {
      for (int i = 0; i < state->grid.n; i++) {
        matrix_set(&state->grid, back, i, matrix_get(state->grid, front, i));
      }
      back--;
    }
  }

  int score = back - front;

  for (; back >= 0; back--) {
    clear_row(&state->grid, back);
  }

  return score;
};

void state_update_stats(State *state, int initial_level, bool is_constant_level,
                        int lines_cleared) {
  state->lines += lines_cleared;

  int level = state->lines / 10 + 1;
  if (is_constant_level || (initial_level > 0 && level < initial_level)) {
    level = initial_level;
  }

  if (lines_cleared > 0 && lines_cleared <= 4) {
    int score_mapping[] = {100, 300, 500, 800};
    int s = score_mapping[lines_cleared - 1] * (level);

    if (state->combo_count > 0) {
      s += state->combo_count * 50 * level;
    }

    state->score += s;

    state->combo_count += 1;
  } else {
    state->combo_count = -1;
  }
}

int state_swap_hold(State *state) {
  int grid_placement = get_grid_placement(state->grid, state->current);
  block_wclear(state->game_window, state->current);
  ghost_wclear(state->game_window, state->current, grid_placement);

  if (state->hold.type == -1) {
    state->hold = block_new();
    state->hold.color = state->current.color;
    state->hold.shape = state->current.shape;
    state->hold.type = state->current.type;
    state->hold.pos.y = 2;

    update_current(state->next_window, state->game_window, state->queue,
                   &state->current);
  } else {
    block_wclear(state->hold_window, state->hold);
    Block tmp = state->hold;
    state->hold = state->current;
    state->current = tmp;
  }

  if (state->current.type == I) {
    state->current.pos.y = 0;
  }

  grid_placement = get_grid_placement(state->grid, state->current);

  ghost_wprint(state->game_window, state->current, grid_placement);
  block_wprint(state->game_window, state->current);

  update_hold_window(state->hold_window, &state->hold);
  wrefresh(state->game_window);

  state->grid_placement = grid_placement;
}

void state_next_block(State *state) {
  state->current = state->queue[0];
  state->queue[0] = state->queue[1];
  state->queue[1] = state->queue[2];
  state->queue[2] = block_new();
}

int state_get_grid_placement(State *state) {

  int offset_y = state->current.pos.y;
  int offset_x = (state->current.pos.x - 1) / 2;

  while (!is_block_overlap(state->grid, state->current.shape, offset_y,
                           offset_x)) {
    offset_y++;
  }

  int grid_placement = offset_y - 1;

  state->grid_placement = grid_placement;

  return grid_placement;
}

bool state_is_on_ground(State *state) {
  return state->current.pos.y == state->grid_placement + 1;
}

void state_grid_place_block(State *state) {
  int placement = state_get_grid_placement(state);

  int offset_y = placement;
  int offset_x = state->current.pos.x / 2;

  for (int i = 0; i < state->current.shape.m; i++) {
    for (int k = 0; k < state->current.shape.n; k++) {
      if (matrix_get(state->current.shape, i, k) == 1) {
        matrix_set(&state->grid, i + offset_y, k + offset_x,
                   state->current.color);
      }
    }
  }
}

WINDOW *create_window_with_box(int height, int width, int y, int x,
                               char word[]) {
  WINDOW *win = newwin(height, width, y, x);
  box(win, 0, 0);
  mvwprintw(win, 0, (width - strlen(word)) / 2, word);
  wrefresh(win);

  return win;
}
