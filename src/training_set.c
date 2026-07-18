#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "setfree.h"
#include "structures.h"

TrainingSet build_training_set(Corpus c, int window_size) {
  TrainingSet t = {0};

  t.capacity = 32;
  t.pairs = calloc(t.capacity, sizeof(TrainingPair));

  if (t.pairs == NULL) {
    printf("ERROR: Memory Allocation for TrainingPair.pairs\n\n");
    t.capacity = 0;
    return t;
  }

  for (int i = 0; i < c.documents_count; i++) {
    int left_edge = 0;
    int right_edge = c.tokens[i].count - 1;

    for (int j = 0; j < c.tokens[i].count; j++) {
      int center = j;

      for (int offset = center - WINDOW_SIZE; offset <= center + WINDOW_SIZE; offset++) {
        if (t.count >= t.capacity) {
          int old_capacity = t.capacity;
          t.capacity *= 2;

          TrainingPair *tmp = realloc(t.pairs, sizeof(TrainingPair) * t.capacity);

          if (tmp == NULL) {
            printf("ERROR: Memory Reallocation for TrainingSet.pairs.\n\n");
            free_training_set(&t);
            return t;
          }

          t.pairs = tmp;
          memset(t.pairs + old_capacity, 0, (t.capacity - old_capacity) * sizeof(TrainingPair));
        }

        if (offset < left_edge || offset > right_edge || offset == center) continue;

        t.pairs[t.count].center = c.tokens[i].tokens[center].id;
        t.pairs[t.count].context = c.tokens[i].tokens[offset].id;
        t.count++;
      }
    }
  }

  t.success = true;
  return t;
}
