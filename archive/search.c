#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "structures.h"
#include "cosine_similarity.h"

SearchResult search_similar_fact(IndexedFact *facts, int count, Query q) {
  double best_score = 0.0;
  int index = -1;

  for (int i = 0; i < count; i++) {
    double cos_s = compute_cosine_similarity(facts[i].terms, q.terms);

    if (cos_s > best_score) {
      best_score = cos_s;
      index = i;
    }
  }

  SearchResult result = {
    .fact_index = index,
    .score = (float)best_score
  };

  return result;
}
