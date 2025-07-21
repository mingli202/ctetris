#include "state.h"
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
      .combo = -1,
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

WINDOW *create_window_with_box(int height, int width, int y, int x,
                               char word[]) {
  WINDOW *win = newwin(height, width, y, x);
  box(win, 0, 0);
  mvwprintw(win, 0, (width - strlen(word)) / 2, word);
  wrefresh(win);

  return win;
}

void update_current(WINDOW *next_win, WINDOW *game_win, Block *queue,
                    Block *current) {
  // things related to queue window
  attrset(A_NORMAL);
  for (int i = 1; i < 9; i++) {
    mvwprintw(next_win, i, 1, "          ");
  }

  *current = queue[0];
  block_center(dim_game.width, current);
  current->pos.y = 1;

  if (current->type == I) {
    current->pos.y = 0;
  }

  queue[0] = queue[1];
  queue[0].pos.y -= 3;

  queue[1] = queue[2];
  queue[1].pos.y -= 3;

  int last_color = queue[1].color;

  queue[2] = block_new();
  queue[2].pos.y = 2 * 3 + 1;

  update_next_window(next_win, queue);
}
