#ifndef OPTIMIZERS_H
#define OPTIMIZERS_H

#include "structures.h"

AdamState create_adam(int vocabulary_size, int embedding_dim, float beta1, float beta2, float epsilon, float learning_rate);

void update_adam_parameter(float *weight, float gradient, float *direction, float *magnitude, AdamState *adam);

#endif
