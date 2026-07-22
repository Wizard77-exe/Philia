#include <stdio.h>
#include <stdlib.h>

#include "hybridsearch.h"
#include "colors.h"
#include "embedding.h"
#include "documents.h"

HybridResult *hybrid_search(Query *query, Corpus *corpus, SkipGram *model, float alpha) {
  HybridResult *result = calloc(corpus->documents_count, sizeof(HybridResult));

  if (result == NULL) {
    printf("%sPHILIA:%s Error on allocating memory for the HybridResult in hybrid_search() function.\n", BRIGHT_GREEN, RESET);
    return NULL;
  }

  for (int i = 0; i < corpus->documents_count; i++) {
    result[i].document_id = i;
    result[i].semantic_score = cosine_similarity(corpus->documents[i].embeddings, query->embeddings, model->embedding_dim);
    result[i].tfidf_score = (float)document_cosine_similarity(query->document, corpus->documents[i]);
    result[i].hybrid_score = (alpha * result[i].semantic_score) + ((1 - alpha) * result[i].tfidf_score);

    for (int j = 0; j < i; j++) {
      if (result[i].hybrid_score > result[j].hybrid_score) {
        HybridResult tmp = result[i];
        result[i] = result[j];
        result[j] = tmp;
      }
    }
  }

  return result;
}
