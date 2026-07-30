#ifndef LINEAR_H
#define LINEAR_H

#include "matrix.h"

#define LINEAR_ERROR ((Linear){     \
    .weights = MATRIX_ERROR,        \
    .bias = MATRIX_ERROR,           \
    .in_features = 0,               \
    .out_features = 0               \
    })

typedef struct {
  Matrix weights;
  Matrix bias;

  int in_features;
  int out_features;
} Linear;

Linear linear_create(int in_features, int out_features);

void linear_free(Linear *layer);

Matrix linear_forward(const Linear *layer, const Matrix input);

void linear_backward(const Linear *layer
                   , const Matrix input
                   , const Matrix d_output
                   , Matrix *d_weights
                   , Matrix *d_bias
                   , Matrix *d_input
);

#endif
