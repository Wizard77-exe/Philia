#ifndef SEARCH_H
#define SEARCH_H

#include "structures.h"

SearchResult search_similar_fact(IndexedFact *facts, int count, Query q);

#endif
