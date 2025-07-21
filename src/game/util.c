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
  int offset_y = current.position.y;
  int offset_x = (current.position.x - 1) / 2;

  while (!is_block_overlap(grid, current.shape, offset_y, offset_x)) {
    offset_y++;
  }

  return offset_y - 1;
}
