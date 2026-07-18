#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "structures.h"
#include "setfree.h"
#include "embedding.h"

SkipGram create_skipgram(int vocabulary_size, int embedding_dim) {
  SkipGram model = {0};

  model.vocabulary_size = vocabulary_size;
  model.embedding_dim = embedding_dim;

  model.input = create_embedding_matrix(vocabulary_size, embedding_dim);
  model.output = create_embedding_matrix(vocabulary_size, embedding_dim);

  if (!model.input.success || !model.output.success) {
    free_skipgram(&model);
    return model;
  }

  random_initialize_embedding(&model.input);
  random_initialize_embedding(&model.output);

  model.success = true;
  return model;
}

ForwardPass forward_skipgram(SkipGram *model, int center_word_id) {
  ForwardPass fp = {0};

  fp.vocabulary_size = model->vocabulary_size;
  fp.logits = calloc(fp.vocabulary_size, sizeof(float));
  fp.probabilities = calloc(fp.vocabulary_size, sizeof(float));

  if (fp.logits == NULL || fp.probabilities == NULL) {
    printf("ERROR: Allocating memory for ForwardPass.logits or ForwardPass.probabilities.\n");
    fp.vocabulary_size = 0;
    return fp;
  }

  EmbeddingVector *center_word_vector = get_embedding(&model->input, center_word_id);

  if (center_word_vector == NULL) {
    printf("ERROR: getting the EmbeddingVector of the center id %d from the SkipGram.input\n", center_word_id);
    free_forwardpass(&fp);
    return fp;
  }

  for (int i = 0; i < fp.vocabulary_size; i++) {
    fp.logits[i] = embedding_dot_product(center_word_vector, &model->output.vectors[i], EMBEDDING_DIM);
  }

  fp.success = true;
  return fp;
}

void softmax(ForwardPass *forward) {
  float sum = 0.0f;

  for (int i = 0; i < forward->vocabulary_size; i++) {
    forward->probabilities[i] = expf(forward->logits[i]);
    sum += forward->probabilities[i];
  }

  printf("Sum: %.6f\n", sum);

  for (int i = 0; i < forward->vocabulary_size; i++) {
    forward->probabilities[i] /= sum;
  }
}
