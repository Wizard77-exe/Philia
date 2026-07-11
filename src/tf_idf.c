#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tf_idf.h"
#include "nlp.h"

static int is_in_documentTerms(DocumentTerms dt, char *word) {
  for (int i = 0; i < dt.count; i++) {
    if (strcmp(dt.terms[i].word, word) == 0)
      return i;
  }

  return -1;
}

static void free_terms(DocumentTerms *dt) {
  for (int i = 0; i < dt->count; i++) {
    free(dt->terms[i].word);
    dt->terms[i].frequency = 0;
    dt->terms[i].tf = 0.0f;
  }
  free(dt->terms);
  dt->terms = NULL;
}

void free_DT(DocumentTerms *dt) {
  free_terms(dt);

  dt->count = 0;
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

  return dt;
}
