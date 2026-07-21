#include <math.h>
#include "activations.h"

void softmax(float *logits, float *probabilities, int size) {
  float max = logits[0];

  for (int i = 1; i < size; i++) {
    if (logits[i] > max)
      max = logits[i];
  }

  float sum = 0.0f;

  for (int i = 0; i < size; i++) {
    probabilities[i] = expf(logits[i] - max);
    sum += probabilities[i];
  }

  for (int i = 0; i < size; i++) {
    probabilities[i] /= sum;
  }
}
