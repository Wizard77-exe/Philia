#ifndef SETFREE_H
#define SETFREE_H

#include "structures.h"

// NOTE: For freeing an array of string with size count.
void free_string_array(char **s, int count);

// NOTE: For freeing tokens.
void free_tokens(Tokens *t);

// NOTE: For freeing Documents.
void free_document(Document *d);

// NOTE: For freeing Vocabulary.
void free_vocabulary(Vocabulary *v);

// NOTE: For freeing An entire corpus.
void free_corpus(Corpus *c);

// NOTE: For freeing the TrainingSet
void free_training_set(TrainingSet *t);

// NOTE: For freeing the EmbeddingMatrix
void free_embedding_matrix(EmbeddingMatrix *matrix);

// NOTE: For freeing the SkipGram
void free_skipgram(SkipGram *model);

// NOTE: For freeing the ForwardPass;
void free_forwardpass(ForwardPass *fp);

// NOTE: For freeing the ExpectedDistribution
void free_expected_distribution(ExpectedDistribution *expected);

// NOTE: For freeing the BackwardPass
void free_backwardpass(BackwardPass *backward);

#endif
