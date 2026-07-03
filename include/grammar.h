#ifndef GRAMMAR_H
#define GRAMMAR_H

typedef struct {
  char *subject;
  char *attribute;
  char *value;
} Fact;

typedef struct {
  char *subject;
  char *attribute;
} Context;

typedef enum {
    INTENT_GREETING,
    INTENT_FACT_QUERY,
    INTENT_UPDATE,
    INTENT_UNKNOWN
} Intent;

Fact *loadknowledge(int *fact_count);
void free_knowledge(Fact *knowledge, int fact_count);

char **load_subjects(int *subject_count);
char **load_attributes(int *attributes_count);

char *extract_subject(char **subjects, int count, char *prompt);
char *extract_attribute(char **attributes, int count, char *prompt);

void store_context(Context *context, char *subject, char *attribute);
void free_context(Context *context);

int has_pronoun(char *prompt);

Intent get_intent(char *prompt);

void update(char *prompt, char **subjects, int subjects_count, char **attributes, int attributes_count, Context context, Fact **knowledges, int knowledges_count);

#endif
