#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "structures.h"
#include "setfree.h"
#include "colors.h"

int vocabulary_index(Vocabulary v, const char *word) {
  for (int i = 0; i < v.count; i++) {
    if (strcmp(word, v.terms[i].word) == 0)
      return i;
  }

  return -1;
}

Vocabulary build_vocabulary(IndexedFact *facts, int count) {
  Vocabulary v = {0};
  v.capacity = 16;

  v.terms = calloc(v.capacity, sizeof(VocabularyTerm));
  if (v.terms == NULL) {
    free_vocabulary(&v);
    return v;
  }

  for (int i = 0; i < count; i++) {
    for (int j = 0; j < facts[i].terms.count; j++) {
      if (v.count >= v.capacity) {
        int old_capacity = v.capacity;
        v.capacity *= 2;

        VocabularyTerm *tmp = realloc(v.terms, v.capacity * sizeof(VocabularyTerm));

        if (tmp == NULL) {
          free_vocabulary(&v);
          return v;
        }

        v.terms = tmp;
        memset(v.terms + old_capacity, 0, (v.capacity - old_capacity) * sizeof(VocabularyTerm));
      }

      int index = vocabulary_index(v, facts[i].terms.terms[j].word);

      if (index >= 0) {
        v.terms[index].df += 1;
        continue;
      }

      v.terms[v.count].word = strdup(facts[i].terms.terms[j].word);
      if (v.terms[v.count].word == NULL) {
        free_vocabulary(&v);
        return v;
      }

      v.terms[v.count].df = 1;
      v.count++;
    }
  }

  return v;
}

void compute_idf(Vocabulary *v, int N) {
  for (int i = 0; i < v->count; i++) {
    v->terms[i].idf = (float)(log((float)N/ (float)v->terms[i].df));
  }
}
