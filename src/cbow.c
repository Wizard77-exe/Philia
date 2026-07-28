#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cbow.h"
#include "setfree.h"
#include "embedding.h"

CBOWTrainingSet build_cbow_samples(Corpus corpus, int window_size) {
  CBOWTrainingSet cbow = {0};

  cbow.capacity = 16;
  cbow.samples = calloc(cbow.capacity, sizeof(CBOWSample));

  if (cbow.samples == NULL) {
    printf("ERROR: Allocation for cbow.samples inside the build_cbow_training_set() function.\n");
    free_cbow_training_set(&cbow);
    return cbow;
  }

  for (int doc = 0; doc < corpus.documents_count; doc++) {
    int left_edge = 0;
    int right_edge = corpus.tokens[doc].count - 1;

    for (int center = 0; center < corpus.tokens[doc].count; center++) {
      if (cbow.samples_count >= cbow.capacity) {
        int old_capacity = cbow.capacity;
        cbow.capacity *= 2;
        CBOWSample *tmp = realloc(cbow.samples, sizeof(CBOWSample) * cbow.capacity);

        // check if tmp == NULL.
        if (tmp == NULL) {
          printf("ERROR: Reallocation on cbow.samples inside the build_cbow_training_set() function.\n");
          free_cbow_training_set(&cbow);
          return cbow;
        }

        cbow.samples = tmp;

        memset(cbow.samples + old_capacity, 0, (cbow.capacity - old_capacity) * sizeof(CBOWSample));
      }

      cbow.samples[cbow.samples_count].center = corpus.tokens[doc].tokens[center].id;
      cbow.samples[cbow.samples_count].context = calloc(window_size * 2, sizeof(int));

      // NULL checking.
      if (cbow.samples[cbow.samples_count].context == NULL) {
        printf("ERROR: Allocation of Memory on cbow.samples[%d].context inside the build_cbow_training_set() function.\n", cbow.samples_count);
        free_cbow_training_set(&cbow);
        return cbow;
      }

      for (int offset = center - window_size; offset <= center + window_size; offset++) {
        if (offset < left_edge || offset > right_edge || offset == center) continue;

        cbow.samples[cbow.samples_count].context[cbow.samples[cbow.samples_count].context_count] = corpus.tokens[doc].tokens[offset].id;
        cbow.samples[cbow.samples_count].context_count++;
      }

      cbow.samples_count++;
      cbow.samples[doc].success = true;
    }
  }
  cbow.success = true;
  return cbow;
}

//returns the hidden vector.
EmbeddingVector cbow_hidden_vector(EmbeddingMatrix *input, CBOWSample *sample) {
  EmbeddingVector hidden = {0};

  hidden.values = calloc(input->dimension, sizeof(float));

  if (hidden.values == NULL) {
    return hidden;
  }

  for (int i = 0; i < sample->context_count; i++) {
    for (int j = 0; j < input->dimension; j++) {
      hidden.values[j] += input->vectors[sample->context[i]].values[j];
    }
  }

  for (int i = 0; i < input->dimension; i++) {
    hidden.values[i] /= sample->context_count;
  }
  return hidden;
}

// forward_cbow, returns CBOWForward with *logits and a ready to assign *probabilities.
CBOWForward forward_cbow(SkipGram *model, const EmbeddingVector *hidden) {
  CBOWForward cfp = {0};

  cfp.vocabulary_size = model->vocabulary_size;
  cfp.logits = calloc(cfp.vocabulary_size, sizeof(float));
  cfp.probabilities = calloc(cfp.vocabulary_size, sizeof(float));

  if (cfp.logits == NULL || cfp.probabilities == NULL) {
    printf("ERROR: Allocating memory for ForwardPass.logits or ForwardPass.probabilities.\n");
    free_forward_cbow(&cfp);
    return cfp;
  }

  for (int i = 0; i < cfp.vocabulary_size; i++) {
    cfp.logits[i] = dot_product(hidden->values, model->output.vectors[i].values, model->embedding_dim);
  }

  cfp.success = true;
  return cfp;
}
