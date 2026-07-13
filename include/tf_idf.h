#ifndef TF_IDF_H
#define TF_IDF_H

#include "structures.h"

typedef struct Term Term;
typedef struct DocumentTerms DocumentTerms;
typedef struct Tokens Tokens;
typedef struct IndexedFact IndexedFact;
typedef struct VocabularyTerm VocabularyTerm;
typedef struct Vocabulary Vocabulary;

void free_DT(DocumentTerms *dt);
DocumentTerms compute_tf(Tokens tokens);
void compute_indexedFact_tf(IndexedFact *f, int count);

void free_vocabulary(Vocabulary *v);
int vocabulary_index(Vocabulary v, const char *word);
Vocabulary build_vocabulary(IndexedFact *facts, int count);
void compute_idf(Vocabulary *v, int N);
void apply_idf(IndexedFact *facts, int indexedFact_count, Vocabulary v);
void compute_tfidf(IndexedFact *facts, int indexedFact_count);

#endif
