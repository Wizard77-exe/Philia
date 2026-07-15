#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "setfree.h"
#include "colors.h"
#include "tokens.h"
#include "documents.h"
#include "normalize.h"

IndexedFact *build_indexed_fact(Fact *knowledges, int knowledges_count, int *indexed_facts_count) {
  IndexedFact *index = calloc(knowledges_count, sizeof(IndexedFact));

  if (index == NULL) {
    printf("%sPHILIA:%s DADDD!! I can't allocate memory for IndexedFact inside the build_indexed_fact() function!\n\n", BRIGHT_GREEN, RESET);
    *indexed_facts_count = 0;
    return NULL;
  }

  char buffer[512];

  for (int i  = 0; i < knowledges_count; i++) {
    index[i].fact = &knowledges[i];

    snprintf(buffer, sizeof(buffer), "%s %s %s", knowledges[i].subject, knowledges[i].attribute, knowledges[i].value);
    normalize(buffer);

    index[i].tokens = tokenize(buffer);
    index[i].terms = compute_tf(index[i].tokens);

    if (!index[i].tokens.success || !index[i].terms.success) {
      free_indexed_fact(index, i + 1);
      *indexed_facts_count = 0;
      return NULL;
    }
  }

  *indexed_facts_count = knowledges_count;
  return index;
}

void apply_indexedFacts_idf(IndexedFact *facts, int count, Vocabulary v) {
  for (int i = 0; i < count; i++) {
    apply_idf(&facts[i].terms, v);
  }
}

void compute_indexedFacts_tfidf(IndexedFact *facts, int count) {
  for (int i = 0; i < count; i++) {
    compute_tfidf(&facts[i].terms);
  }
}
