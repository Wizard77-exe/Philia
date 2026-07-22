#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "query.h"
#include "tokens.h"
#include "colors.h"
#include "setfree.h"
#include "documents.h"
#include "semantic.h"
#include "embedding.h"
#include "corpus.h"


Query build_query(Corpus *corpus, SkipGram *model, char *prompt) {
  // Empty initialization
  Query query = {0};

  // tokenizing and storing the tokens in query.tokens.
  query.tokens = tokenize(prompt);
  if (!query.tokens.success) {
    printf("%sPHILIA:%s Error on tokenizing query in build_query() function.\n", BRIGHT_GREEN, RESET);
    return query;
  }

  // copies IDs of each word form the corpus' vocabulary.
  copy_id_to_tokens(&query.tokens, 1, corpus->vocabulary);

  // Creating the Query Document
  query.document = compute_tf(query.tokens);
  if (!query.document.success) {
    printf("%sPHILIA:%s Error on creating document in build_query() function.\n", BRIGHT_GREEN, RESET);
    free_tokens(&query.tokens);
    return query;
  }

  copy_id_to_documents(&query.document, 1, corpus->vocabulary);

  apply_idf(&query.document, corpus->vocabulary);
  compute_tfidf(&query.document);
  document_magnitude(&query.document, 1);

  query.embeddings = calloc(model->embedding_dim, sizeof(float));

  if (query.embeddings == NULL) {
    printf("%sPHILIA:%s Error on allocating memory for the Query.embeddings inside the build_query() function.\n", BRIGHT_GREEN, RESET);
    free_query(&query);
    return query;
  }

  // getting the sentence embeddings
  sentence_embedding(model, &query.tokens, query.embeddings);
  normalize_query_embeddings(query.embeddings, model->embedding_dim);

  query.success = true;
  return query;
}
