#ifndef GAME_H
#define GAME_H

#include "../blocks.h"
#include "../lib.h"
#include "../matrix/matrix.h"
#include "../vec.h"
#include "state.h"

#include <assert.h>
#include <ncurses.h>

void game(enum Screen *game_state, Vec *highscores, int initial_level,
          bool is_constant_level);
void print_instructions(WINDOW *win);
void print_stats(State state);
void init_windows_dimensions();
void refresh_game_window(WINDOW *game_win, Matrix grid);
void refresh_next_window(WINDOW *next_win, Block queue[]);
void refresh_hold_window(WINDOW *hold_win, Block hold);
bool update_board(State *state, int initial_level, bool is_constant_level);
#endif
