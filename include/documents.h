#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "structures.h"

Document compute_tf(Tokens tokens);

void apply_idf(Document *docs, int count, Vocabulary v);
void compute_tfidf(Document *docs, int count);

void document_magnitude(Document *docs, int count);
double document_dot_product(Document *a, Document *b);
double document_cosine_similarity(Document a, Document b);

#endif
