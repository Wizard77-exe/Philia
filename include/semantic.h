#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "structures.h"

void sentence_embedding(SkipGram *model, Tokens *tokens, float *output);
void build_document_embeddings(SkipGram *model, Corpus *corpous);

#endif
