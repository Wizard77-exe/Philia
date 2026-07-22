#ifndef QUERY_H
#define QUERY_H

#include "structures.h"

Query build_query(Corpus *corpus, SkipGram *model, char *query);

#endif
