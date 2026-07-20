#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "embedding.h"
#include "vocabulary.h"

static Neighbor *compute_neighbors(SkipGram *model, Vocabulary *vocabulary, int word_id) {
  Neighbor *neighbors = malloc(sizeof(Neighbor) * vocabulary->count);

  // checking
  if (neighbors == NULL) {
    printf("ERROR: allocating memory for Neighbors.\n");
    return NULL;
  }

  for (int i = 0; i < vocabulary->count; i++) {
    neighbors[i].id = i;

    neighbors[i].similarity = embedding_cosine_similarity(&model->input.vectors[word_id], &model->input.vectors[i], model->embedding_dim);
  }

  return neighbors;
}

static void sort_neighbors(Neighbor *neighbors, int count) {
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < count; j++) {
      if (neighbors[j].similarity < neighbors[i].similarity) {
        Neighbor tmp = neighbors[i];
        neighbors[i] = neighbors[j];
        neighbors[j] = tmp;
      }
    }
  }
}

static void print_top_neighbors(Neighbor *neighbors, Vocabulary *vocabulary, int word_id, int top_k) {
  int printed = 0;

  for (int i = 0; i < vocabulary->count; i++) {
    if (neighbors[i].id == word_id)
      continue;

    printf("%d. %-15s(ID:%3d)              sim=%f\n", printed+1, vocabulary->terms[neighbors[i].id].word, neighbors[i].id, neighbors[i].similarity);

    printed++;
    
    if (printed == top_k)
      break;
  }
}

void evaluate_word(SkipGram *model, Vocabulary *vocabulary, const char *word, int top_k) {
  int id = vocabulary_index(*vocabulary, word);
  
  if (id == -1) {
    printf("Word Not Found!\n");
    return;
  }

  Neighbor *neighbors = compute_neighbors(model, vocabulary, id);

  if (neighbors == NULL) {
    printf("Error creating neighbors!\n");
    return;
  }

  sort_neighbors(neighbors, vocabulary->count);
  print_top_neighbors(neighbors, vocabulary, id, top_k);
  
  free(neighbors);
}
