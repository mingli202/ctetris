#ifndef ACTIONS_H
#define ACTIONS_H

#include "../blocks.h"
#include "../vec.h"
#include "state.h"
#include <ncurses.h>
#include <time.h>

enum Action { MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, ROTATE_LEFT, ROTATE_RIGHT };

void *init_listenter(void *args);

int handle_rotate(Matrix grid, Block *current, Matrix standby, clock_t *now);
int handle_rotate_left(Matrix grid, Block *current, clock_t *now);
int handle_rotate_right(Matrix grid, Block *current, clock_t *now);

bool can_move_left(Matrix grid, Block current);
bool can_move_right(Matrix grid, Block current);
int dispatch(State *state, enum Action action, clock_t *now);

#endif
