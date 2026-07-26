#ifndef MATRIX_H
#define MATRIX_H

#include "structures.h"

EmbeddingMatrix matrix_transpose(const EmbeddingMatrix m);
EmbeddingMatrix matrix_add(const EmbeddingMatrix a, const EmbeddingMatrix b);
EmbeddingMatrix matrix_subtract(const EmbeddingMatrix a, const EmbeddingMatrix b);
EmbeddingMatrix matrix_scale(const EmbeddingMatrix a, float scalar);
EmbeddingMatrix matrix_copy(const EmbeddingMatrix a);

#endif
