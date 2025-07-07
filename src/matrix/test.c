#include "../test/test.h"
#include "matrix.h"

void matrix_test() {
  Matrix A = matrix_new(2, 2);
  matrix_set(&A, 0, 0, 1);
  matrix_set(&A, 0, 1, 2);
  matrix_set(&A, 1, 0, 3);
  matrix_set(&A, 1, 1, 4);

  int a[] = {1, 2, 3, 4};
  assert_res(matrix_compare(A, matrix_from(2, 2, a), "from_and_set_1"));

  Matrix B = matrix_new(2, 3);
  matrix_set(&B, 0, 0, 1);
  matrix_set(&B, 0, 1, 2);
  matrix_set(&B, 0, 2, 3);
  matrix_set(&B, 1, 0, 4);
  matrix_set(&B, 1, 1, 5);
  matrix_set(&B, 1, 2, 6);

  int b[] = {1, 2, 3, 4, 5, 6};
  assert_res(matrix_compare(B, matrix_from(2, 3, b), "from_and_set_2"));

  Matrix C = matrix_mul(A, B);
  int c[] = {9, 12, 15, 19, 26, 33};
  assert_res(matrix_compare(C, matrix_from(2, 3, c), "matrix_mul"));

  Matrix D = matrix_transpose(C);
  int d[] = {9, 19, 12, 26, 15, 33};
  assert_res(matrix_compare(D, matrix_from(3, 2, d), "matrix_transpose"));

  int arr[] = {0, 1, 0, 1, 1, 1};
  Matrix E = matrix_from(2, 3, arr);

  int l[] = {0, 1, 1, 1, 0, 1};
  assert_res(matrix_compare(matrix_rotate_left(E), matrix_from(3, 2, l),
                            "rotate_left"));

  int r[] = {1, 0, 1, 1, 1, 0};
  assert_res(matrix_compare(matrix_rotate_right(E), matrix_from(3, 2, r),
                            "rotate_right"));
}

int main(void) {
  matrix_test();
  return 0;
}
