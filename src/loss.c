#include <stdio.h>
#include <math.h>

#include "structures.h"

float compute_cross_entropy_loss(float *probabilities, int target_word_id) {
  float epsilon = 1e-7f;
  float probability = probabilities[target_word_id];

  return -logf(probability + epsilon);
}
