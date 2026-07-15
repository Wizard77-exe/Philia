#include <stdio.h>
#include <string.h>

#include "documents.h"
#include "vocabulary.h"
#include "indexing.h"
#include "setfree.h"
#include "files.h"
#include "query.h"

int main() {
  int knowledges_count, indexed_fact_count;

  Fact *knowledges = load_knowledges(&knowledges_count);

  IndexedFact *indexed_fact = build_indexed_fact(knowledges, knowledges_count, &indexed_fact_count);

  Vocabulary v = build_vocabulary(indexed_fact, indexed_fact_count);
  compute_idf(&v, indexed_fact_count);
  
  apply_indexedFacts_idf(indexed_fact, indexed_fact_count, v);
  compute_indexedFacts_tfidf(indexed_fact, indexed_fact_count);

  for (int i = 0; i < indexed_fact_count; i++) {
    printf("Fact #%d:\n", i);
    for (int j = 0; j < indexed_fact[i].terms.count; j++) {
      printf("%-20s%.5f               %.5f               %.5f\n", indexed_fact[i].terms.terms[j].word, indexed_fact[i].terms.terms[j].tf, indexed_fact[i].terms.terms[j].idf, indexed_fact[i].terms.terms[j].tf_idf);
    }
    printf("\n");
  }

  char input[64];
  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] = '\0';

  Query q = build_query(input, v);

  for (int i = 0; i < q.terms.count; i++) {
    printf("%-20s%.5f          %.5f            %.5f\n", q.terms.terms[i].word, q.terms.terms[i].tf, q.terms.terms[i].idf, q.terms.terms[i].tf_idf);
  }

  // free_indexed_fact(indexed_fact, indexed_fact_count);
  for (int i = 0; i < indexed_fact_count; i++) {
    free_DT(&indexed_fact[i].terms);
  }

  free_query(&q);

  free_indexed_fact(indexed_fact, indexed_fact_count);
  free_knowledges(knowledges, knowledges_count);
  free_vocabulary(&v);


  return 0;
}
