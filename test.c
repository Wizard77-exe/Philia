#include <stdio.h>
#include <math.h>

#include "matrix.h"
#include "embedding.h"
#include "setfree.h"
#include "structures.h"

int main() {
  EmbeddingMatrix a = create_embedding_matrix(2, 3);
  random_initialize_embedding(&a);
  EmbeddingMatrix b = create_embedding_matrix(3, 5);
  random_initialize_embedding(&b);

  EmbeddingMatrix c = matrix_product(a, b);

  printf("Matrix A: \n");
  for (int i = 0; i < a.vocabulary_size; i++) {
    printf("[");
    for (int j = 0; j < a.dimension; j++) {
      printf("%f ", a.vectors[i].values[j]);
    }
    printf("]\n");
  }

  printf("Matrix B: \n");
  for (int i = 0; i < b.vocabulary_size; i++) {
    printf("[");
    for (int j = 0; j < b.dimension; j++) {
      printf("%f ", b.vectors[i].values[j]);
    }
    printf("]\n");
  }

  printf("Matrix Product: \n");
  for (int i = 0; i < c.vocabulary_size; i++) {
    printf("[");
    for (int j = 0; j < c.dimension; j++) {
      printf("%f ", c.vectors[i].values[j]);
    }
    printf("]\n");
  }

  free_embedding_matrix(&a);
  free_embedding_matrix(&b);
  free_embedding_matrix(&c);
  return 0;
}
