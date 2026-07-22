#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "structures.h"
#include "setfree.h"

EmbeddingMatrix create_embedding_matrix(int vocabulary_size, int dimension) {
  EmbeddingMatrix matrix = {0};

  matrix.vocabulary_size = vocabulary_size;
  matrix.dimension = dimension;

  matrix.vectors = calloc(vocabulary_size, sizeof(EmbeddingVector));
  if (matrix.vectors == NULL) {
    printf("ERROR: Allocating memory for EmbeddingMatrix.vectors.\n\n");
    matrix.vocabulary_size = 0;
    matrix.dimension = 0;
    return matrix;
  }

  for (int i = 0; i < vocabulary_size; i++) {
    matrix.vectors[i].values = calloc(matrix.dimension, sizeof(float));

    if (matrix.vectors[i].values == NULL) {
      printf("ERROR: Allocating memory for EmbeddingMatrix.vectors[%d].\n\n", i);
      matrix.vocabulary_size = i + 1;
      free_embedding_matrix(&matrix);
      return matrix;
    }
  }

  matrix.success = true;
  return matrix;
}

void random_initialize_embedding(EmbeddingMatrix *matrix) {
  for (int i = 0; i < matrix->vocabulary_size; i++) {
    for (int j = 0; j < matrix->dimension; j++) {
      matrix->vectors[i].values[j] = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 0.01f;
    }
  }
}

float embedding_dot_product(const EmbeddingVector *a, const EmbeddingVector *b, int dim) {
  float dot_product = 0.0f;
  for (int i = 0; i < dim; i++) {
    dot_product += a->values[i] * b->values[i];
  }

  return dot_product;
}

float embedding_magnitude(const EmbeddingVector *v, int dim) {
  float sum_squared_components = 0.0f;
  for (int i = 0; i < dim; i++) {
    sum_squared_components += v->values[i] * v->values[i];
  }

  return sqrtf(sum_squared_components);
}

float embedding_cosine_similarity(const EmbeddingVector *a, const EmbeddingVector *b, int dim) {
  float dot_product = embedding_dot_product(a, b, dim);
  float magnitude_a = embedding_magnitude(a, dim);
  float magnitude_b = embedding_magnitude(b, dim);

  if (magnitude_a == 0.0f || magnitude_b == 0.0f) {
    return 0.0f;
  }

  return dot_product / (magnitude_a * magnitude_b);
}

EmbeddingVector *get_embedding(EmbeddingMatrix *matrix, int word_id) {
  if (matrix == NULL) 
    return NULL;

  if (word_id < 0 || word_id >= matrix->vocabulary_size)
    return NULL;

  return &matrix->vectors[word_id];
}

float dot_product(float *a, float *b, int dim) {
  float dot_product = 0.0f;

  for (int i = 0; i < dim; i++) {
    dot_product += a[i] * b[i];
  }

  return dot_product;
}

float get_magnitude(float *vector, int dim) {
  float magnitude = 0.0f;

  for (int i = 0; i < dim; i++) {
    magnitude += vector[i] * vector[i];
  }
  return sqrtf(magnitude) ;
}

float cosine_similarity(float *a, float *b, int dim) {
  return dot_product(a, b, dim);
}

static void normalize_vector(float *vector, int dim) {
  float magnitude = get_magnitude(vector, dim);

  if (magnitude < 1e-8f)
    return;

  for (int i = 0; i < dim; i++) {
    vector[i] /= magnitude;
  }
}

void normalize_embedding_matrix(SkipGram *model) {
  for (int i = 0; i < model->vocabulary_size; i++) {
    normalize_vector(model->input.vectors[i].values, model->embedding_dim);
    normalize_vector(model->output.vectors[i].values, model->embedding_dim);
  }
}

void normalize_query_embeddings(float *embeddings, int dim) {
  normalize_vector(embeddings, dim);
}

void normalize_corpus_embeddings(Corpus *corpus, int dim) {
  for (int i = 0; i < corpus->documents_count; i++) {
    normalize_vector(corpus->documents[i].embeddings, dim);
  }
}
