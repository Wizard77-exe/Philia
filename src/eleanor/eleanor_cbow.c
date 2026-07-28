#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#include "embedding.h"
#include "setfree.h"
#include "structures.h"
#include "skipgram.h"
#include "corpus.h"
#include "cbow.h"
#include "loss.h"
#include "persistence.h"
#include "optimizers.h"
#include "activations.h"
#include "backpropagation.h"
#include "training_set.h"

int main() {
  srand(time(NULL));

  Corpus corpus = build_corpus();
  // Checking corpus.
  if (!corpus.success) {
    fprintf(stderr, "Error building corpus.\n");
    free_corpus(&corpus);
    return 1;
  }

  CBOWTrainingSet samples = build_cbow_samples(corpus, WINDOW_SIZE);
  if (!samples.success) {
    fprintf(stderr, "Error building training set (CBOW).\n");
    free_corpus(&corpus);
    free_cbow_training_set(&samples);
    return 1;
  }

  SkipGram model = create_skipgram(corpus.vocabulary.count, EMBEDDING_DIM);
  if (!model.success) {
    fprintf(stderr, "Error building a model.\n");
    free_corpus(&corpus);
    free_cbow_training_set(&samples);
    return 1;
  }

  AdamState adam = create_adam(model.vocabulary_size, EMBEDDING_DIM, BETA1, BETA2, EPSILON, LEARNING_RATE);
  if (!adam.success) {
    fprintf(stderr, "Error building a model.\n");

  }

  FILE *loss_file = fopen("loss.csv", "w");
  fprintf(loss_file, "epoch,loss\n");

  float previous_loss = FLT_MAX;

  for (int epoch = 0; epoch < EPOCHS; epoch++) {
    float epoch_loss = 0.0f;
    // shuffle.
    shuffle_cbow_samples(&samples);

    for (int sample = 0; sample < samples.samples_count; sample++) {
      // Context IDs.
      EmbeddingVector hidden = cbow_hidden_vector(&model.input, &samples.samples[sample]);

      // CBOW Forward Pass.
      CBOWForward forward = forward_cbow(&model, &hidden);
      // check if forward.success == true.

      // probabilities.
      softmax(forward.logits, forward.probabilities, model.vocabulary_size);

      //expected distribution.
      ExpectedDistribution expected = create_expected_distribution(model.vocabulary_size, samples.samples[sample].center);

      // cross entropy loss.
      float loss = compute_cross_entropy_loss(forward.probabilities, samples.samples[sample].center);

      // backward pass.
      BackwardPass backward = backward_skipgram(&model, &forward, &expected, samples.samples[sample].center);
      // center.
      
      // loop for adam.
      adam.timestep++;
      // Update context embedding.
      for (int ctx = 0; ctx < samples.samples[sample].context_count; ctx++) {
        int word = samples.samples[sample].context[ctx];

        for (int dim = 0; dim < model.embedding_dim; dim++) {
          float gradient = backward.d_hidden[dim] / samples.samples[sample].context_count;

          update_adam_parameter(
                                &model.input.vectors[word].values[dim],
                                gradient,
                                &adam.input_direction.vectors[word].values[dim],
                                &adam.input_magnitude.vectors[word].values[dim],
                                &adam
          );
        }
      }

      // Update the output.
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

      free(hidden.values);
      free_forward_cbow(&forward);
      free_expected_distribution(&expected);
      free_backwardpass(&backward);
    }

    epoch_loss /= samples.samples_count;

    fprintf(loss_file, "%d,%f\n", epoch+1, epoch_loss);
    printf("Epoch %3d.......Average Loss: %f\n", epoch + 1, epoch_loss);
  }

  // normalizing the embeddings.
  normalize_embedding_matrix(&model);

  // saving the model.
  if (!save_embeddings("models/embeddings.bin", &model)) {
    printf("Model failed to save.\n");
  }

  // NOTE: FREEING AREA.
  fclose(loss_file);
  free_corpus(&corpus);
  free_cbow_training_set(&samples);
  free_skipgram(&model);
  free_embedding_matrix(&adam.input_direction);
  free_embedding_matrix(&adam.input_magnitude);
  free_embedding_matrix(&adam.output_direction);
  free_embedding_matrix(&adam.output_magnitude);

  return 0;
}

