#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "setfree.h"
#include "vocabulary.h"

static int is_in_documentTerms(DocumentTerms dt, const char *word) {
  for (int i = 0; i < dt.count; i++) {
    if (strcmp(dt.terms[i].word, word) == 0)
      return i;
  }

  return -1;
}

DocumentTerms compute_tf(Tokens tokens) {
  DocumentTerms dt = {0};

  dt.terms = calloc(tokens.count, sizeof(Term));

  if (dt.terms == NULL) {
    free_DT(&dt);
    return dt;
  }

  for (int i = 0; i < tokens.count; i++) {
    int index = is_in_documentTerms(dt, tokens.tokens[i].word);
    if (index >= 0) {
      (dt.terms[index].frequency) += 1;
      dt.terms[index].tf = (float)((float)dt.terms[index].frequency / (float)tokens.count);
      continue;
    }

    dt.terms[dt.count].word = strdup(tokens.tokens[i].word);

    if (dt.terms[dt.count].word == NULL) {
      free_DT(&dt);
      return dt;
    }

    dt.terms[dt.count].frequency = 1;
    dt.terms[dt.count].tf = (float)((float)dt.terms[dt.count].frequency / (float)tokens.count);
    dt.count++;
  }

  dt.success = true;
  return dt;
}

void apply_idf(DocumentTerms *dt, Vocabulary v) {
  for (int i = 0; i < dt->count; i++) {
    for (int j = 0; j < v.count; j++) {
      int idx = vocabulary_index(v, dt->terms[i].word);

      if (idx == -1) {
        // tell something
        dt->terms[i].idf = 0.0f;
        continue;
      }

      dt->terms[i].idf = v.terms[idx].idf;
    }
  }
}

void compute_tfidf(DocumentTerms *dt) {
  for (int i = 0; i < dt->count; i++) {
    dt->terms[i].tf_idf = dt->terms[i].tf * dt->terms[i].idf;
  }
}
