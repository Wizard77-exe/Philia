#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdbool.h>

// from TF_IDF
struct Term {
  char *word;
  int frequency;
  float tf;
  float idf;

  float tf_idf;
};

struct DocumentTerms {
  struct Term *terms;
  int count;
  bool success;
};

struct VocabularyTerm {
  char *word;
  float idf;
  int df;
};

struct Vocabulary {
  struct VocabularyTerm *terms;
  int count;
  int capacity;
  bool success;
};
// from NLP
struct Token {
  float score;
  char *word;
};

struct Tokens {
  struct Token *tokens;

  int count;
  int capacity;

  bool success;
};

struct Keywords{
  struct Token *words;

  int count;
  int capacity;

  bool success;
};

struct Query {
  char *subject;
  char *attribute;
};

struct Fact {
  char *subject;
  char *attribute;
  char *value;
};

struct IndexedFact {
  struct Tokens tokens;
  struct DocumentTerms terms;
  struct Fact *fact;
};

struct Synonym {
  char *synonym;
  char *canonical;
};

#endif
