#ifndef RELU_H
#define RELU_H

#include "matrix.h"

void relu_forward(Matrix *m);
void relu_backward(Matrix *d_input, const Matrix input, const Matrix d_output);

#endif
