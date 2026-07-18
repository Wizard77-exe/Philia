#ifndef CORPUS_H
#define CORPUS_H

#include "structures.h"

Corpus build_corpus(void);

void copy_id_to_documents(Document *docs, int count, Vocabulary v);
void copy_id_to_tokens(Tokens *tokens, int count, Vocabulary v);

#endif