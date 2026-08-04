#include <stdio.h>
#include <stdlib.h>

#include "dense.h"

Dense *dense_create(int in_features, int out_features, const Activation *activation) {
  if (activation == NULL) {
    fprintf(stderr, "ERROR: dense_create() received a NULL activation function pointer.\n");
    return NULL;
  }

  Dense *layer = malloc(sizeof(Dense));

  if (layer == NULL) {
    fprintf(stderr, "ERROR: Memory Allocation Failure in dense_create().\n");
    return NULL;
  }

  layer->linear = linear_create(in_features, out_features);

  if (!LINEAR_OK(layer->linear)) {
    fprintf(stderr, "ERROR: Linear creation failure in dense_create().\n");
    free(layer);
    return NULL;
  }

  layer->activation = activation;
  layer->Z = MATRIX_ERROR;

  return layer;
}

void dense_free(Dense *layer) {
  if (layer == NULL)
    return;

  linear_free(&layer->linear);
  matrix_free(&layer->Z);
  free(layer);
}

Matrix dense_forward(Dense *layer, const Matrix input) {
  if (layer == NULL) {
    fprintf(stderr, "ERROR: dense_forward() received a NULL pointer to the layer.\n");
    return MATRIX_ERROR;
  }

  if (!MATRIX_OK(input)) {
    fprintf(stderr, "ERROR: dense_forward() received an empty input.\n");
    return MATRIX_ERROR;
  }

  Matrix Z = linear_forward(&layer->linear, input);

  if (!MATRIX_OK(Z)) {
    fprintf(stderr, "ERROR: linear_forward() failed inside the dense_forward() function.\n");
    return MATRIX_ERROR;
  }

  Matrix Y = matrix_copy(&Z);

  if (!MATRIX_OK(Y)) {
    fprintf(stderr, "ERROR: matrix_copy() failure inside the dense_forward() function.\n");
    matrix_free(&Z);
    return MATRIX_ERROR;
  }

  
  matrix_move(&layer->Z, &Z);

  linear->activation->forward(&Y);

  return Y;
}
