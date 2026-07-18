#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"

void canonicalize_tokens(Tokens *t, Synonym *s, int synonyms_count) {
  for (int i = 0; i < t->count; i++) {
    for (int j = 0; j < synonyms_count; j++) {
      if (strcmp(t->tokens[i].word, s[j].synonym) == 0) {
        free(t->tokens[i].word);
        t->tokens[i].word = strdup(s[j].canonical);
        break;
      }
    }
  }
}
