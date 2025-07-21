#ifndef ACTIONS_H
#define ACTIONS_H

#include "../blocks.h"
#include "../vec.h"
#include <ncurses.h>
#include <time.h>

enum Action { MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, ROTATE_LEFT, ROTATE_RIGHT };

void place_block(Matrix *grid, Block current, int placement);

int handle_rotate(Matrix grid, Block *current, Matrix standby, clock_t *now);
int handle_rotate_left(Matrix grid, Block *current, clock_t *now);
int handle_rotate_right(Matrix grid, Block *current, clock_t *now);

bool can_move_left(Matrix grid, Block current);
bool can_move_right(Matrix grid, Block current);
int dispatch(WINDOW *game_win, enum Action action, Block *current, Matrix grid,
             clock_t *now);

#endif
