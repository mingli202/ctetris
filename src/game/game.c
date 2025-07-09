#include "game.h"

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

void print_stats(WINDOW *win, int score, int level) {
  mvwprintw(win, LINES - 2, (COLS - dim_game.width) / 2 - dim_hold.width,
            "Score: %d", score);
  mvwprintw(win, LINES - 1, (COLS - dim_game.width) / 2 - dim_hold.width,
            "Level: %d", level);
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

bool update_board(WINDOW *game_win, WINDOW *next_win, WINDOW *win, Matrix *grid,
                  Block *current, Block *queue, int *score, bool *did_hold,
                  int *lines_cleared, int *combo_count) {
  int placement = get_grid_placement(*grid, *current);
  place_block(grid, *current, placement);

  // scoring
  int line_cleared = update_grid(grid);
  *lines_cleared += line_cleared;

  int level = *lines_cleared / 10 + 1;

  if (line_cleared > 0 && line_cleared <= 4) {
    int score_mapping[] = {100, 300, 500, 800};
    int s = score_mapping[line_cleared - 1] * (level);

    if (*combo_count > 0) {
      s += *combo_count * 50 * level;
    }

    *score += s;

    *combo_count += 1;
  } else {
    *combo_count = -1;
  }

  print_stats(win, *score, level);

  block_wclear(game_win, *current);
  print_game_win(game_win, *grid);
  update_current(next_win, game_win, queue, current);

  placement = get_grid_placement(*grid, *current);
  ghost_wprint(game_win, *current, placement);
  block_wprint(game_win, *current);
  wrefresh(game_win);

  *did_hold = false;

  int offset_y = current->position.y - 1;
  int offset_x = (current->position.x - 1) / 2;

  return !is_block_overlap(*grid, current->shape, offset_y, offset_x);
}

void game(enum State *game_state, Vec *highscores) {
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
  current.position.y = 1;
  block_center(dim_game, &current);

  update_next_window(next_win, queue);

  int tick = 3;
  int score = 0;
  int lines_cleared = 0;
  int combo_count = -1;
  int grid_placement = get_grid_placement(grid, current);
  print_stats(win, score, lines_cleared / 10 + 1);

  Block hold = block_new(NULL);
  hold.type = -1;
  bool did_hold = false;

  ghost_wprint(game_win, current, grid_placement);

  block_wprint(game_win, current);
  wrefresh(game_win);

  bool run = true;
  clock_t now, then, interval;
  now = clock();

  bool is_on_ground = false;

  while (run) {
    int ch = getch();

    switch (ch) {
    case 'q':
      quit();
      break;
    case ' ': {
      run = update_board(game_win, next_win, win, &grid, &current, queue,
                         &score, &did_hold, &lines_cleared, &combo_count);

      now = clock();
      break;
    }
    case KEY_LEFT:
      grid_placement = dispatch(game_win, MOVE_LEFT, &current, grid, &now);
      break;
    case KEY_RIGHT:
      grid_placement = dispatch(game_win, MOVE_RIGHT, &current, grid, &now);
      break;
    case KEY_DOWN: {
      grid_placement = dispatch(game_win, MOVE_DOWN, &current, grid, &now);
      break;
    }
    case 'z':
      grid_placement = dispatch(game_win, ROTATE_LEFT, &current, grid, &now);
      break;
    case KEY_UP:
      grid_placement = dispatch(game_win, ROTATE_RIGHT, &current, grid, &now);
      break;
    case 'c':
      if (!did_hold) {
        grid_placement = swap_hold(hold_win, game_win, next_win, &current,
                                   &hold, queue, grid);
        now = clock();
        did_hold = true;
      }
      break;
    }

    then = clock();
    interval = then - now;

    is_on_ground = current.position.y == grid_placement + 1;

    if (is_on_ground) {
      if (interval >= 0.5 * CLOCKS_PER_SEC) {
        run = update_board(game_win, next_win, win, &grid, &current, queue,
                           &score, &did_hold, &lines_cleared, &combo_count);

        now = then;
      }
    } else if (interval >=
               CLOCKS_PER_SEC * calculate_speed(lines_cleared / 10 + 1)) {
      grid_placement = dispatch(game_win, MOVE_DOWN, &current, grid, &now);
    }
  }

  *game_state = MENU;
  vec_push(highscores, score);

  delwin(win);
  delwin(game_win);
  delwin(hold_win);
  delwin(next_win);
}
