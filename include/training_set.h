#ifndef TRAINING_SET_H
#define TRAINING_SET_H

#include "structures.h"

TrainingSet build_training_set(Corpus c, int window_size);
void shuffle_training_pairs(TrainingSet *ts);

#endif
