#ifndef NLP_H
#define NLP_H

#define SUBJECT_WEIGHT    1.0f
#define ATTRIBUTE_WEIGHT  1.0f
#define FACT_THRESHOLD (SUBJECT_WEIGHT + ATTRIBUTE_WEIGHT)

#include <stdbool.h>
#include "structures.h"

typedef struct Token Token;
typedef struct Tokens Tokens;
typedef struct Keywords Keywords;
typedef struct Query Query;
typedef struct Fact Fact;

typedef enum {
  INTENT_UNKNOWN,
  
  // social interaction
  INTENT_GREETING,
  INTENT_THANKS,
  INTENT_SORRY,
  INTENT_GOODBYE,

  // knowledge
  INTENT_UPDATE,
  INTENT_QUERY
} Intent;

typedef enum {
  ACTION_REPLY,         // for INTENT_QUERY
  ACTION_SEARCH,        // for social conversations
  ACTION_UPDATE         // for INTENT_UPDATE
} Action;

typedef struct Synonym Synonym;

typedef struct IndexedFact IndexedFact;

void free_fact(Fact f);

void free_knowledges(Fact *knowledges, int knowledges_count);

Fact *load_knowledges(int *fact_count);

char **load_subjects(int *subjects_count);
char **load_attributes(int *attributes_count);

void normalize(char *prompt);
void free_tokens(Tokens *tokens);
void free_keywords(Keywords *k);
void free_query(Query *q);
Tokens tokenize(char *input);
void score_tokens(Tokens *t, char **subjects, int subjects_count, char **attributes, int attributes_count);
Keywords extract_keywords(Tokens *t);
Query detect_query(Keywords *k, char **subjects, int subjects_count, char **attributes, int attributes_count);

Intent detect_intent(Tokens *t);
Fact *search_knowledges(Query *q, Fact *knowledges, int count);

void generate_response(Fact *fact);

Fact learn_fact(Query q);

void append_fact(Fact **knowledges, int *knowledges_count, Fact new_fact);
void save_knowledges(Fact *knowledges, int knowledges_count);

void free_synonyms(Synonym *s, int synonyms_count);

Synonym *load_synonyms(int *synonyms_count);

void canonicalize_tokens(Tokens *t, Synonym *s, int synonyms_count);

void free_indexed_fact(IndexedFact *f, int indexedfacts_count);
IndexedFact *build_indexed_fact(Fact *knowledges, int knowledges_count, int *indexed_facts_count);
#endif
