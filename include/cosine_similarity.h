#ifndef COSINE_SIMILARITY_H
#define COSINE_SIMILARITY_H

#include "structures.h"

double compute_dot_product(DocumentTerms a, DocumentTerms b);
double compute_cosine_similarity(DocumentTerms a, DocumentTerms b);
Fact get_best_similar_fact(IndexedFact *facts, int count, Query q);

#endif
