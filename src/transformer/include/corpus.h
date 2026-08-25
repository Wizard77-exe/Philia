#ifndef CORPUS_H
#define CORPUS_H

#define VOCABULARY_ERROR ((Vocabulary){ \
                            .vocabulary = NULL, \
                            .size = 0, \
                            .capacity = 0 \
                         })

typedef struct {
  char *word;
  int id;
} VocabularyTerm;

typedef struct {
  VocabularyTerm *vocabulary;
  int size;
  int capacity;
} Vocabulary;

#endif
