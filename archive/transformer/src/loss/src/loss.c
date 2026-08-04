#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "loss.h"

float forward_loss(Matrix yTrue, Matrix yPred) {
  if (!MATRIX_OK(yTrue) || !MATRIX_OK(yPred)) {
    fprintf(stderr, "ERROR: forward_loss() received empty matrix/ces.\n");
    return -1.0f;
  }
  if (!MATRIX_EQUAL_SHAPE(yTrue, yPred)) {
    fprintf(stderr, "ERROR: forward_loss() expects equal matrices, idiot.\n");
    return -1.0f;       // Loss can't possibly have a negative value, right?
  }

  size_t len = yPred.rows * yPred.cols;

  for (size_t i = 0; i < len; i++) {
    if (yPred.data[i] > 1.0f || yPred.data[i] < 0.0f) {
      fprintf(stderr, "ERROR: Predicted Probability out of bound.\n");
      return -1.0f;
    }
  }

  for (int i = 0; i < yTrue.rows; i++) {
    float sum = 0.0f;

    for (int j = 0; j < yTrue.cols; j++) {
      sum += MAT_AT(yTrue, i, j);
    }
    if (sum != 1.0f) {
      fprintf(stderr, "ERROR: Invalid terget distribution.\n");
      return -1.0f;
    }
  }

  int N = yTrue.rows;
  int C = yTrue.cols;

  float loss = 0.0f;

  for (int n = 0; n < N; n++) {
    for (int c = 0; c < C; c++) {
      // ADD isfinite() checker later to avoid INFINITY or NaN.
      loss += MAT_AT(yTrue, n, c) * logf(MAT_AT(yPred, n, c) + EPSILON);
    }
  }

  loss = -loss/N;

  assert(isfinite(loss));
  return loss;
}
