#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar.h"
#include "stdphilia.h"

char *get_value(Fact *knowledge, int knowledge_count, char **subjects, int subjects_count, char **attributes, int attributes_count, char *prompt, Fact *context) {
  char *subject = extract_subject(subjects, subjects_count, prompt);
  char *attribute = extract_attribute(attributes, attributes_count, prompt);
  char *value = NULL;

  if (subject == NULL && attribute == NULL && strstr(prompt, "who")) {
    for (int i = 0; i < knowledge_count; i++) {
      if (strstr(prompt, knowledge[i].value)) {
        value = strdup(knowledge[i].subject);
        break;
      }
    }
  }

  if (value != NULL) {
    free(subject);
    free(attribute);
    return value;
  }

  if (subject == NULL && has_pronoun(prompt)) {
    subject = strdup(context->subject);
  }

  if (attribute == NULL) {
    attribute = strdup(context->attribute);
  }

  // searching for value from the knowledge array.
  for (int i = 0; i < knowledge_count; i++) {
    if ((strcmp(knowledge[i].subject, subject) == 0) && (strcmp(knowledge[i].attribute, attribute) == 0)) {
      value = strdup(knowledge[i].value);
      break;
    }
  }

  store_context(context, subject, attribute, value);

  free(subject);
  free(attribute);

  return value;
}

int learn(Fact **knowledges, int *knowledges_count, Fact context) {
  printf("\n%sPHILIA:%s What should I answer?\n\n", BRIGHT_MAGENTA, RESET);
  printf("%sHAPPY:%s ", BRIGHT_CYAN, RESET);

  char input[256];
  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] = '\0';

  Fact *tmp = realloc(*knowledges, sizeof(Fact) * (*knowledges_count + 1));
  if (tmp == NULL) {
    return 0;
  }

  *knowledges = tmp;

  (*knowledges)[*knowledges_count].subject = strdup(context.subject);
  (*knowledges)[*knowledges_count].attribute = strdup(context.attribute);
  (*knowledges)[*knowledges_count].value = strdup(input);

  (*knowledges_count)++;

  printf("\n%sPHILIA:%s Got it!\n\n", BRIGHT_MAGENTA, RESET);

  return 1;
}

void save_new_knowledge(char *subject, char *attribute, char *value) {
  FILE *file = fopen("data/knowledge.null", "a");
  if (file == NULL) {
    printf("%sPHILIA:%sI'm sorry, I failed to save the new knowledge on file\n\n", BRIGHT_MAGENTA, RESET);
    return;
  }

  fprintf(file, "%s.%s=%s\n", subject, attribute, value);

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s Ooops! There's some error closing the knowledges.null after saving new knowledge!\n\n", BRIGHT_MAGENTA, RESET);
  }
}
