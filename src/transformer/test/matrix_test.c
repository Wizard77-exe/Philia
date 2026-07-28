#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"

int main() {
  srand(time(NULL));
  
  Matrix a = matrix_create(2, 3);
  Matrix b = matrix_create(3, 3);

  matrix_fill(&a, 2);
  matrix_fill(&b, 3);

  Matrix I = matrix_identity(10);

  Matrix c = matrix_create(3, 3);
  matrix_random(&c);

  printf("SUM: %f\n", matrix_sum(a));
  printf("MEAN: %f\n", matrix_mean(a));
  printf("\n");
  matrix_pretty_print(c);
  printf("MAX: %f\n", matrix_max(c));
  printf("MIN: %f\n", matrix_min(c));
  printf("ARGMAX: %d\n", matrix_argmax(c));

  Matrix x = matrix_create(2, 3);
  Matrix y = matrix_create(2, 3);
  matrix_fill(&x, 2);
  matrix_fill(&y, 2);

  Matrix z = matrix_hadamard(x, y);

  matrix_pretty_print(z);

  matrix_free(&a);
  matrix_free(&b);
  matrix_free(&I);
  matrix_free(&c);

  matrix_free(&x);
  matrix_free(&y);
  matrix_free(&z);

  return 0;
}
