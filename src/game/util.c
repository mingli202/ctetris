#include "util.h"

bool is_block_overlap(Matrix grid, Matrix shape, int offset_y, int offset_x) {
  for (int i = 0; i < shape.m; i++) {
    for (int k = 0; k < shape.n; k++) {
      if (matrix_get(shape, i, k) == 1) {
        if (!(i + offset_y >= 0 && i + offset_y < grid.m) ||
            !(k + offset_x >= 0 && k + offset_x < grid.n) ||
            matrix_get(grid, i + offset_y, k + offset_x) != 0) {
          return true;
        }
      }
    }
  }

  return false;
}

int get_grid_placement(Matrix grid, Block current) {
  int offset_y = current.pos.y;
  int offset_x = (current.pos.x - 1) / 2;

  while (!is_block_overlap(grid, current.shape, offset_y, offset_x)) {
    offset_y++;
  }

  return offset_y - 1;
}

double calculate_speed(int level, int initial_level, bool is_constant_level) {
  level--;

  if (is_constant_level || (initial_level > 0 && level < initial_level)) {
    level = initial_level;
  }

  double speed_curve[] = {
      1.0,         0.793,       0.617796,    0.472729139, 0.355196928,
      0.26200355,  0.189677245, 0.134734731, 0.093882249, 0.064151585,
      0.042976258, 0.028217678, 0.018153329, 0.011439342, 0.007058616,
      0.004263557, 0.002520084, 0.001457139, 0.000823907, 0.000455398,
  };

  if (level > 19) {
    level = 19;
  }

  return speed_curve[level];
}
