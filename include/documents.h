#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "structures.h"

DocumentTerms compute_tf(Tokens tokens);

void apply_idf(DocumentTerms *dt, Vocabulary v);
void compute_tfidf(DocumentTerms *dt);

#endif
