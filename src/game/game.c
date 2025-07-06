#include "game.h"

void print_instructions(WINDOW *win) {
  char *instructions[] = {
      "LEFT - move piece left", "RIGHT - move piece right",
      "DOWN - soft drop",       "SPACE - hard drop",
      "z - rotate left",        "x - rotate right",
      "c - swap hold",          "q - quit",
  };

  for (int i = 0; i < 8; i++) {
    mvwprintw(win, i + 1, (COLS - 22) / 2 - 12, instructions[i]);
  }
  wrefresh(win);
}

WINDOW *create_window_box(int height, int width, int y, int x, char word[]) {
  WINDOW *win = newwin(height, width, y, x);
  box(win, 0, 0);
  mvwprintw(win, 0, (width - strlen(word)) / 2, word);
  wrefresh(win);

  return win;
}

void print_game_win(WINDOW *game_win, Matrix grid) {
  for (int i = 0; i < grid.m; i++) {
    for (int k = 0; k < grid.n; k++) {
      wattrset(game_win, COLOR_PAIR(matrix_get(grid, i, k)));
      mvwprintw(game_win, i + 1, k * 2 + 1, "  ");
    }
  }
  wattrset(game_win, A_NORMAL);
  wrefresh(game_win);
}

int update_board(WINDOW *game_win, WINDOW *next_win, Matrix *grid,
                 Block *current, Block *queue) {
  int placement = get_placement(*grid, *current);
  place_block(grid, *current, placement);
  int score = update_grid(grid);
  block_wclear(game_win, *current);
  print_game_win(game_win, *grid);
  update_current(next_win, game_win, queue, current);

  return score;
}

void game(enum State *game_state) {
  WINDOW *win = newwin(0, 0, 0, 0);
  wclear(win);
  wrefresh(win);

  print_instructions(win);

  dim_game.y = (LINES - dim_game.height) / 2;
  dim_game.x = (COLS - dim_game.width) / 2;

  dim_hold.y = dim_game.y;
  dim_hold.x = dim_game.x - dim_hold.width;

  dim_next.y = dim_game.y;
  dim_next.x = dim_game.x + dim_game.width;

  WINDOW *game_win = create_window_box(dim_game.height, dim_game.width,
                                       dim_game.y, dim_game.x, "Play");
  WINDOW *hold_win = create_window_box(dim_hold.height, dim_hold.width,
                                       dim_hold.y, dim_hold.x, "Hold");

  WINDOW *next_win = create_window_box(dim_next.height, dim_next.width,
                                       dim_next.y, dim_next.x, "Next");

  Matrix grid = matrix_new(20, 10);

  Block queue[3];
  create_initial_queue(queue);

  int last_color = queue[0].color;
  Block current = block_new(&last_color);
  current.position.x = 5;

  update_next_window(next_win, queue);

  bool run = true;
  clock_t now, then;
  now = clock();

  int tick = 3;

  int score = 0;

  while (run) {
    int ch = getch();

    switch (ch) {
    case 'q':
      run = false;
      break;
    case ' ':
      score += update_board(game_win, next_win, &grid, &current, queue);
      break;
    case KEY_LEFT:
      dispatch(game_win, MOVE_LEFT, &current, grid);
      break;
    case KEY_RIGHT:
      dispatch(game_win, MOVE_RIGHT, &current, grid);
      break;
    case KEY_DOWN: {
      int placement = get_placement(grid, current);

      if (current.position.y + current.shape.m < dim_game.height - 1 &&
          current.position.y != placement + 1) {
        dispatch(game_win, MOVE_DOWN, &current, grid);
      }
      break;
    }
    case 'z':
      dispatch(game_win, ROTATE_LEFT, &current, grid);
      break;
    case 'x':
      dispatch(game_win, ROTATE_RIGHT, &current, grid);
      break;
    }

    then = clock();

    if (1 * (then - now) / CLOCKS_PER_SEC >= 1) {
      int placement = get_placement(grid, current);

      if (current.position.y != placement + 1) {
        dispatch(game_win, MOVE_DOWN, &current, grid);
        tick = 3;
      } else {
        if (tick < 0) {
          score += update_board(game_win, next_win, &grid, &current, queue);
          tick = 2;
        } else {
          tick--;
        }
      }
      now = then;
    }
  }

  delwin(win);
  delwin(game_win);
  delwin(hold_win);
  delwin(next_win);
  quit();
}
