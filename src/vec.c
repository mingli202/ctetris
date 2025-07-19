#include "vec.h"

Vec vec_with_capacity(int capacity) {
  assert(capacity > 0);

  Vec R = {.arr = (int *)malloc(capacity * sizeof(int)),
           .length = 0,
           .capacity = capacity};

  assert(R.arr != NULL);

  return R;
}

Vec vec_new() {
  Vec R = {.arr = (int *)malloc(DEFAULT_CAPACITY * sizeof(int)),
           .length = 0,
           .capacity = DEFAULT_CAPACITY};

  assert(R.arr != NULL);

  return R;
}

Vec vec_from(int arr[], int length) {
  Vec vec = vec_with_capacity(length);

  for (int i = 0; i < length; i++) {
    vec_push(&vec, arr[i]);
  }

  return vec;
}

void vec_push(Vec *vec, int value) {
  if (vec->length == vec->capacity) {
    vec->capacity += 3;
    vec->arr = (int *)realloc(vec->arr, vec->capacity * sizeof(int));
  }

  vec->arr[vec->length] = value;
  vec->length++;
}

int vec_pop(Vec *vec) {
  assert(vec->length > 0);

  int value = vec->arr[vec->length - 1];

  vec->length--;

  return value;
}

void vec_sort(Vec *vec) {
  int *arr = vec->arr;

  quick_sort(arr, 0, vec->length - 1);
}

int vec_get(Vec vec, int index) {
  assert(index < vec.length);
  assert(index >= 0);

  return vec.arr[index];
}

int vec_get_last(Vec vec) {
  assert(vec.length > 0);

  return vec.arr[vec.length - 1];
}

void vec_reverse(Vec *vec) {
  int *arr = vec->arr;

  for (int i = 0; i < vec->length / 2; i++) {
    int temp = arr[i];
    arr[i] = arr[vec->length - 1 - i];
    arr[vec->length - 1 - i] = temp;
  }
}

void vec_print(Vec vec) {
  printf("[");
  for (int i = 0; i < vec.length; i++) {
    printf("%i ", vec.arr[i]);
  }
  printf("]\n");
}

void quick_sort(int arr[], int left, int right) {
  // if the array has only one element, it is already sorted
  if (left >= right)
    return;

  int pivot = arr[right];
  int i = left;

  for (int k = left; k < right; k++) {
    if (arr[k] < pivot) {
      int temp = arr[i];
      arr[i] = arr[k];
      arr[k] = temp;

      i++;
    }
  }

  int temp = arr[i];
  arr[i] = arr[right];
  arr[right] = temp;

  quick_sort(arr, left, i - 1);
  quick_sort(arr, i + 1, right);
}
