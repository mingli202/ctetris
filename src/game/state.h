#ifndef STATE_H
#define STATE_H

#include "../blocks.h"
#include <ncurses.h>

typedef struct State {
  Block queue[3];
  Block current;
  Block hold;
  Matrix grid;
  int score;
  int level;
  int lines;
} State;

State state_new();

void create_initial_queue(Block *queue);
void update_current(WINDOW *next_win, WINDOW *game_win, Block *queue,
                    Block *current);

#endif
