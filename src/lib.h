#ifndef LIB_H
#define LIB_H

typedef struct Dimensions {
  int height;
  int width;
  int x;
  int y;
  int box;
} Dimensions;

Dimensions dim_game = {.height = 22, .width = 22, .box = 2};

Dimensions dim_hold = {.height = 4, .width = 12, .box = 2};

Dimensions dim_next = {.height = 10, .width = 12, .box = 2};

enum Screen { MENU, GAME };

void init();
void quit();
void print_help();
void parse_args(int argc, char **argv, int *level, bool *is_constant_level);

#endif
