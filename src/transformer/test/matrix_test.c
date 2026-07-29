#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"

int main() {
  srand(time(NULL));
  
  Matrix a = matrix_create(1, 3);
  Matrix b = matrix_create(1, 2);

  a.data[0] = 1;
  a.data[1] = 2;
  a.data[2] = 3;
  b.data[0] = 4;
  b.data[1] = 5;

  Matrix c = matrix_outer(a, b);

  matrix_pretty_print(c);

  matrix_free(&a);
  matrix_free(&b);
  matrix_free(&c);

  return 0;
}
