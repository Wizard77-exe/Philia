#ifndef EMBEDDING_H
#define EMBEDDING_H

#include "structures.h"

EmbeddingMatrix create_embedding_matrix(int vocabulary_size, int dimension);

void random_initialize_embedding(EmbeddingMatrix *matrix);

EmbeddingVector *get_embedding(EmbeddingMatrix *matrix, int word_id);

float dot_product(float *a, float *b, int dim);
float get_magnitude(float *vector, int dim);
float cosine_similarity(float *a, float *b, int dim);

void normalize_vector(float *vector, int dim);
void normalize_embedding_matrix(SkipGram *model);
void normalize_query_embeddings(Query *query, int embedding_dim, int vocabulary_size);
void normalize_corpus_embeddings(Corpus *corpus, int dim);

#endif
