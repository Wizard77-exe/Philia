#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "setfree.h"
#include "structures.h"
#include "colors.h"
#include "corpus.h"
#include "vocabulary.h"
#include "documents.h"
#include "embedding.h"
#include "semantic.h"
#include "persistence.h"
#include "skipgram.h"
#include "query.h"
#include "hybridsearch.h"

#define TOP_K 3

int main() {
  // NOTE: BUILDING CORPUS
  Corpus corpus = build_corpus();
  // NOTE: add bool success; field in the Corpus struct to know if it build successfully or not.

  // NOTE: BUILDING THE CORPUS' VOCABULARY
  corpus.vocabulary = build_vocabulary(corpus.documents, corpus.documents_count);
  if (!corpus.vocabulary.success) {
    printf("%sERROR:%s build_vocabulary().\n", BRIGHT_RED, RESET);
    free_corpus(&corpus);
    return 1;
  }

  // NOTE: COMPUTING AND ASSIGNING TF-IDF AND IDs TO DOCUMENTS AND VOCABULARY.
  compute_idf(&corpus.vocabulary, corpus.documents_count);
  apply_corpus_idf(corpus.documents, corpus.documents_count, corpus.vocabulary);
  compute_corpus_tfidf(corpus.documents, corpus.documents_count);
  document_magnitude(corpus.documents, corpus.documents_count);
  
  copy_id_to_documents(corpus.documents, corpus.documents_count, corpus.vocabulary);
  copy_id_to_tokens(corpus.tokens, corpus.documents_count, corpus.vocabulary);

  // TEST: 
  for (int i = 0; i < corpus.documents_count; i++) {
    printf("Document #%d: Magnitude: %f\n", i + 1, corpus.documents[i].magnitude);
    for (int j = 0; j < corpus.documents[i].count; j++) {
      printf("Term: %-15s ID: %-4d TF: %.6f IDF: %.6f TF-IDF: %-4.5f\n", corpus.documents[i].terms[j].word, corpus.documents[i].terms[j].id, corpus.documents[i].terms[j].tf, corpus.documents[i].terms[j].idf, corpus.documents[i].terms[j].tf_idf);
    }
  }

  // NOTE: CREATING AND LOADING TRAINED MODEL TO SKIPGRAM.
  SkipGram model = create_skipgram(corpus.vocabulary.count, EMBEDDING_DIM);

  if (!load_embeddings("models/embedding.bin", &model)) {
    printf("%sERROR:%s load_embedding().\n", BRIGHT_RED, RESET);
    free_corpus(&corpus);
    free_skipgram(&model);
    return 1;
  }

  // NOTE: computes and assign average embedding vectors of each corpus' documents.
  build_document_embeddings(&model, &corpus);
  normalize_corpus_embeddings(&corpus, EMBEDDING_DIM);

  // NOTE: THIS MIGHT BE CHANGED LATER: This only asks for the user's prompt.
  char prompt[4096];

  printf("%sHAPPY:%s ", BRIGHT_CYAN, RESET);
  fgets(prompt, sizeof(prompt), stdin);
  prompt[strcspn(prompt, "\n")] = '\0';         // REMOVES THE NEWLINE CHARACTER FROM FGETS AND CHANGES IT TO '\0'.
  
  // TEST: 
  Query query = build_query(&corpus, &model, prompt);
  if (!query.success) {
    free_corpus(&corpus);
    free_skipgram(&model);
    return 1;
  }

  // TEST: 
  printf("Sentence magnitude: %f\n", query.document.magnitude);
  for (int i = 0; i < query.document.count; i++) {
    printf("Term: %-15s ID: %-3d TF: %.6f IDF: %.6f TF-IDF: %-5.6f\n", query.document.terms[i].word, query.document.terms[i].id, query.document.terms[i].tf, query.document.terms[i].idf, query.document.terms[i].tf_idf);
  }

  HybridResult *hybridresult = hybrid_search(&query, &corpus, &model, 0.5);

  for (int i = 0; i < corpus.documents_count; i++) {
    printf("Document ID: %-3d  Semantic Score: %-10f TF-IDF Score: %-10f  Hybrid Score: %-10f\n", hybridresult[i].document_id, hybridresult[i].semantic_score, hybridresult[i].tfidf_score, hybridresult[i].hybrid_score);
  }

  // NOTE: Don't forget to free everything you've borrowed from the memory.
  free_corpus(&corpus);
  free_skipgram(&model);
  free_query(&query);
  free(hybridresult);
}
