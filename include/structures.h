#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

// from TF_IDF
typedef struct Term {
  char *word;
  int frequency;
  float tf;
  float idf;

  float tf_idf;
} Term;

typedef struct DocumentTerms {
  struct Term *terms;
  int count;
  bool success;
} DocumentTerms;

typedef struct VocabularyTerm {
  char *word;
  float idf;
  int df;
} VocabularyTerm;

typedef struct Vocabulary {
  VocabularyTerm *terms;
  int count;
  int capacity;
  bool success;
} Vocabulary;
// from NLP
typedef struct Token {
  float score;
  char *word;
} Token;

typedef struct Tokens {
  Token *tokens;

  int count;
  int capacity;

  bool success;
} Tokens;

typedef struct Fact {
  char *subject;
  char *attribute;
  char *value;
} Fact;

typedef struct IndexedFact {
  Tokens tokens;
  DocumentTerms terms;
  Fact *fact;
} IndexedFact;

typedef struct Synonym {
  char *synonym;
  char *canonical;
} Synonym;

// Query Representation
typedef struct Query {
  Tokens tokens;
  DocumentTerms terms;
} Query;

#endif
