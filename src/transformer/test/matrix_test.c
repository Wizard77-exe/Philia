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
  
  Matrix d_weights = MATRIX_ERROR;
  Matrix d_bias = MATRIX_ERROR;
  Matrix d_input = MATRIX_ERROR;

  linear_backward(&layer, input, output, &d_weights, &d_bias, &d_input);

  printf("Weights:");
  matrix_pretty_print(layer.weights);
  printf("\nBias:");
  matrix_pretty_print(layer.bias);
  printf("\nInput:");
  matrix_pretty_print(input);
  printf("\nOutput: ");
  matrix_pretty_print(output);

  printf("\n\n");

  printf("dW:");
  matrix_pretty_print(d_weights);
  printf("\ndb:");
  matrix_pretty_print(d_bias);
  printf("\ndX:");
  matrix_pretty_print(d_input);

  matrix_free(&d_weights);
  matrix_free(&d_bias);
  matrix_free(&d_input);

  linear_free(&layer);
  matrix_free(&input);
  matrix_free(&output);

  return 0;
}
