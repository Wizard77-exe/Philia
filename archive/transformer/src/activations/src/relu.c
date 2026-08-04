#include <stdio.h>
#include <stdlib.h>

#include "relu.h"
#include "matrix.h"
#include "activation.h"

void relu_forward(Matrix *m) {
  if (m == NULL || !MATRIX_OK(*m)) {
    fprintf(stderr, "ERROR: relu_forward() received invalid matrix.\n");
    return;
  }

  size_t len = m->rows * m->cols;
  for (size_t i = 0; i < len; i++) {
    if (m->data[i] <= 0.0f)
      m->data[i] = 0.0f;
  }
}

void relu_backward(Matrix *d_input, const Matrix input, const Matrix d_output) {
  if (d_input == NULL || !MATRIX_OK(*d_input) || !MATRIX_OK(input) || !MATRIX_OK(d_output)) {
    fprintf(stderr, "ERROR: relu_backward() received invalid matrix.\n");
    return;
  }

  if (!MATRIX_EQUAL_SHAPE(*d_input, input) || !MATRIX_EQUAL_SHAPE(*d_input, d_output)) {
    fprintf(stderr, "ERROR: relu_backward() received matrices with unequal shapes.\n");
    return;
  }

  size_t len = d_input->rows * d_input->cols;

  for (size_t i = 0; i < len; i++) {
    if (input.data[i] <= 0)
      d_input->data[i] = 0.0f;
    else 
      d_input->data[i] = d_output.data[i];
  }
}

const Activation RELU = {
  .name = "ReLU",
  .forward = relu_forward,
  .backward = relu_backward
};
