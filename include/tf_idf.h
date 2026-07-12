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
Vocabulary build_vocabulary(IndexedFact *facts, int count);

#endif
