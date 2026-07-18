#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "structures.h"

int vocabulary_index(Vocabulary v, const char *word);

Vocabulary build_vocabulary(Document *documents, int count);

void compute_idf(Vocabulary *v, int N);

#endif
