#include <stdio.h>
#include <stdlib.h>

#include "linear.h"

void linear_free(Linear *layer) {
  if (layer == NULL)
    return;

  matrix_free(&layer->weights);
  matrix_free(&layer->bias);

  layer->weights = MATRIX_ERROR;
  layer->bias = MATRIX_ERROR;

  layer->in_features = 0;
  layer->out_features = 0;
}

Linear linear_create(int in_features, int out_features) {
  if (in_features <= 0 || out_features <= 0) {
    fprintf(stderr, "ERROR: linear_create() requires dimensions greater than zero.\n");
    return LINEAR_ERROR;
  }
  Linear layer = {0};

  layer.in_features = in_features;
  layer.out_features = out_features;

  layer.weights = matrix_create(layer.out_features, layer.in_features);
  if (!MATRIX_OK(layer.weights)) {
    linear_free(&layer);
    return LINEAR_ERROR;
  }

  matrix_fill_he_uniform(&layer.weights);

  layer.bias = matrix_create(1, layer.out_features);
  if (!MATRIX_OK(layer.bias)) {
    linear_free(&layer);
    return LINEAR_ERROR;
  }

  matrix_fill(&layer.bias, 0);
  
  return layer;
}

Matrix linear_forward(const Linear *layer, const Matrix input) {
  if (input.cols != layer->in_features) {
    fprintf(stderr, "ERROR: Input features mismatch.\n");
    return MATRIX_ERROR;
  }

  Matrix wt = matrix_transpose(layer->weights);
  if (!MATRIX_OK(wt)) {
    fprintf(stderr, "ERROR: Transposing Layer Weight failed.\n");
    return MATRIX_ERROR;
  }

  Matrix y = matrix_multiply(&input, &wt);

  matrix_free(&wt);

  if (!MATRIX_OK(y)) {
    fprintf(stderr, "ERROR: Matrix Multiplication failed.\n");
    matrix_free(&wt);
    return MATRIX_ERROR;
  }

  matrix_add_bias(&y, layer->bias);

  return y;
}

void linear_backward(const Linear *layer
                   , const Matrix input
                   , const Matrix d_output
                   , Matrix *d_weights
                   , Matrix *d_bias
                   , Matrix *d_input
) {
  if (layer == NULL || d_weights == NULL || d_bias == NULL || d_input == NULL) {
    fprintf(stderr, "ERROR: linear_backward() received NULL parameter.\n");
    return;
  }

  if (input.rows != d_output.rows || d_output.cols != layer->out_features || input.cols != layer->in_features) {
    fprintf(stderr, "ERROR: linear_backward() failed because of dimension mismatch.\n");
    return;
  }

  // computing dW
  Matrix dy_t = matrix_transpose(d_output);
  if (!MATRIX_OK(dy_t)) {
    fprintf(stderr, "ERROR: Transposing d_output in linear_backward() function.\n");
    return;
  }

  Matrix dW = matrix_multiply(&dy_t, &input);
  if (!MATRIX_OK(dW)) {
    fprintf(stderr, "ERROR: Matrix Multiplication failed.\n");
    matrix_free(&dy_t);
    return;
  }

  *d_weights = dW;
  
  dW.data = NULL;

  matrix_free(&dW);
  matrix_free(&dy_t);

  // computing db
  Matrix db = matrix_create(1, layer->out_features);
  matrix_fill(&db, 0);
  // check.
  for (int col = 0; col < d_output.cols; col++) {
    for (int row = 0; row < d_output.rows; row++) {
      MAT_AT(db, 0, col) += MAT_AT(d_output, row, col);
    }
  }

  *d_bias = db;

  db.data = NULL;
  matrix_free(&db);

  // computing dX
  Matrix dX = matrix_multiply(&d_output, &layer->weights);
  if (!MATRIX_OK(dX)) {
    fprintf(stderr, "ERROR: Matrix MUltiplication failed.\n");
    return;
  }

  *d_input = dX;

  dX.data = NULL;
  matrix_free(&dX);
}
