#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "structures.h"
#include "setfree.h"

double compute_dot_product(DocumentTerms a, DocumentTerms b) {
  double dot_product = 0;

  for (int i = 0; i < a.count; i++) {
    for (int j = 0; j < b.count; j++) {
      if (strcmp(a.terms[i].word, b.terms[j].word) == 0) {
        dot_product += a.terms[i].tf_idf * b.terms[j].tf_idf;
        break;
      }
    }
  }

  return dot_product;
}

double compute_cosine_similarity(DocumentTerms a, DocumentTerms b) {
  double dot_product = compute_dot_product(a, b);
  double cos_sim = dot_product / (a.magnitude * b.magnitude);

  return cos_sim;
}

Fact get_best_similar_fact(IndexedFact *facts, int count, Query q) {
  double best_score = 0.0;
  Fact best_fact;

  for (int i = 0; i < count; i++) {
    double dot_product = compute_dot_product(facts[i].terms, q.terms);
    double cos_s = compute_cosine_similarity(facts[i].terms, q.terms);

    if (cos_s > best_score) {
      best_score = cos_s;
      best_fact = *facts[i].fact;
    }
  }

  return best_fact;
}
