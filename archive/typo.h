#ifndef TYPO_H
#define TYPO_H

#include "structures.h"

typedef struct {
  int count;
  char **words;
} Dictionary;

void recover_typos(Tokens *t, Dictionary *d);

#endif
