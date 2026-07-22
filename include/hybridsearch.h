#ifndef HYBRIDSEARCH_H
#define HYBRIDSEARCH_H

#include "structures.h"

HybridResult *hybrid_search(Query *query, Corpus *corpus, SkipGram *model, float alpha);

#endif
