#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

#define WINDOW_SIZE   2
#define EMBEDDING_DIM 64
#define LEARNING_RATE 0.001f
#define EPSILON       1e-8f
#define BETA1         0.9f
#define BETA2         0.999f
#define EPOCHS        100
#define ALPHA         0.5f

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
} DocumentTerm;

typedef struct {
    DocumentTerm *terms;

    float *tf_idf_values;     // array to store tf_idf vector.
    float *embeddings;        // average embedding of Document.

    float embedding_magnitude;  // magnitude of the embedding vector.
    float tf_idf_magnitude;     // magnitude of the tf_idf vector.

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
    Vocabulary vocabulary;
    Document *documents;
    Tokens *tokens;

    int documents_count;
    int documents_capacity;

    bool success;
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
  EmbeddingVector *vectors;   // size of vocabulary_size. each vector has size of dimension.

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
    float *d_logits;

    float *d_hidden;

    EmbeddingMatrix output_gradients;

    bool success;
} BackwardPass;

typedef struct {
  int id;
  float similarity;
} Neighbor;

// NOTE: MAYBE ERASE LATER.
typedef struct {
  int documents_id;
  float similarity;
} SemanticResult;

// NOTE: HYBRID SEARCH.
typedef struct {
  int document_id;

  float semantic_score;
  float tfidf_score;
  float hybrid_score;
} HybridResult;

// NOTE: OPTIMIZERS
typedef struct {
  EmbeddingMatrix input_direction;
  EmbeddingMatrix input_magnitude;

  EmbeddingMatrix output_direction;
  EmbeddingMatrix output_magnitude;

  int timestep;

  float beta1;
  float beta2;

  float epsilon;

  float learning_rate;

  bool success;
} AdamState;

typedef struct {
  Tokens tokens;
  Document document;
  float *embeddings;

  bool success;
} Query;

// NOTE: For CBOW.
typedef struct {
  int *context;
  int center;
  int context_count;

  bool success;
} CBOWSample;

typedef struct {
  CBOWSample *samples;
  int capacity;
  int samples_count;

  bool success;
} CBOWTrainingSet;

typedef struct {
  float *logits;
  float *probabilities;

  int vocabulary_size;

  bool success;
} CBOWForward;

#endif
