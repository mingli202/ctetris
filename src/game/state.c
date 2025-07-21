#include "state.h"

void create_initial_queue(Block *queue) {
  int last_color = -1;

  for (int i = 0; i < 3; i++) {
    queue[i] = block_new();
    queue[i].position.y = 3 * i + 1;
  }
}

void update_current(WINDOW *next_win, WINDOW *game_win, Block *queue,
                    Block *current) {
  // things related to queue window
  attrset(A_NORMAL);
  for (int i = 1; i < 9; i++) {
    mvwprintw(next_win, i, 1, "          ");
  }

  *current = queue[0];
  block_center(W_GAME_WIDTH, current);
  current->position.y = 1;

  if (current->type == I) {
    current->position.y = 0;
  }

  queue[0] = queue[1];
  queue[0].position.y -= 3;

  queue[1] = queue[2];
  queue[1].position.y -= 3;

  int last_color = queue[1].color;

  queue[2] = block_new();
  queue[2].position.y = 2 * 3 + 1;

  update_next_window(next_win, queue);
}
