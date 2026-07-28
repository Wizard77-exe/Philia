#include <stdlib.h>
#include <stdio.h>

#include "structures.h"

// NOTE: for freeing array of string char s[][];

void free_string_array(char **s, int count) {
  if (s == NULL) 
    return;

  for (int i = 0; i < count; i++) {
    free(s[i]);
  }

  free(s);
  s = NULL;
}

// NOTE: for freeing Tokens
void free_tokens(Tokens *t) {
  for (int i = 0; i < t->count; i++) {
    free(t->tokens[i].word);
    t->tokens[i].id = -1;
  }

  free(t->tokens);
  t->tokens = NULL;
  t->count = 0;
  t->capacity = 0;
  t->success = false;
}

// NOTE: for freeing Document
void free_document(Document *d) {
  for (int i = 0; i < d->count; i++) {
    free(d->terms[i].word);
  }

  free(d->terms);
  free(d->embeddings);
  free(d->tf_idf_values);
  d->terms = NULL;
  d->count = 0;
  d->capacity = 0;
  d->embedding_magnitude = 0.0f;
  d->tf_idf_magnitude = 0.0f;
  d->success = false;
}

// NOTE: for freeing Vocabulary
void free_vocabulary(Vocabulary *v) {
  for (int i = 0; i < v->count; i++) {
    free(v->terms[i].word);
  }
  free(v->terms);
  v->terms = NULL;
  v->count = 0;
  v->capacity = 0;
}

// NOTE: for freeing corpus
void free_corpus(Corpus *c) {
  free_vocabulary(&c->vocabulary);
  for (int i = 0; i < c->documents_count; i++) {
    free_document(&c->documents[i]);
    free_tokens(&c->tokens[i]);
  }
  free(c->documents);
  free(c->tokens);

  c->documents = NULL;
  c->tokens = NULL;
  
  c->documents_count = 0;
  c->documents_capacity = 0;
}

// NOTE: for freeing TrainingSet
void free_training_set(TrainingSet *ts) {
  free(ts->pairs);
  ts->pairs = NULL;
  ts->count = 0;
  ts->capacity = 0;
  ts->success = false;
}

// NOTE: for freeing EmbeddingMatrix
void free_embedding_matrix(EmbeddingMatrix *matrix) {
  for (int i = 0; i < matrix->vocabulary_size; i++) {
    free(matrix->vectors[i].values);
  }
  free(matrix->vectors);
  matrix->vectors = NULL;
  matrix->vocabulary_size = 0;
  matrix->dimension = 0;
  matrix->success = false;
}

// NOTE: for freeing SkipGram
void free_skipgram(SkipGram *model) {
  free_embedding_matrix(&model->input);
  free_embedding_matrix(&model->output);

  model->vocabulary_size = 0;
  model->embedding_dim = 0;

  model->success = false;
}

// NOTE: for freeing ForwardPass
void free_forwardpass(ForwardPass *fp) {
  free(fp->logits);
  free(fp->probabilities);
  fp->logits = NULL;
  fp->probabilities = NULL;
  fp->vocabulary_size = 0;
  fp->success = false;
}

// NOTE: for freeing ExpectedDistribution
void free_expected_distribution(ExpectedDistribution *expected) {
  free(expected->expected);

  expected->expected = NULL;
  expected->vocabulary_size = 0;
  expected->success = false;
}

// NOTE: for freeing BackwardPass
void free_backwardpass(BackwardPass *backward) {
  free(backward->d_logits);
  free(backward->d_hidden);
  free_embedding_matrix(&backward->output_gradients);
  backward->success = false;
}

// NOTE: for freeing Query
void free_query(Query *query) {
  free_tokens(&query->tokens);
  free_document(&query->document);
  free(query->embeddings);
  query->embeddings = NULL;
}

void free_cbow_training_set(CBOWTrainingSet *cbow) {
  for (int i = 0; i < cbow->samples_count; i++) {
    free(cbow->samples[i].context);
    cbow->samples[i].center = 0;
    cbow->samples[i].context_count = 0;
    cbow->samples[i].success = false;
  }
  free(cbow->samples);
  cbow->samples = NULL;
  cbow->capacity = 0;
  cbow->success = false;
}

void free_forward_cbow(CBOWForward *cfp) {
  free(cfp->probabilities);
  free(cfp->logits);
  cfp->probabilities = NULL;
  cfp->logits = NULL;
  cfp->vocabulary_size = 0;
  cfp->success = false;
}
