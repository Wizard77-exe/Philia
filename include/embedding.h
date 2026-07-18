#ifndef EMBEDDING_H
#define EMBEDDING_H

#include "structures.h"

EmbeddingMatrix create_embedding_matrix(int vocabulary_size, int dimension);

void random_initialize_embedding(EmbeddingMatrix *matrix);

float embedding_dot_product(const EmbeddingVector *a, const EmbeddingVector *b, int dim);
float embedding_magnitude(const EmbeddingVector *v, int dim);
float embedding_cosine_similarity(const EmbeddingVector *a, const EmbeddingVector *b, int dim);

EmbeddingVector *get_embedding(EmbeddingMatrix *matrix, int word_id);

#endif
