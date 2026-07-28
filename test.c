#include <stdio.h>

#include "cbow.h"
#include "setfree.h"
#include "structures.h"
#include "corpus.h"

int main() {
  Corpus corpus = build_corpus();

  CBOWTrainingSet cbow = build_cbow_samples(corpus, WINDOW_SIZE);

  if (!cbow.success) {
    printf("ERROR\n");
    free_corpus(&corpus);
  }

  for (int i = 0; i < cbow.samples_count; i++) {
    printf("CBOW Sample #%d\n", i + 1);
    printf("Center Word: %-15s Context Words: [", corpus.vocabulary.terms[cbow.samples[i].center].word);
    for (int j = 0; j < cbow.samples[i].context_count; j++) {
      printf("%-10s ", corpus.vocabulary.terms[cbow.samples[i].context[j]].word);
    }
    printf("]\n\n");
  }

  free_corpus(&corpus);
  free_cbow_training_set(&cbow);
  return 0;
}
