#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "matrix.h"
#include "linear.h"

int main() {
  srand(time(NULL));
  
  Linear layer = linear_create(4, 3);

  float w[] = {
     1,  2,  3,  4,
     5,  6,  7,  8,
     9, 10, 11, 12
  };

  memcpy(layer.weights.data, w, sizeof(w));

  float b[] = {1, 2, 3};

  memcpy(layer.bias.data, b, sizeof(b));

  Matrix input = matrix_create(2, 4);
  
  float x[] = {
    1,1,1,1,
    2,2,2,2
  };

  memcpy(input.data, x, sizeof(x));

  Matrix output = linear_forward(&layer, input);

  printf("Weights: \n");
  matrix_pretty_print(layer.weights);
  printf("Bias: \n");
  matrix_pretty_print(layer.bias);
  printf("Input: \n");
  matrix_pretty_print(input);
  printf("Output: ");

  matrix_pretty_print(output);

  linear_free(&layer);
  matrix_free(&input);
  matrix_free(&output);

  return 0;
}
