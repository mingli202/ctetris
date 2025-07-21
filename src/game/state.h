#ifndef STATE_H
#define STATE_H

#include "../blocks.h"
#include <ncurses.h>

typedef struct State {
  Block *queue;
  Block current;
  Block hold;
  Matrix grid;
  int score;
  int level;
  int lines;
  int combo_count;
  bool did_hold;
  int grid_placement;
  WINDOW *window;
  WINDOW *game_window;
  WINDOW *hold_window;
  WINDOW *next_window;
} State;

State state_new();
void state_init_queue(State *state);
int state_update_grid(State *state);
int state_swap_hold(State *state);

WINDOW *create_window_with_box(int height, int width, int y, int x,
                               char word[]);
void update_current(WINDOW *next_win, WINDOW *game_win, Block *queue,
                    Block *current);

#endif
