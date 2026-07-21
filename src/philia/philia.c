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
  apply_idf(corpus.documents, corpus.documents_count, corpus.vocabulary);
  compute_tfidf(corpus.documents, corpus.documents_count);
  document_magnitude(corpus.documents, corpus.documents_count);

  copy_id_to_documents(corpus.documents, corpus.documents_count, corpus.vocabulary);
  copy_id_to_tokens(corpus.tokens, corpus.documents_count, corpus.vocabulary);

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

  // NOTE: THIS MIGHT BE CHANGED LATER: This only asks for the user's prompt.
  char prompt[4096];

  printf("%sHAPPY:%s ", BRIGHT_CYAN, RESET);
  fgets(prompt, sizeof(prompt), stdin);
  prompt[strcspn(prompt, "\n")] = '\0';         // REMOVES THE NEWLINE CHARACTER FROM FGETS AND CHANGES IT TO '\0'.
  
  SemanticResult *top_k_documents = semantic_search(&model, &corpus, prompt, TOP_K);
  if (top_k_documents == NULL) {
    printf("%sPHILIA:%s Dad? I think I failed on semantic_search() function.\n", BRIGHT_GREEN, RESET);
    free_corpus(&corpus);
    free_skipgram(&model);
    return 1;
  }

  // NOTE: TEST RUN.
  printf("Top Retrieved Documents (TOP %d):\n", TOP_K);
  for (int i = 0; i < TOP_K; i++) {
    printf("%d. Document #%-3d Similarity: %f\n", i+1, top_k_documents[i].documents_id, top_k_documents[i].similarity);
  }

  // NOTE: Don't forget to free everything you've borrowed from the memory.
  free_corpus(&corpus);
  free_skipgram(&model);
  free(top_k_documents);
}
