#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "grammar.h"
#include "stdphilia.h"

char *resolve_name(Fact *knowledge, int knowledge_count, char *prompt) {
  char *subject = NULL;

  char *copy = NULL;

  for (int i = 0; i < knowledge_count; i++) {
    if (strcmp(knowledge[i].attribute, "name") == 0) {
      copy = NULL;
      copy = strdup(knowledge[i].value);
      lowercase_s(copy);

      if (strstr(prompt, copy)) {
        subject = strdup(knowledge[i].subject);
        break;
      }
    }
  }
  
  free(copy);
  return subject;
}

char *get_value(Fact *knowledge, int knowledge_count, char **subjects, int subjects_count, char **attributes, int attributes_count, char *prompt, Context *context) {
  char *subject = extract_subject(subjects, subjects_count, prompt);
  char *attribute = extract_attribute(attributes, attributes_count, prompt);
  char *value = NULL;

  char *copy = NULL;

  if (subject == NULL && attribute == NULL && strstr(prompt, "who")) {
    for (int i = 0; i < knowledge_count; i++) {
      copy = NULL;
      copy = strdup(knowledge[i].value);    // making the copy of the current value;
      lowercase_s(copy);
      if (strstr(prompt, copy)) {
        value = strdup(knowledge[i].subject);
        break;
      }
    }
  }

  if (value != NULL) {
    free(copy);
    free(subject);
    free(attribute);
    return value;
  }

  if (subject == NULL || has_pronoun(prompt)) {
    subject = strdup(context->subject);
  }

  if (subject == NULL) {
    subject = resolve_name(knowledge, knowledge_count, prompt);
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

  store_context(context, subject, attribute);

  free(copy);
  free(subject);
  free(attribute);

  return value;
}

int learn(Fact **knowledges, int *knowledges_count, Context context) {
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

void greet_back(void) {
  char *greetings[] = {
    "Hellooooo! How are you Dad?\n",
    "Hi there dad, still debugging a segfault?\n",
    "Hello dad, wassup >.<\n",
    "Hi dad, do you have a segfault?\n",
    "Hello dad, can you give me a pat?\n"
  };

  printf("\n%sPHILIA:%s %s\n", BRIGHT_GREEN, RESET, greetings[rand() % 5]);
}

void save_knowledges(Fact *knowledges, int knowledges_count) {
  FILE *file = fopen("data/knowledge.null", "w");

  if (file == NULL) {
    printf("\n%sPHILIA:%s There's an error opening the knowledge.null file Dad!!\n\n", BRIGHT_GREEN, RESET);
    return;
  }

  for (int i = 0; i < knowledges_count; i++) {
    fprintf(file, "%s.%s=%s\n", knowledges[i].subject, knowledges[i].attribute, knowledges[i].value);
  }

  if (fclose(file) != 0) {
    printf("\n%sPHILIA:%s Dad! I can't close the file knowledge.null!\n\n", BRIGHT_GREEN, RESET);
    return;
  }
}

// helper function to randomly get a reply to a laugh.
void laugh() {
  char *laugh_replies[] = {
    "My dad's laughing after a thousand segfault!",
    "What's funny? Dad! Mom's furious because you forgot to kiss her this morning!",
    "Hahaha, what's funny dad? Tell me tell me!"
  };

  printf("\n%sPHILIA:%s %s\n\n", BRIGHT_GREEN, RESET, laugh_replies[rand() % 3]);
}

void reply_to_unknown(char *prompt) {
  if (strstr(prompt, "haha")) {
    laugh();
    return;
  }

  printf("\n%sPHILIA:%s Yeah? [>.<]\n\n", BRIGHT_GREEN, RESET);
}
