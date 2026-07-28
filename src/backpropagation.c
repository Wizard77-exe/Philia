#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "structures.h"
#include "setfree.h"
#include "embedding.h"

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

// NOTE: make a copy of this function for SkipGram.
static float *compute_d_logits(CBOWForward *forward, ExpectedDistribution *expected) {
  float *d_logits = calloc(forward->vocabulary_size, sizeof(float));
  // check.
  for (int i = 0; i < forward->vocabulary_size; i++) {
    d_logits[i] = forward->probabilities[i] - expected->expected[i];
  }

  return d_logits;
}

static float *compute_d_hidden(SkipGram *model, float *d_logits) {
  float *d_hidden = calloc(model->embedding_dim, sizeof(float));
  // check;

  for (int dim = 0; dim < model->embedding_dim; dim++) {
    d_hidden[dim] = 0.0f;

    for (int word = 0; word < model->vocabulary_size; word++) {
      d_hidden[dim] += model->output.vectors[word].values[dim] * d_logits[word];
    }
  }

  return d_hidden;
}

static EmbeddingMatrix compute_output_gradients(SkipGram *model, float *d_logits, int center) {
  EmbeddingMatrix output_gradients = create_embedding_matrix(model->vocabulary_size, model->embedding_dim);
  // check;

  for (int word = 0; word < model->vocabulary_size; word++) {
    for (int dim = 0; dim < model->embedding_dim; dim++) {
      output_gradients.vectors[word].values[dim] = d_logits[word] * model->input.vectors[center].values[dim];
    }
  }

  return output_gradients;
}

// NOTE: make a copy of this function that works for SkipGram.
BackwardPass backward_skipgram(SkipGram *model, CBOWForward *forward, ExpectedDistribution *expected, int center) {
  BackwardPass backward = {0};

  backward.d_logits = compute_d_logits(forward, expected);
  // check.
  backward.d_hidden = compute_d_hidden(model, backward.d_logits);
  // check.
  backward.output_gradients = compute_output_gradients(model, backward.d_logits, center);
  // check.

  backward.success = true;

  return backward;
}

/*BackwardPass backward_skipgram(ForwardPass *forward, ExpectedDistribution *expected) {
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
}*/

/*void gradient_descent(SkipGram *model, BackwardPass *backward, int center_word_id, float learning_rate) {
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
}*/
