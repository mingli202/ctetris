#ifndef VEC_H
#define VEC_H

#define DEFAULT_CAPACITY 10

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Vec {
  int *arr;
  int length;
  int capacity;
} Vec;

Vec vec_with_capacity(int capacity);
Vec vec_new();
Vec vec_from(int arr[], int length);
void vec_push(Vec *vec, int value);
int vec_pop(Vec *vec);
void vec_sort(Vec *vec);
int vec_get(Vec vec, int index);
int vec_get_last(Vec vec);
void vec_reverse(Vec *vec);
void vec_print(Vec vec);
void quick_sort(int arr[], int left, int right);

#endif
