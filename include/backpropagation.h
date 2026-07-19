#ifndef BACKPROPAGATION_H
#define BACKPROPAGATION_H

#include "structures.h"

ExpectedDistribution create_expected_distribution(int vocabulary_size, int target_word_id);
BackwardPass backward_skipgram(ForwardPass *forward, ExpectedDistribution *expected);

void gradient_descent(SkipGram *model, BackwardPass *backward, int center_word_id, float learning_rate);

#endif
