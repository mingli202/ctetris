#include "../blocks.h"
#include "../matrix/matrix.h"
#include "../vec.h"

int main(void) {
  Matrix m;

  m = block_get_shape(I);
  matrix_print(m);
  m = matrix_rotate_left(m);
  matrix_print(m);

  m = block_get_shape(L);
  matrix_print(m);
  m = matrix_rotate_left(m);
  matrix_print(m);

  Vec v = vec_new(10);
  // push in random order
  for (int i = 0; i < 10; i++) {
    int random = rand() % 10;
    vec_push(&v, random);
  }

  vec_print(v);
  vec_sort(&v);
  vec_print(v);

  // check if vec is sorted
  for (int i = 0; i < v.length - 1; i++) {
    assert(v.arr[i] <= v.arr[i + 1]);
  }

  return 0;
}
