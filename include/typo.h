#include "nlp.h"

#ifndef TYPO_H
#define TYPO_H

typedef struct {
  int count;
  char **words;
} Dictionary;

void recover_typos(Tokens *t, Dictionary *d);
char **load_dictionary(int *dictionary_count);

#endif
