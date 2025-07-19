#ifndef LIB_H
#define LIB_H

#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

enum State { MENU, GAME };

void init();
void quit();
void print_help();
void parse_args(int argc, char **argv, int *level, bool *is_constant_level);

#endif
