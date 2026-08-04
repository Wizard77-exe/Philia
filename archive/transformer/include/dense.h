#ifndef DENSE_H
#define DENSE_H

#include "linear.h"
#include "matrix.h"
#include "activation.h"

typedef struct {
    Linear linear;                 // owns weights and bias
    const Activation *activation;  // borrowed, points to RELU/SIGMOID/etc
    
    Matrix Z;
} Dense;

Dense *dense_create(int in_features,
                   int out_features,
                   const Activation *activation);

void dense_free(Dense *layer);

Matrix dense_forward(Dense *layer, const Matrix input);

void dense_backward(Dense *layer,
                    const Matrix input,
                    const Matrix d_output,
                    Matrix *d_weights,
                    Matrix *d_bias,
                    Matrix *d_input);

#endif
