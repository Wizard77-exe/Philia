#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "structures.h"

#define   VOCABULARY_ERROR    ((Vocabulary) {               \
                                            .terms = NULL,  \
                                            .count = 0,     \
                                            .capacity = 0,  \
                                            .success = false\
                              })
int vocabulary_index(Vocabulary v, char *word);

Vocabulary build_vocabulary(const char *dir);

void compute_idf(Vocabulary *v, int N);

#endif
