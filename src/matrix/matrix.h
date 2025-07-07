#ifndef MATRIX_H
#define MATRIX_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix {
  int m;
  int n;
  int *M;
} Matrix;

Matrix matrix_new(int m, int n) {
  Matrix R = {.m = m, .n = n, .M = (int *)calloc(m * n, sizeof(int))};

  assert(R.M != NULL);

  return R;
}

Matrix matrix_from(int m, int n, int arr[]) {
  Matrix R = {.m = m, .n = n, .M = malloc(m * n * sizeof(int))};

  assert(R.M != NULL);

  for (int i = 0; i < m * n; i++) {
    R.M[i] = arr[i];
  }

  return R;
}

int matrix_get(Matrix matrix, int row, int column) {
  assert(row < matrix.m);
  assert(row >= 0);
  assert(column < matrix.n);
  assert(column >= 0);

  int i = row * matrix.n + column;

  assert(i < matrix.m * matrix.n);
  assert(matrix.M != NULL);

  return matrix.M[i];
}

void matrix_set(Matrix *matrix, int row, int column, int value) {
  assert(row < matrix->m);
  assert(row >= 0);
  assert(column < matrix->n);
  assert(column >= 0);

  int i = row * matrix->n + column;

  assert(i < matrix->m * matrix->n);

  matrix->M[i] = value;
}

void matrix_print(Matrix matrix) {
  printf("[\n");
  for (int i = 0; i < matrix.m; i++) {
    printf("  ");
    for (int k = 0; k < matrix.n; k++) {
      printf("%i ", matrix_get(matrix, i, k));
    }
    printf("\n");
  }
  printf("]\n");
}

Matrix matrix_mul(Matrix A, Matrix B) {
  assert(A.n == B.m);

  Matrix R = matrix_new(A.m, B.n);

  for (int i = 0; i < A.m; i++) {
    for (int k = 0; k < B.n; k++) {
      int sum = 0;

      for (int w = 0; w < A.n; w++) {
        sum += matrix_get(A, i, w) * matrix_get(B, w, k);
      }
      matrix_set(&R, i, k, sum);
    }
  }

  return R;
}

Matrix matrix_transpose(Matrix A) {
  Matrix R = matrix_new(A.n, A.m);

  for (int i = 0; i < A.m; i++) {
    for (int k = 0; k < A.n; k++) {
      matrix_set(&R, k, i, matrix_get(A, i, k));
    }
  }

  return R;
}

Matrix matrix_rotate_left(Matrix A) {
  Matrix R = matrix_new(A.n, A.m);

  for (int i = 0; i < A.m; i++) {
    for (int k = 0; k < A.n; k++) {
      matrix_set(&R, A.n - k - 1, i, matrix_get(A, i, k));
    }
  }

  return R;
}

Matrix matrix_rotate_right(Matrix A) {
  Matrix R = matrix_new(A.n, A.m);

  for (int i = 0; i < R.m; i++) {
    for (int k = 0; k < R.n; k++) {
      matrix_set(&R, i, k, matrix_get(A, R.n - 1 - k, i));
    }
  }

  return R;
}

#endif
