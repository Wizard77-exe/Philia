#ifndef CBOW_H
#define CBOW_H

#include "structures.h"

CBOWTrainingSet build_cbow_samples(Corpus corpus, int window_size);

EmbeddingVector cbow_hidden_vector(EmbeddingMatrix *input, CBOWSample *sample);

CBOWForward forward_cbow(SkipGram *model, const EmbeddingVector *hidden);

#endif
