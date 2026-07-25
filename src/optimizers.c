#include <stdio.h>
#include <math.h>

#include "embedding.h"
#include "optimizers.h"
#include "setfree.h"

// ###### ####  ###### #     #
// #    # #   # #    # ##   ##
// #    # #   # #    # # # # #
// ###### #   # ###### #  #  #
// #    # #   # #    # #     #
// #    # ####  #    # #     #
AdamState create_adam(int vocabulary_size, int embedding_dim, float beta1, float beta2, float epsilon, float learning_rate) {
  AdamState adam = {0};

  adam.input_direction = create_embedding_matrix(vocabulary_size, embedding_dim);
  // check.
  adam.input_magnitude = create_embedding_matrix(vocabulary_size, embedding_dim);
  // check.
  adam.output_direction = create_embedding_matrix(vocabulary_size, embedding_dim);
  // check.
  adam.output_magnitude = create_embedding_matrix(vocabulary_size, embedding_dim);
  // check.

  adam.beta1 = beta1;
  adam.beta2 = beta2;
  adam.epsilon = epsilon;
  adam.learning_rate = learning_rate;

  adam.success = true;

  return adam;
}

void update_adam_parameter(float *weight, float gradient, float *direction, float *magnitude, AdamState *adam) {
  // first moment
  *direction = (*direction) * adam->beta1 + (1 - adam->beta1) * gradient;
  // second moment
  *magnitude = (*magnitude) * adam->beta2 + (1 - adam->beta2) * (gradient * gradient);

  // BIAS CORRECTION.
  float direction_hat = (*direction) / (1.0f - powf(adam->beta1, adam->timestep));
  float magnitude_hat = (*magnitude) / (1.0f - powf(adam->beta2, adam->timestep));

  // PARAMETER UPDATE.
  *weight -= adam->learning_rate * (direction_hat / (sqrtf(magnitude_hat) + adam->epsilon));
}
