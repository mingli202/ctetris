#ifndef UTIL_H
#define UTIL_H

#include "../blocks.h"
#include "../matrix/matrix.h"

bool is_block_overlap(Matrix grid, Matrix shape, int offset_y, int offset_x);
double calculate_speed(int level, int initial_level, bool is_constant_level);

#endif
