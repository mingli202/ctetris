#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>

typedef struct Matrix {
  int m;
  int n;
  int *M;
} Matrix;

Matrix matrix_new(int m, int n);
Matrix matrix_from(int m, int n, int arr[]);
int matrix_get(Matrix matrix, int row, int column);
void matrix_set(Matrix *matrix, int row, int column, int value);
void matrix_print(Matrix matrix);
Matrix matrix_mul(Matrix A, Matrix B);
Matrix matrix_transpose(Matrix A);
Matrix matrix_rotate_left(Matrix A);
Matrix matrix_rotate_right(Matrix A);
bool is_row_full(Matrix grid, int row);
bool clear_row(Matrix *grid, int row);

#endif
