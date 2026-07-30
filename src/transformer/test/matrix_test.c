#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"

int main() {
  srand(time(NULL));
  
  Matrix a = matrix_create(3, 3);
  a.data[0] = 1;
  a.data[1] = 2;
  a.data[2] = 3;
  a.data[3] = 2;
  a.data[4] = 4;
  a.data[5] = 5;
  a.data[6] = 3;
  a.data[7] = 5;
  a.data[8] = 6;
  matrix_pretty_print(a);

  printf("Is Square: %s\n", matrix_is_square(a) ? "true" : "false");
  printf("Is Identity: %s\n", matrix_is_identity(a) ? "true" : "false");
  printf("Is Symmetric: %s\n", matrix_is_symmetric(a) ? "true" : "false");

  matrix_free(&a);

  return 0;
}
