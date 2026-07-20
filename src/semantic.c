#include <stdio.h>
#include <string.h>
#include "structures.h"

void sentence_embedding(SkipGram *model, Tokens *tokens, float *output) {
  if (tokens->count == 0) {
    memset(output, 0, sizeof(float) * model->embedding_dim);
    return;
  }
  // float should be size EMBEDDING_DIM.
  float inv = 1.0f / tokens->count;

  for (int i = 0; i < model->embedding_dim; i++) {
    float average = 0.0f;

    for (int j = 0; j < tokens->count; j++) {
      average += model->input.vectors[tokens->tokens[j].id].values[i];
    }

    output[i] = average * inv;
  }
}

void build_document_embeddings(SkipGram *model, Corpus *corpus) {
  for (int i = 0; i < corpus->documents_count; i++) {
    sentence_embedding(model, &corpus->tokens[i], corpus->documents[i].embeddings);
  }
}
