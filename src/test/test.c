#include "test.h"
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
  vec_sort(&v);

  // check if vec is sorted
  for (int i = 0; i < v.length - 1; i++) {
    assert(v.arr[i] <= v.arr[i + 1]);
  }

  int arr[] = {1, 2, 3, 4, 5};
  int ar2[] = {5, 4, 3, 2, 1};
  Vec v2 = vec_from(arr, 5);
  vec_reverse(&v2);

  assert_res(compare_vec(v2, vec_from(ar2, 5), "vec_reverse"));

  v = vec_new(10);
  vec_push(&v, 300);
  vec_push(&v, 100);
  vec_push(&v, 200);
  vec_sort(&v);

  int expected[] = {100, 200, 300};
  assert_res(compare_vec(v, vec_from(expected, 3), "vec_sort"));

  return 0;
}
