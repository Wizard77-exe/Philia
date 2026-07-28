#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#include "embedding.h"
#include "backpropagation.h"
#include "corpus.h"
#include "training_set.h"
#include "optimizers.h"
#include "skipgram.h"
#include "loss.h"
#include "structures.h"
#include "setfree.h"
#include "persistence.h"
#include "activations.h"

int main() {
  srand(time(NULL));

  Corpus corpus = build_corpus();

  TrainingSet ts = build_training_set(corpus, WINDOW_SIZE);
  // check if successful.
  
  SkipGram model = create_skipgram(corpus.vocabulary.count, EMBEDDING_DIM);
  // check if successful.
  

  AdamState adam = create_adam(model.vocabulary_size, EMBEDDING_DIM, BETA1, BETA2, EPSILON, LEARNING_RATE);
  // check if successful
  
  FILE *loss_file = fopen("loss.csv", "w");
  fprintf(loss_file, "epoch,loss\n");

  float previous_loss = FLT_MAX;

  for (int epoch = 0; epoch < EPOCHS; epoch++) {
    float epoch_loss = 0.0f;

    // shuffle training set.
    shuffle_training_pairs(&ts);

    for (int pair = 0; pair < ts.count; pair++) {
      // forward pass 
      ForwardPass forward = forward_skipgram(&model, ts.pairs[pair].center);
      // check.

      // softmax
      softmax(forward.logits, forward.probabilities, forward.vocabulary_size);

      // expected distribution
      ExpectedDistribution expected = create_expected_distribution(forward.vocabulary_size, ts.pairs[pair].context);
      //check.

      // cross entropy loss.
      float loss = compute_cross_entropy_loss(forward.probabilities, ts.pairs[pair].context);

      // backward pass.
      BackwardPass backward = backward_skipgram(&model, &forward, &expected, ts.pairs[pair].center);
      // check.
      

      // loop for Adam.
      adam.timestep++;

      // Update INPUT embedding
      for (int dim = 0; dim < model.embedding_dim; dim++) {
        update_adam_parameter(&model.input.vectors[ts.pairs[pair].center].values[dim]
                              , backward.d_hidden[dim]
                              , &adam.input_direction.vectors[ts.pairs[pair].center].values[dim]
                              , &adam.input_magnitude.vectors[ts.pairs[pair].center].values[dim]
                              , &adam);
      }

      // Update OUTPUT embedding.
      for (int word = 0; word < model.vocabulary_size; word++) {
        for (int dim = 0; dim < model.embedding_dim; dim++) {
          update_adam_parameter(&model.output.vectors[word].values[dim]
                                , backward.output_gradients.vectors[word].values[dim]
                                , &adam.output_direction.vectors[word].values[dim]
                                , &adam.output_magnitude.vectors[word].values[dim]
                                , &adam);
        }
      }

      epoch_loss += loss;

      free_forwardpass(&forward);
      free_expected_distribution(&expected);
      free_backwardpass(&backward);
    }

    epoch_loss /= ts.count;

    fprintf(loss_file, "%d,%f\n", epoch+1, epoch_loss);
    printf("Epoch %3d.....Average Loss: %.6f\n", epoch+1, epoch_loss);
  }

  // normalizing the embeddings.
  normalize_embedding_matrix(&model);

  if (!save_embeddings("models/embeddings.bin", &model)) {
    printf("Saving Failes!\n");
  }

  // NOTE: FREEING AREA.
  fclose(loss_file);
  free_corpus(&corpus);
  free_training_set(&ts);
  free_embedding_matrix(&adam.input_direction);
  free_embedding_matrix(&adam.input_magnitude);
  free_embedding_matrix(&adam.output_direction);
  free_embedding_matrix(&adam.output_magnitude);
  free_skipgram(&model);
  
  return 0;
}
