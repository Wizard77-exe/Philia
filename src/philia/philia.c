#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "corpus.h"
#include "skipgram.h"
#include "structures.h"
#include "setfree.h"
#include "persistence.h"
#include "semantic.h"
#include "embedding.h"
#include "query.h"
#include "hybridsearch.h"


int main(void)
{
  /* NOTE: SPINNER ANIMATION LATER IF EVERYTHING'S STABLE

  const char *spinner[] = {
      "⠋", "⠙", "⠹", "⠸", "⠼",
      "⠴", "⠦", "⠧", "⠇", "⠏"
  };

  int frame = 0;

  while (1) {
    printf("\r%s Training Philia...", spinner[frame]);
    fflush(stdout);

    usleep(80000);           // 80 ms

    frame = (frame + 1) % 10;
  }*/

  // Making the corpus.
  Corpus corpus = build_corpus();

  // Loading the Model.
  SkipGram model = create_skipgram(corpus.vocabulary.count, EMBEDDING_DIM);
  if (!model.success) {
    printf("Error creating SkipGram.\n");
    free_corpus(&corpus);
    return 1;
  }

  if (!load_embeddings("models/embeddings.bin", &model)) {
    printf("Error Loading Model.\n");
    free_corpus(&corpus);
    free_skipgram(&model);
    return 1;
  }

  // building the documents embeddings.
  build_document_embeddings(&model, &corpus);
  normalize_corpus_embeddings(&corpus, EMBEDDING_DIM);

  char prompt[4096];
  printf("Enter sentence: ");
  fgets(prompt, sizeof(prompt), stdin);
  prompt[strcspn(prompt, "\n")] = '\0';

  Query query = build_query(&corpus, &model, prompt);

  HybridResult *result = hybrid_search(&query, &corpus, &model, ALPHA);

  for (int i = 0; i < corpus.documents_count; i++) {
    printf("%-2d. Document (ID:%-2d) Semantic_score: (%.6f) TF-IDF_score: (%.6f) Hybrid_score: (%.6f)\n"
           , i + 1
           , result[i].document_id
           , result[i].semantic_score
           , result[i].tfidf_score
           , result[i].hybrid_score);
  }

  /*printf("Term: %-15s ID: %-3d TF: %-3.6f IDF: %-3.6f TF-IDF: %-3.6f\n"
         , corpus.documents[0].terms[0].word
         , corpus.documents[0].terms[0].id
         , corpus.documents[0].terms[0].tf
         , corpus.vocabulary.terms[corpus.documents[0].terms[0].id].idf
         , corpus.documents[0].tf_idf_values[corpus.documents[0].terms[0].id]);

  printf("%f\n", model.input.vectors[0].values[0]);*/

  // NOTE: FREEING AREA.
  free_corpus(&corpus);
  free_skipgram(&model);
  free_query(&query);
  free(result);

  return 0;
}
