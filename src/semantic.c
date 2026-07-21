#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "embedding.h"
#include "tokens.h"
#include "setfree.h"
#include "corpus.h"

static void sentence_embedding(SkipGram *model, Tokens *tokens, float *output) {
  if (tokens->count == 0) {
    memset(output, 0, sizeof(float) * model->embedding_dim);
    return;
  }
  // float should be size EMBEDDING_DIM.
  float inv = 1.0f / tokens->count;

  for (int i = 0; i < model->embedding_dim; i++) {
    float average = 0.0f;

    for (int j = 0; j < tokens->count; j++) {
      if (tokens->tokens[j].id == -1)
        continue;

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

static SemanticResult *rank_documents_semantics(Corpus *corpus, float *output, int dim) {
  SemanticResult *results = malloc(sizeof(SemanticResult) * corpus->documents_count);

  if (results == NULL) {
    printf("ERROR: Allocating memory for SemanticResult in rank_documents_semantics() function.\n");
    return NULL;
  }

  for (int i = 0; i < corpus->documents_count; i++) {
    float similarity = cosine_similarity(corpus->documents[i].embeddings, output, dim);

    results[i].documents_id = i;
    results[i].similarity = similarity;

    for (int j = 0; j < i + 1; j++) {
      if (results[i].similarity > results[j].similarity) {
        SemanticResult tmp = results[i];
        results[i] = results[j];
        results[j] = tmp;
      }
    }
  }

  return results;
}

static SemanticResult *retrieve_top_k_documents_semantics(SemanticResult *ranking, int top_k) {
  SemanticResult *tops = malloc(sizeof(SemanticResult) * top_k);

  if (tops == NULL) 
    return NULL;

  for (int i = 0; i < top_k; i++){
    tops[i] = ranking[i];
  }

  return tops;
}

SemanticResult *semantic_search(SkipGram *model, Corpus *corpus, char *query, int top_k) {
  Tokens query_tokens = tokenize(query);
  if (!query_tokens.success)
    return NULL;

  copy_id_to_tokens(&query_tokens, 1, corpus->vocabulary);

  float *query_embeddings = calloc(EMBEDDING_DIM, sizeof(float));
  if (query_embeddings == NULL) {
    free_tokens(&query_tokens);
    return NULL;
  }

  sentence_embedding(model, &query_tokens, query_embeddings);
  // check query_embeddings.
  
  SemanticResult *results = rank_documents_semantics(corpus, query_embeddings, EMBEDDING_DIM);
  // check results.
  if (results == NULL) {
    free_tokens(&query_tokens);
    free(query_embeddings);
    return NULL;
  }

  SemanticResult *output = retrieve_top_k_documents_semantics(results, top_k);

  free_tokens(&query_tokens);
  free(query_embeddings);
  free(results);

  return output;
}
