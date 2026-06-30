#ifndef GRAMMAR_H
#define GRAMMAR_H

typedef struct {
  char *subject;
  char *attribute;
  char *value;
} Fact;

Fact *loadknowledge(int *fact_count);
void free_knowledge(Fact *knowledge, int fact_count);

char **load_subjects(int *subject_count);
char **load_attributes(int *attributes_count);

char *extract_subject(char **subjects, int count, char *prompt);
char *extract_attribute(char **attributes, int count, char *prompt);

void store_context(Fact *context, char *subject, char *attribute, char *value);
void free_context(Fact *context);

int has_pronoun(char *prompt);

#endif
