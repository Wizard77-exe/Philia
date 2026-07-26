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

float sigmoidf(float x) {
  return 1 / (1 + expf(-(x)));
}

void sigmoid(float *input, float *output, int size) {
  for (int i = 0; i < size; i++) {
    output[i] = sigmoidf(input[i]);
  }
}

float reluf(float x) {
  return x > 0.0f ? x : 0;
}

void relu(float *input, float *output, int size) {
  for (int i = 0; i < size; i++) {
    output[i] = reluf(input[i]);
  }
}

float tanh_activationf(float x) {
  return (expf(x) - expf(-(x))) / (expf(x) + expf(-(x)));
}

void tanh_activation(float *input, float *output, int size) {
  for (int i = 0; i < size; i++) {
    output[i] = tanh_activationf(input[i]);
  }
}

float geluf(float x) {
  float c = sqrtf(2/PI); 
  float cube = x * x * x;

  return 0.5f * x * (1.0f + tanh_activationf(c * (x + 0.044715 * cube)));
}

void gelu(float *input, float *output, int size) {
  for (int i = 0; i < size; i++) {
    output[i] = geluf(input[i]);
  }
}
