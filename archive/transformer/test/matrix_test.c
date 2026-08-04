#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "matrix.h"
#include "linear.h"

int main() {
  srand(time(NULL));

  Linear layer = linear_create(2, 3);

/* Weights (3 x 2) */
float W[] = {
    1, 2,
    3, 4,
    5, 6
};

memcpy(layer.weights.data, W, sizeof(W));

/* Bias (1 x 3) */
float B[] = {
    0, 0, 0
};

memcpy(layer.bias.data, B, sizeof(B));

/* Input (2 x 2) */
Matrix input = matrix_create(2, 2);

float X[] = {
    1, 2,
    3, 4
};

memcpy(input.data, X, sizeof(X));

/* dOutput (2 x 3) */
Matrix dOutput = matrix_create(2, 3);

float dY[] = {
    1, 2, 3,
    4, 5, 6
};

memcpy(dOutput.data, dY, sizeof(dY));

Matrix dW = MATRIX_ERROR;
Matrix dB = MATRIX_ERROR;
Matrix dX = MATRIX_ERROR;

linear_backward(
    &layer,
    input,
    dOutput,
    &dW,
    &dB,
    &dX
);

printf("dW:\n");
matrix_pretty_print(dW);

printf("dB:\n");
matrix_pretty_print(dB);

printf("dX:\n");
matrix_pretty_print(dX);


  linear_free(&layer);
  matrix_free(&input);
  matrix_free(&dOutput);
  matrix_free(&dW);
  matrix_free(&dB);
  matrix_free(&dX);

  return 0;
}
