#ifndef TRAINING_SET_H
#define TRAINING_SET_H

#include "structures.h"

#define MAX_CONTEXT (WINDOW_SIZE * 2)

TrainingSet build_training_set(Corpus c, int window_size);
void shuffle_training_pairs(TrainingSet *ts);
void shuffle_cbow_samples(CBOWTrainingSet *samples);

#endif
