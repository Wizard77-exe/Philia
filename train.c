#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

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
#include "persistence.h"
#include "activations.h"

void shuffle_training_pairs(TrainingSet *ts)
{
    for (int i = ts->count - 1; i > 0; i--) {

        int j = rand() % (i + 1);

        TrainingPair temp = ts->pairs[i];
        ts->pairs[i] = ts->pairs[j];
        ts->pairs[j] = temp;
    }
}

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

  FILE *loss_file = fopen("loss.csv", "w");
  fprintf(loss_file, "epoch,loss\n");

  float previous_loss = FLT_MAX;
  float lr = LEARNING_RATE;

  for (int epoch = 0; epoch < EPOCHS; epoch++) {

    float epoch_loss = 0.0f;

    // shuffle pairs
    shuffle_training_pairs(&ts);

    if (epoch % 100 == 0)
      lr *= 0.5f;

    for (int pair = 0; pair < ts.count; pair++) {

        // Forward
        ForwardPass forward = forward_skipgram(&model, ts.pairs[pair].center); 
        softmax(forward.logits, forward.probabilities, forward.vocabulary_size);
        // Loss
        float loss = compute_cross_entropy_loss(&forward, ts.pairs[pair].context);        
        // Backward
        ExpectedDistribution expected = create_expected_distribution(t.vocabulary.count, ts.pairs[pair].context);
        BackwardPass backward = backward_skipgram(&forward, &expected);
        // Gradient Descent
        gradient_descent(&model, &backward, ts.pairs[pair].center, lr);
        epoch_loss += loss;

      free_forwardpass(&forward);
      free_expected_distribution(&expected);
      free_backwardpass(&backward);
    }

    epoch_loss /= ts.count;
    
    fprintf(loss_file, "%d,%f\n", epoch + 1, epoch_loss);
    printf("Epoch %3d | Average Loss: %.6f\n",
           epoch + 1,
           epoch_loss);

    /*if (fabs(previous_loss - epoch_loss) < 1e-6f) {
      printf("\nTrainer-san: Converged at epoch %d!\n", epoch + 1);
      break;
    }*/

    previous_loss = epoch_loss;

  }

  if (!save_embeddings("models/embedding.bin", &model)) {
    printf("Error on saving embeddings\n");
    fclose(loss_file);
    free_skipgram(&model);
    free_training_set(&ts);
    free_embedding_matrix(&matrix);
    free_corpus(&t);
    return 1;
  }

  /*SkipGram v = create_skipgram(t.vocabulary.count, EMBEDDING_DIM);
  if (!load_embeddings("models/embedding.bin", &v)) {
    fclose(loss_file);
    free_skipgram(&model);
    free_training_set(&ts);
    free_embedding_matrix(&matrix);
    free_corpus(&t);
    return 1;
  }
  
  for (int i = 0; i < v.vocabulary_size; i++) {
    printf("Vector #%d\n", i + 1);
    for (int j = 0; j < v.embedding_dim; j++) {
      printf("INPUT: %20s OUTPUT: %20s\n", model.input.vectors[i].values[j] == v.input.vectors[i].values[j]? "true":"false", model.output.vectors[i].values[j] == v.output.vectors[i].values[j]?"true":"false");
    }
  }*/

  // NOTE: freeing area;
  //free_forwardpass(&fp);
  fclose(loss_file);
  //free_skipgram(&v);
  free_skipgram(&model);
  free_embedding_matrix(&matrix);
  free_corpus(&t);
  free_training_set(&ts);
  //free_document(&dt);
  return 0;
}
