#ifndef CORPUS_H
#define CORPUS_H

#include "structures.h"
void compute_corpus_tfidf(Document *docs, int count, Vocabulary *vocabulary);
void corpus_document_tf_idf_magnitude(Document *docs, int count, int dim);

void copy_id_to_documents(Document *docs, int count, Vocabulary v);
void copy_id_to_tokens(Tokens *tokens, int count, Vocabulary v);

Corpus build_corpus(void);

#endif
