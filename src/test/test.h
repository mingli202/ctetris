#ifndef TEST_H
#define TEST_H

#include "../matrix/matrix.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct Result {
  bool success;
  char *reason;
  char *desc;
  char *name;
} Result;

void reset() { printf("\033[0m"); }

void red() { printf("\033[1;31m"); }

void green() { printf("\033[0;32m"); }

void fail(char *name, char *reason, char *description) {
  printf("%s...", name);
  red();
  printf("FAILED: %s ", reason);
  reset();
  printf("%s\n", description);
}

void success(char *name) {
  printf("%s...", name);
  green();
  printf("SUCCESS");
  reset();
  printf("\n");
}

Result matrix_compare(Matrix a, Matrix b, char *name) {
  if (a.m != b.m || a.n != b.n) {
    char desc[50];
    sprintf(desc, "A has shape %ix%i but B has shape %ix%i", a.n, a.m, b.n,
            b.m);

    Result res = {.success = false,
                  .reason = "Incompatible shape",
                  .desc = desc,
                  .name = name};
    return res;
  }

  for (int i = 0; i < a.m; i++) {
    for (int k = 0; k < a.n; k++) {
      if (matrix_get(a, i, k) != matrix_get(b, i, k)) {
        Result res = {.success = false,
                      .reason = "Comparison Error",
                      .desc = "Matrices are not equal!",
                      .name = name};

        matrix_print(a);
        matrix_print(b);
        return res;
      }
    }
  }

  Result res = {.success = true, .name = name};
  return res;
}

void assert_res(Result res) {
  if (res.success) {
    success(res.name);
  } else {
    fail(res.name, res.reason, res.desc);
  }
}

void assert_bool(bool exp, char *name) {
  if (exp) {
    success(name);
  } else {
    fail(name, "False", "");
  }
}

#endif
