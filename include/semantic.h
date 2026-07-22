#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "structures.h"

void sentence_embedding(SkipGram *model, Tokens *tokens, float *output);
void build_document_embeddings(SkipGram *model, Corpus *corpus);

//SemanticResult *rank_documents_semantics(Corpus *corpus, float *output, int dim);

//int *retrieve_top_k_documents_semantics(SemanticResult *ranking, int top_k);

SemanticResult *semantic_search(SkipGram *model, Corpus *corpus, char *query, int top_k);

#endif
