#ifndef GRAMMAR_H
#define GRAMMAR_H

typedef struct {
  char *subject;
  char *attribute;
  char *value;
} Fact;

typedef struct Context {
  char *subject;
  char *attribute;
} Context;

Fact *loadknowledge(int *fact_count);
void free_knowledge(Fact *knowledge, int fact_count);

char **load_subjects(int *subject_count);
char **load_attributes(int *attributes_count);

char *extract_subject(char **subjects, int count, char *prompt);
char *extract_attribute(char **attributes, int count, char *prompt);

void store_context(Context *context, char *subject, char *attribute);
void free_context(Context *context);

int has_pronoun(char *prompt);

#endif
