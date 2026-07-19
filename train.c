#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "structures.h"
#include "normalize.h"
#include "documents.h"
#include "vocabulary.h"
#include "corpus.h"
#include "setfree.h"
#include "training_set.h"
#include "embedding.h"
#include "skipgram.h"
#include "loss.h"
#include "backpropagation.h"

int main() {

  srand(time(NULL));

  Corpus t = build_corpus();
  t.vocabulary = build_vocabulary(t.documents, t.documents_count);

  compute_idf(&t.vocabulary, t.documents_count);
  apply_idf(t.documents, t.documents_count, t.vocabulary);
  compute_tfidf(t.documents, t.documents_count);
  document_magnitude(t.documents, t.documents_count);

  copy_id_to_documents(t.documents, t.documents_count, t.vocabulary);
  copy_id_to_tokens(t.tokens, t.documents_count, t.vocabulary);

  TrainingSet ts = build_training_set(t, WINDOW_SIZE);

  if (!ts.success) {
    printf("HAHAHAHA WRONGGGGG!!!\n\n");
    free_corpus(&t);
    return 1;
  }
  //printf("Vocabulary Count: %d\n", t.vocabulary.count);
  EmbeddingMatrix matrix = create_embedding_matrix(t.vocabulary.count, EMBEDDING_DIM);
  if (!matrix.success) {
    printf("Something wrong in the create_embedding_matrix()\n");
    free_corpus(&t);
    free_training_set(&ts);
    return 1;
  }

  random_initialize_embedding(&matrix);

  float cosine_similarity = embedding_cosine_similarity(&matrix.vectors[ts.pairs[3].center], &matrix.vectors[ts.pairs[3].context], matrix.dimension);

  SkipGram model = create_skipgram(t.vocabulary.count, EMBEDDING_DIM);

  if (!model.success) {
    printf("Error creating skipgram.\n");
    free_embedding_matrix(&matrix);
    free_corpus(&t);
    free_training_set(&ts);
    return 1;
  }

  int sample = 0;   // one training pair

printf("===============================================\n");
printf("BEFORE TRAINING\n");
printf("===============================================\n");

ForwardPass before =
    forward_skipgram(&model, ts.pairs[sample].center);

softmax(&before);

float loss_before =
    compute_cross_entropy_loss(
        &before,
        ts.pairs[sample].context
    );

printf("Center : %s\n",
       t.vocabulary.terms[ts.pairs[sample].center].word);

printf("Target : %s\n",
       t.vocabulary.terms[ts.pairs[sample].context].word);

printf("Loss Before : %f\n", loss_before);

printf("\n");

ExpectedDistribution expected =
    create_expected_distribution(
        before.vocabulary_size,
        ts.pairs[sample].context
    );

BackwardPass backward =
    backward_skipgram(
        &before,
        &expected
    );

/* ---------- DEBUG ---------- */

printf("Input Embedding Before:\n");
for (int d = 0; d < 5; d++)
    printf("%f ", model.input.vectors[ts.pairs[sample].center].values[d]);
printf("\n");

printf("Output Embedding Before:\n");
for (int d = 0; d < 5; d++)
    printf("%f ", model.output.vectors[ts.pairs[sample].context].values[d]);
printf("\n\n");

/* ---------- LEARNING ---------- */

gradient_descent(
    &model,
    &backward,
    ts.pairs[sample].center,
    LEARNING_RATE
);

/* ---------- DEBUG ---------- */

printf("Input Embedding After:\n");
for (int d = 0; d < 5; d++)
    printf("%f ", model.input.vectors[ts.pairs[sample].center].values[d]);
printf("\n");

printf("Output Embedding After:\n");
for (int d = 0; d < 5; d++)
    printf("%f ", model.output.vectors[ts.pairs[sample].context].values[d]);
printf("\n\n");

/* ---------- VERIFY ---------- */

ForwardPass after =
    forward_skipgram(&model, ts.pairs[sample].center);

softmax(&after);

float loss_after =
    compute_cross_entropy_loss(
        &after,
        ts.pairs[sample].context
    );

printf("===============================================\n");
printf("AFTER TRAINING\n");
printf("===============================================\n");

printf("Loss After : %f\n", loss_after);

printf("Difference : %f\n", loss_before - loss_after);

free_forwardpass(&before);
free_forwardpass(&after);
free_expected_distribution(&expected);
free_backwardpass(&backward);

  

  // NOTE: freeing area;
  //free_forwardpass(&fp);
  free_skipgram(&model);
  free_embedding_matrix(&matrix);
  free_corpus(&t);
  free_training_set(&ts);
  //free_document(&dt);
  return 0;
}
