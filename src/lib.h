#ifndef LIB_H
#define LIB_H

#define W_GAME_HEIGHT 22
#define W_GAME_WIDTH 22
#define BOX_SIZE 2

#define W_HOLD_HEIGHT 4
#define W_HOLD_WIDTH 12

#define W_NEXT_HEIGHT 10
#define W_NEXT_WIDTH 12

enum Screen { MENU, GAME };

void init();
void quit();
void print_help();
void parse_args(int argc, char **argv, int *level, bool *is_constant_level);

#endif
