#include <stdbool.h>

#ifndef NLP_H
#define NLP_H

typedef struct {
  float score;
  char *word;
} Token;

typedef struct {
  int count;
  int capacity;

  Token *tokens;

  bool success;
} Tokens;

typedef struct {
  int count;
  int capacity;

  Token *words;

  bool success;
} Keywords;

typedef struct {
  char *subject;
  char *attribute;
} Query;

typedef struct {
  char *subject;
  char *attribute;
  char *value;
} Fact;

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

typedef struct {
  char *synonym;
  char *canonical;
} Synonym;

void free_fact(Fact f);

void free_knowledges(Fact *knowledges, int knowledges_count);

Fact *load_knowledges(int *fact_count);

char **load_subjects(int *subjects_count);
char **load_attributes(int *attributes_count);

void normalize(char *prompt);
void free_tokens(Tokens *tokens);
void free_keywords(Keywords *k);
void free_query(Query *q);
Tokens tokenize(char *token);
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

void canonalize_tokens(Tokens *t, Synonym *s, int synonyms_count);
#endif
