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

  /*for (int i = 0; i < model.vocabulary_size; i++) {
    for (int j = 0; j < model.embedding_dim; j++) {
      printf("Input: %-10.6fOutput: %-10.6f\n", model.input.vectors[i].values[j], model.output.vectors[i].values[j]);
    }
  }*/

  build_document_embeddings(&model, &corpus);

  /*for (int i = 0; i < corpus.documents_count; i++) {
    printf("Document %d\n", i);

    for (int j = 0; j < corpus.tokens[i].count; j++) {
      printf("%s -> %d\n",
           corpus.tokens[i].tokens[j].word,
           corpus.tokens[i].tokens[j].id);
    }
  }*/

  printf("Enter sentence: ");
  char sentence[128];
  fgets(sentence, sizeof(sentence), stdin);
  sentence[strcspn(sentence, "\n")] = '\0';

  int *semantic_search_output = semantic_search(&model, &corpus, sentence, 3);
  if (semantic_search_output == NULL) {
    free_skipgram(&model);
    free_corpus(&corpus);
    return 1;
  }

  for (int i = 0; i < 3; i++) {
    printf("%d ", semantic_search_output[i]);
  }
  printf("\n");
  free(semantic_search_output);

  /*Tokens s_tokens = tokenize(sentence);
  copy_id_to_tokens(&s_tokens, 1, corpus.vocabulary);

  /*for (int i = 0; i < s_tokens.count; i++) {
    printf("%s ", s_tokens.tokens[i].word);
  }
  printf("\n");*/

  /*float *s_embeddings = calloc(EMBEDDING_DIM, sizeof(float));
  
  sentence_embedding(&model, &s_tokens, s_embeddings);

  SemanticResult *results = rank_documents_semantics(&corpus, s_embeddings, EMBEDDING_DIM);

  if (results == NULL) {
    free_tokens(&s_tokens);
    free(s_embeddings);
    free_skipgram(&model);
    free_corpus(&corpus);
    return 1;
  }
  
  int *tops = retrieve_top_k_documents_semantics(results, 3);

  if (tops == NULL) {
    free_tokens(&s_tokens);
    free(s_embeddings);
    free_skipgram(&model);
    free_corpus(&corpus);
  }

  for (int i = 0; i < 3; i++) {
    printf("%-3d. Documents ID: %-3d\n", i+1, tops[i]);
  }

  printf("\n");
  
  for (int i = 0; i < corpus.documents_count; i++) {
    printf("%-3d. Document ID: %-3d Similarity: %f\n", i + 1, results[i].documents_id, results[i].similarity);
  }

  printf("\nLet's Check:\n");
  for (int i = 0; i < corpus.documents_count; i++) {
    printf("Cosine Smilarity with document %d: %f\n", i + 1, cosine_similarity(corpus.documents[i].embeddings, s_embeddings, EMBEDDING_DIM));
  }*/ 



  /*for (int i = 0; i < 5; i++) {
    printf("%f ", s_embeddings[i]);
  }
  printf("\n");*/

  /*for (int i = 0; i < corpus.documents_count; i++) {
    printf("Cosine Similarity with document %d: %f\n", i + 1, cosine_similarity(corpus.documents[i].embeddings, s_embeddings, EMBEDDING_DIM));
  }*/

  /*free_tokens(&s_tokens);
  free(s_embeddings);
  free(results);
  free(tops);
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
