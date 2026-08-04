#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

  Matrix z = matrix_multiply(&input, &wt);

  matrix_free(&wt);

  if (!MATRIX_OK(z)) {
    fprintf(stderr, "ERROR: Matrix Multiplication failed.\n");
    matrix_free(&wt);
    return MATRIX_ERROR;
  }

  matrix_add_bias(&z, layer->bias);

  return z;
}

static bool compute_dW(const Matrix d_output, const Matrix input, Matrix *d_weights) {
  Matrix dy_t = matrix_transpose(d_output);

  if (!MATRIX_OK(dy_t)) {
    fprintf(stderr, "ERROR: Transposing d_output.\n");
    return false;
  }

  Matrix dW = matrix_multiply(&dy_t, &input);

  if (!MATRIX_OK(dW)) {
    fprintf(stderr, "ERROR: Matrix Multiplication failure.\n");
    matrix_free(&dy_t);
    return false;
  }

  *d_weights = dW;

  dW.data = NULL;
  matrix_free(&dy_t);
  matrix_free(&dW);

  return true;
}

static bool compute_db(const Matrix d_output, Matrix *d_bias) {
  Matrix db = matrix_create(1, d_output.cols);

  if (!MATRIX_OK(db)) {
    fprintf(stderr, "ERROR: Matrix creation failed.");
    return false;
  }

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

  return true;
}

static bool compute_dX(const Linear *layer, const Matrix d_output, Matrix *d_input) {
  Matrix dX = matrix_multiply(&d_output, &layer->weights);

  if (!MATRIX_OK(dX)) {
    fprintf(stderr, "ERROR: Matrix MUltiplication failed.\n");
    return false;
  }

  *d_input = dX;

  dX.data = NULL;

  matrix_free(&dX);

  return true;
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

  if (!compute_dW(d_output, input, d_weights)) {
    fprintf(stderr, "ERROR: d_weight computation failed.\n");
    return;
  }

  if (!compute_db(d_output, d_bias)) {
    fprintf(stderr, "ERROR: d_bias computation failed.\n");
    matrix_free(d_weights);
    return;
  }

  if (!compute_dX(layer, d_output, d_input)) {
    fprintf(stderr, "ERROR: d_input computation failed.\n");
    matrix_free(d_weights);
    matrix_free(d_bias);
    return;
  }
}
