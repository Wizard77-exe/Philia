#ifndef SKIPGRAM_H
#define SKIPGRAM_H

#include "structures.h"

SkipGram create_skipgram(int vocabulary_size, int embedding_dim);

ForwardPass forward_skipgram(SkipGram *model, int center_word_id);

#endif
