#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "js3.h"
#include "structures.h"
#include "typo.h"

void recover_typos(Tokens *t, Dictionary *d) {
  for (int i = 0; i < t->count; i++) {
    for (int j = 0; j < d->count; j++) {
      float score = JS3_similarity(t->tokens[i].word, d->words[j]);

      if (score >= 0.75) {
        free(t->tokens[i].word);
        t->tokens[i].word = strdup(d->words[j]);
      }
    }
  }
}
