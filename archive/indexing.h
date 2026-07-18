#ifndef INDEXING_H
#define INDEXING_H

#include "structures.h"

IndexedFact *build_indexed_fact(Fact *knowledges, int knowledges_count, int *indexed_facts_count);

void apply_indexedFacts_idf(IndexedFact *facts, int count, Vocabulary v);
void compute_indexedFacts_tfidf(IndexedFact *facts, int count);

void compute_indexedFacts_magnitude(IndexedFact *facts, int count);

#endif
