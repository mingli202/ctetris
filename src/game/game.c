#include "game.h"
#include "../vec.h"
#include "actions.h"
#include "state.h"
#include "util.h"

#include <pthread.h>
#include <string.h>

void game(enum Screen *game_state, Vec *highscores, int initial_level,
          bool is_constant_level) {
  init_windows_dimensions();

  State state = state_new();

  wclear(state.window);
  wrefresh(state.window);
  print_instructions(state.window);

  refresh_next_window(state.next_window, state.queue);

  int grid_placement = state_get_grid_placement(&state);
  print_stats(state);

  ghost_wprint(state.game_window, state.current, grid_placement);

  block_wprint(state.game_window, state.current);
  wrefresh(state.game_window);

  bool run = true;
  clock_t now, then, interval;
  now = clock();

  // pthread_t listenter;
  // pthread_create(&listenter, NULL, init_listenter, NULL);

  while (run) {
    int ch = getch();

    switch (ch) {
    case 'q':
      quit();
      break;
    case ' ': {
      run = update_board(&state, initial_level, is_constant_level);

      now = clock();
      break;
    }
    case KEY_LEFT:
      grid_placement = dispatch(&state, MOVE_LEFT, &now);
      break;
    case KEY_RIGHT:
      grid_placement = dispatch(&state, MOVE_RIGHT, &now);
      break;
    case KEY_DOWN: {
      grid_placement = dispatch(&state, MOVE_DOWN, &now);
      break;
    }
    case 'z':
      grid_placement = dispatch(&state, ROTATE_LEFT, &now);
      break;
    case KEY_UP:
      grid_placement = dispatch(&state, ROTATE_RIGHT, &now);
      break;
    case 'c':
      if (!state.did_hold) {
        grid_placement = state_swap_hold(&state);
        now = clock();
        state.did_hold = true;
      }
      break;
    }

    then = clock();
    interval = then - now;

    if (state_is_on_ground(&state)) {
      if (interval >= 0.5 * CLOCKS_PER_SEC) {
        run = update_board(&state, initial_level, is_constant_level);

        now = then;
      }
    } else if (interval >=
               CLOCKS_PER_SEC * calculate_speed(state.lines / 10 + 1,
                                                initial_level,
                                                is_constant_level)) {
      grid_placement = dispatch(&state, MOVE_DOWN, &now);
    }
  }

  *game_state = MENU;
  vec_push(highscores, state.score);

  delwin(state.window);
  delwin(state.game_window);
  delwin(state.hold_window);
  delwin(state.next_window);
}

void init_windows_dimensions() {
  dim_game.y = (LINES - dim_game.height) / 2;
  dim_game.x = (COLS - dim_game.width) / 2;

  dim_hold.y = dim_game.y;
  dim_hold.x = dim_game.x - dim_hold.width;

  dim_next.y = dim_game.y;
  dim_next.x = dim_game.x + dim_game.width;
}

void print_instructions(WINDOW *win) {
  char *instructions_left[] = {
      "LEFT - move piece left",
      "RIGHT - move piece right",
      "DOWN - soft drop",
      "SPACE - hard drop",
  };

  for (int i = 0; i < 4; i++) {
    mvwprintw(win, i + 1, (COLS - 22) / 2 - 12, instructions_left[i]);
  }

  char *instructions_right[] = {
      "quit - q",
      "swap hold - c",
      "rotate left - z",
      "rotate right - UP",
  };

  for (int i = 0; i < 4; i++) {
    int len = strlen(instructions_right[i]);
    mvwprintw(win, i + 1, COLS / 2 + dim_game.width / 2 + dim_hold.width - len,
              instructions_right[i]);
  }
  wrefresh(win);
}

void print_stats(State state) {
  WINDOW *win = state.window;

  mvwprintw(win, LINES - 3, (COLS - dim_game.width) / 2 - dim_hold.width,
            "Score: %d", state.score);
  mvwprintw(win, LINES - 2, (COLS - dim_game.width) / 2 - dim_hold.width,
            "Level: %d", state.level);
  mvwprintw(win, LINES - 1, (COLS - dim_game.width) / 2 - dim_hold.width,
            "Lines: %d", state.lines);
  wrefresh(win);
}

void refresh_game_window(WINDOW *game_win, Matrix grid) {
  for (int i = 0; i < grid.m; i++) {
    for (int k = 0; k < grid.n; k++) {
      wattrset(game_win, COLOR_PAIR(matrix_get(grid, i, k)));
      mvwprintw(game_win, i + 1, k * 2 + 1, "  ");
    }
  }
  wattrset(game_win, A_NORMAL);
  wrefresh(game_win);
}

void refresh_next_window(WINDOW *next_win, Block queue[]) {
  attrset(A_NORMAL);
  for (int i = 1; i < 9; i++) {
    mvwprintw(next_win, i, 1, "          ");
  }

  for (int i = 0; i < 3; i++) {
    Matrix block = queue[i].shape;
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

void refresh_hold_window(WINDOW *hold_win, Block hold) {
  int x = 3;
  if (hold.type == I) {
    x = 2;
  } else if (hold.type == O) {
    x = 4;
  }
  hold.pos.y = 1;
  hold.pos.x = x;
  hold.shape = block_get_shape(hold.type);
  block_wprint(hold_win, hold);
  wrefresh(hold_win);
}

bool update_board(State *state, int initial_level, bool is_constant_level) {
  state_grid_place_block(state);

  // scoring
  int lines_cleared = state_update_grid(state);
  state_update_stats(state, initial_level, is_constant_level, lines_cleared);
  print_stats(*state);

  block_wclear(state->game_window, state->current);
  refresh_game_window(state->game_window, state->grid);
  state_next_block(state);

  int placement = state_get_grid_placement(state);
  ghost_wprint(state->game_window, state->current, placement);
  block_wprint(state->game_window, state->current);
  wrefresh(state->game_window);

  state->did_hold = false;

  int offset_y = state->current.pos.y - 1;
  int offset_x = (state->current.pos.x - 1) / 2;

  return !is_block_overlap(state->grid, state->current.shape, offset_y,
                           offset_x);
}
