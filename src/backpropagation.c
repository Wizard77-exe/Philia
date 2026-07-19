#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "structures.h"
#include "setfree.h"

ExpectedDistribution create_expected_distribution(int vocabulary_size, int target_word_id) {
  ExpectedDistribution expected = {0};

  expected.vocabulary_size = vocabulary_size;
  expected.expected = calloc(expected.vocabulary_size, sizeof(float));

  if (expected.expected == NULL) {
    expected.vocabulary_size = 0;
    return expected;
  }

  expected.expected[target_word_id] = 1.0f;
  expected.success = true;

  return expected;
}

BackwardPass backward_skipgram(ForwardPass *forward, ExpectedDistribution *expected) {
  BackwardPass backward = {0};

  backward.vocabulary_size = forward->vocabulary_size;
  backward.gradients = calloc(backward.vocabulary_size, sizeof(float));

  if (backward.gradients == NULL) {
    backward.vocabulary_size = 0;
    return backward;
  }

  for (int i = 0; i < backward.vocabulary_size; i++) {
    backward.gradients[i] = forward->probabilities[i] - expected->expected[i];
  }

  backward.success = true;

  return backward;
}

void gradient_descent(SkipGram *model, BackwardPass *backward, int center_word_id, float learning_rate) {
  // NOTE: update the output vector of the output matrix.
  for (int i = 0; i < model->vocabulary_size; i++) {
    for (int j = 0; j < model->embedding_dim; j++) {
      model->output.vectors[i].values[j] = model->output.vectors[i].values[j] - learning_rate * backward->gradients[i] * model->input.vectors[center_word_id].values[j];
    }
  }

  for (int i = 0; i < model->embedding_dim; i++) {
    float accumulator = 0.0f;

    for (int j = 0; j < model->vocabulary_size; j++) {
      accumulator += backward->gradients[j] * model->output.vectors[j].values[i];
    }

    model->input.vectors[center_word_id].values[i] -= learning_rate * accumulator;
  }
}
