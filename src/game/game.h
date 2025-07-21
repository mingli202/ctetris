#ifndef GAME_H
#define GAME_H

#include "../blocks.h"
#include "../lib.h"
#include "../matrix/matrix.h"
#include "../vec.h"

#include <assert.h>
#include <ncurses.h>

void game(enum Screen *game_state, Vec *highscores, int initial_level,
          bool is_constant_level);
void print_instructions(WINDOW *win);
void init_windows_dimensions();

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

int update_grid(Matrix *grid) {
  int front = 19;
  int back = 19;

  for (; front >= 0; front--) {
    bool is_full = is_row_full(*grid, front);

    if (!is_full) {
      for (int i = 0; i < grid->n; i++) {
        matrix_set(grid, back, i, matrix_get(*grid, front, i));
      }
      back--;
    }
  }

  int score = back - front;

  for (; back >= 0; back--) {
    clear_row(grid, back);
  }

  return score;
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
    *hold = block_new();
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

#endif
