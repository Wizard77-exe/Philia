#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

#define WINDOW_SIZE   2
#define EMBEDDING_DIM 64
#define LEARNING_RATE 0.01f
#define EPOCHS        1000

// from TF_IDF
typedef struct {
  char *word;
  int id;
} Token;
typedef struct {
  Token *tokens;

  int count;
  int capacity;

  bool success;
} Tokens;

typedef struct {
    char *word;

    int frequency;
    int id;

    float tf;
    float idf;
    float tf_idf;
} DocumentTerm;

typedef struct {
    DocumentTerm *terms;

    float magnitude;

    int count;
    int capacity;

    bool success;
} Document;

typedef struct {
  int df;
  int id;

  float idf;

  char *word;
} VocabularyTerm;

typedef struct {
    VocabularyTerm *terms;

    int count;
    int capacity;

    bool success;
} Vocabulary;

typedef struct {
    Document *documents;
    Tokens *tokens;

    int documents_count;
    int documents_capacity;

    Vocabulary vocabulary;
} Corpus;

typedef struct {
  int center;
  int context;
} TrainingPair;

typedef struct {
  TrainingPair *pairs;

  int count;
  int capacity;

  bool success;
} TrainingSet;

typedef struct {
  float *values;
} EmbeddingVector;

typedef struct {
  EmbeddingVector *vectors;

  int vocabulary_size;
  int dimension;

  bool success;
} EmbeddingMatrix;

typedef struct {
  EmbeddingMatrix input;
  EmbeddingMatrix output;

  int vocabulary_size;
  int embedding_dim;

  bool success;
} SkipGram;

typedef struct {
  float *logits;
  float *probabilities;

  int vocabulary_size;

  bool success;
} ForwardPass;

typedef struct {
  float *expected;

  int vocabulary_size;

  bool success;
} ExpectedDistribution;

typedef struct {
  float *gradients;

  int vocabulary_size;

  bool success;
} BackwardPass;

// Query Representation
typedef struct Query {
  Tokens tokens;
  Document terms;
} Query;

#endif
