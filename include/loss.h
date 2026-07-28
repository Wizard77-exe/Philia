#ifndef LOSS_H
#define LOSS_H

#include "structures.h"

float compute_cross_entropy_loss(float *probabilities, int target_word_id);

#endif
