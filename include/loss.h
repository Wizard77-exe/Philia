#ifndef LOSS_H
#define LOSS_H

#include "structures.h"

float compute_cross_entropy_loss(ForwardPass *forward, int target_word_id);

#endif
