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
void state_next_block(State *state);
int state_update_grid(State *state);
int state_swap_hold(State *state);
int state_get_grid_placement(State *state);
bool state_is_on_ground(State *state);

void state_grid_place_block(State *state);
void state_update_stats(State *state, int initial_level, bool is_constant_level,
                        int lines_cleared);

WINDOW *create_window_with_box(int height, int width, int y, int x,
                               char word[]);

#endif
