#include <stdio.h>

#include "embedding.h"
#include "matrix.h"
#include "setfree.h"

EmbeddingMatrix matrix_transpose(const EmbeddingMatrix m) {
  EmbeddingMatrix m_t = create_embedding_matrix(m.dimension, m.vocabulary_size);

  for (int i = 0; i < m.vocabulary_size; i++) {
    for (int j = 0; j < m.dimension; j++) {
      m_t.vectors[j].values[i] = m.vectors[i].values[j];
    }
  }

  return m_t;
}

EmbeddingMatrix matrix_add(const EmbeddingMatrix a, const EmbeddingMatrix b) {
  EmbeddingMatrix c = create_embedding_matrix(a.vocabulary_size, b.dimension);

  if ((a.dimension != b.dimension) || (a.vocabulary_size != b.vocabulary_size)) {
    printf("Can't add Matrix because of difference in dimensions.\n");
    free_embedding_matrix(&c);
    return c;
  }

  for (int i = 0; i < a.vocabulary_size; i++) {
    for (int j = 0; j < a.dimension; j++) {
      c.vectors[i].values[j] = a.vectors[i].values[j] + b.vectors[i].values[j];
    }
  }

  return c;
}

EmbeddingMatrix matrix_subtract(const EmbeddingMatrix a, const EmbeddingMatrix b) {
  EmbeddingMatrix c = create_embedding_matrix(a.vocabulary_size, a.dimension);

  if ((a.dimension != b.dimension) || (a.vocabulary_size != b.vocabulary_size)) {
    printf("Can't subtract mattrices because of difference in dimension.\n");
    free_embedding_matrix(&c);
    return c;
  }

  for (int i = 0; i < a.vocabulary_size; i++) {
    for (int j = 0; j < a.dimension; j++) {
      c.vectors[i].values[j] = a.vectors[i].values[j] - b.vectors[i].values[j];
    }
  }

  return c;
}

EmbeddingMatrix matrix_scale(const EmbeddingMatrix a, float scalar) {
  EmbeddingMatrix b = create_embedding_matrix(a.vocabulary_size, a.dimension);

  for (int i = 0; i < b.vocabulary_size; i++) {
    for (int j = 0; j < b.dimension; j++) {
      b.vectors[i].values[j] = scalar * a.vectors[i].values[j];
    }
  }

  return b;
}

EmbeddingMatrix matrix_copy(const EmbeddingMatrix a) {
  EmbeddingMatrix b = create_embedding_matrix(a.vocabulary_size, a.dimension);

  for (int i = 0; i < a.vocabulary_size; i++) {
    for (int j = 0; j < a.dimension; j++) {
      b.vectors[i].values[j] = a.vectors[i].values[j];
    }
  }

  return b;
}
