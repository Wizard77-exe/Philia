#include <stdio.h>
#include "nlp.h"
#include "tf_idf.h"

int main() {
  int knowledges_count, indexed_fact_count;

  Fact *knowledges = load_knowledges(&knowledges_count);

  IndexedFact *indexed_fact = build_indexed_fact(knowledges, knowledges_count, &indexed_fact_count);
  compute_indexedFact_tf(indexed_fact, indexed_fact_count);
  Vocabulary v = build_vocabulary(indexed_fact, indexed_fact_count);
  compute_idf(&v, indexed_fact_count);
  apply_idf(indexed_fact, indexed_fact_count, v);
  compute_tfidf(indexed_fact, indexed_fact_count);

  for (int i = 0; i < indexed_fact_count; i++) {
    printf("Fact #%d:\n", i);
    for (int j = 0; j < indexed_fact[i].terms.count; j++) {
      printf("%-20s%.5f               %.5f               %.5f\n", indexed_fact[i].terms.terms[j].word, indexed_fact[i].terms.terms[j].tf, indexed_fact[i].terms.terms[j].idf, indexed_fact[i].terms.terms[j].tf_idf);
    }
    printf("\n");
  }

  // free_indexed_fact(indexed_fact, indexed_fact_count);
  for (int i = 0; i < indexed_fact_count; i++) {
    free_DT(&indexed_fact[i].terms);
  }

  free_indexed_fact(indexed_fact, indexed_fact_count);
  free_knowledges(knowledges, knowledges_count);
  free_vocabulary(&v);


  return 0;
}
