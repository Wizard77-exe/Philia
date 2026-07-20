#ifndef VALIDATION_H
#define VALIDATION_H

#include "structures.h"

//Neighbor *compute_neighbors(SkipGram *model, Vocabulary *vocabulary, int word_id);

//void sort_neighbors(Neighbor *neighbors, int count);
//void print_top_neighbors(Neighbor *neighbors, Vocabulary *vocabulary, int word_id, int top_k);
void evaluate_word(SkipGram *model, Vocabulary *vocabulary, const char *word, int top_k);

#endif
