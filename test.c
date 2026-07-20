#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "validation.h"
#include "corpus.h"
#include "vocabulary.h"
#include "documents.h"
#include "embedding.h"
#include "persistence.h"
#include "setfree.h"
#include "skipgram.h"
#include "semantic.h"
#include "tokens.h"
#include "normalize.h"

int main() {
  Corpus corpus = build_corpus();
  corpus.vocabulary = build_vocabulary(corpus.documents, corpus.documents_count);

  // NOTE: add all these things in the build_corpus() later.
  compute_idf(&corpus.vocabulary, corpus.documents_count);
  apply_idf(corpus.documents, corpus.documents_count, corpus.vocabulary);
  compute_tfidf(corpus.documents, corpus.documents_count);
  document_magnitude(corpus.documents, corpus.documents_count);
  copy_id_to_documents(corpus.documents, corpus.documents_count, corpus.vocabulary);
  copy_id_to_tokens(corpus.tokens, corpus.documents_count, corpus.vocabulary);

  SkipGram model = create_skipgram(corpus.vocabulary.count, EMBEDDING_DIM);

  if (!load_embeddings("models/embedding.bin", &model)) {
    free_corpus(&corpus);
    free_skipgram(&model);
    return 1;
  }

  build_document_embeddings(&model, &corpus);

  printf("Enter sentence: ");
  char sentence[128];
  fgets(sentence, sizeof(sentence), stdin);
  sentence[strcspn(sentence, "\n")] = '\0';

  normalize(sentence);
  Tokens s_tokens = tokenize(sentence);

  float *s_embeddings = malloc(sizeof(float) * EMBEDDING_DIM);

  sentence_embedding(&model, &s_tokens, s_embeddings);

  printf("Cosine Similarity: %f\n", cosine_similarity(corpus.documents[0].embeddings, s_embeddings, EMBEDDING_DIM));

  free_tokens(&s_tokens);
  free(s_embeddings);
  /*
  printf("Philia: \n");
  evaluate_word(&model, &corpus.vocabulary, "philia", 5);
  printf("===================================================================\n");
  printf("Eleanor: \n");
  evaluate_word(&model, &corpus.vocabulary, "eleanor", 5);
  printf("===================================================================\n");
  printf("Jaypee: \n");
  evaluate_word(&model, &corpus.vocabulary, "jaypee", 5);
  */

  free_skipgram(&model);
  free_corpus(&corpus);

  return 0;
}
