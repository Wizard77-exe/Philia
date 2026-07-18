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

  ForwardPass fp = forward_skipgram(&model, ts.pairs[3].center);
  if (!fp.success) {
    printf("Error creating ForwardPass\n");
    free_embedding_matrix(&matrix);
    free_corpus(&t);
    free_training_set(&ts);
    free_skipgram(&model);
    return 1;
  }

  softmax(&fp);

  float sum = 0.0f;

  for (int i = 0; i < fp.vocabulary_size; i++) {
    printf("Logits: %-30.6f Probability: %-70.6f\n", fp.logits[i], fp.probabilities[i]);
    sum += fp.probabilities[i];
  }
  
  printf("Sum: %f\n", sum);

  float loss = compute_cross_entropy_loss(&fp, ts.pairs[3].context);

  printf("Cross Entropy Loss: %f\n", loss);
  /*for (int i = 0; i < fp.vocabulary_size; i++) {
    printf("Input: 15 Output: %3d Logits: %20.6f\n", i+1, fp.logits[i]);
  }
  /*for (int i = 0; i < model.vocabulary_size; i++) {
    for (int j = 0; j < model.embedding_dim; j++) {
      printf("Input: %.6f               Output: %.6f\n", model.input.vectors[i].values[j], model.output.vectors[i].values[j]);
    }
  }
  /*printf("Cosine Similarity of 15 and 10: %.6f\n", cosine_similarity);

  EmbeddingVector *v = get_embedding(&matrix, 26);
  if (v == NULL) {
    printf("get_embedding() returned a NULL\n");
    free_embedding_matrix(&matrix);
    free_corpus(&t);
    free_training_set(&ts);
    return 1;
  }

  /*for (int i = 0; i < EMBEDDING_DIM; i++) {
    printf("#%d == %.6f\n", i + 1, v->values[i]);
  }
  /*for (int i = 0; i < matrix.vocabulary_size; i++) {
    for (int j = 0; j < matrix.dimension; j++) {
      printf("%.6f \n", matrix.vectors[i].values[j]);
    }
    printf("\n");
  }

  for (int i = 0; i < t.documents_count; i++) {
    printf("Documents #%d\n", i+1);
    for (int j = 0; j < t.documents[i].count; j++) {
      printf("%s\n", t.documents[i].terms[j].word);
    }
    printf("\n");
  }

  for (int i = 0; i < ts.count; i++) {
    printf("(\"%s\", \"%s\")\n", t.vocabulary.terms[ts.pairs[i].center].word, t.vocabulary.terms[ts.pairs[i].context].word);
  }*/

  

  // NOTE: freeing area;
  free_forwardpass(&fp);
  free_skipgram(&model);
  free_embedding_matrix(&matrix);
  free_corpus(&t);
  free_training_set(&ts);
  //free_document(&dt);
  return 0;
}
