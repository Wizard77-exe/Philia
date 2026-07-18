#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "files.h"
#include "structures.h"
#include "colors.h"
#include "setfree.h"

// NOTE: counts number of lines in a file
static int count_lines(FILE *file) {
  int count = 0;

  if (!file) {
    return -1;
  }

  char buffer[LINE_MAX];

  while(fgets(buffer, sizeof(buffer), file)) {
    count++;
  }
  
  rewind(file);

  return count;
}

// NOTE: loads knowledges from the knowledge.null
Fact *load_knowledges(int *fact_count) {
  FILE *file = fopen("data/knowledge.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDD!!! HELPPP!! I can't open knowledge.null!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    return NULL;
  }

  int line_count = count_lines(file);
  int counter = 0;

  // allocating memories for the knowledges array;
  Fact *knowledges = malloc(sizeof(Fact) * line_count);

  if (knowledges == NULL) {
    printf("%sPHILIA:%s HEY DAD!!! I failed allocating memory for knowledges array, I'll exit and you can restart me again, okay? Sorry, Love you!!\n\n", BRIGHT_GREEN, RESET);
    fclose(file);
    *fact_count = 0;
    return NULL;
  }

  char line[LINE_MAX];
  char *token;

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    knowledges[counter] = (Fact){0};

    //getting the subject part.
    token = strtok(line, ".");
    //assigning the token to Fact.subject.
    knowledges[counter].subject = strdup(token);
    if (knowledges[counter].subject == NULL) {
      free_fact(knowledges[counter]);
      free_knowledges(knowledges, counter);
      fclose(file);
      *fact_count = 0;
      return NULL;
    }

    // getting the attribute part.
    token = strtok(NULL, "=");
    //assigning the token to Fact.attribute.
    knowledges[counter].attribute = strdup(token);
    if (knowledges[counter].attribute == NULL) {
      free_fact(knowledges[counter]);
      free_knowledges(knowledges, counter);
      fclose(file);
      *fact_count = 0;
      return NULL;
    }

    // getting the value part.
    token = strtok(NULL, "");
    // assigning the token to Fact.value.
    knowledges[counter].value = strdup(token);
    if (knowledges[counter].value == NULL) {
      free_fact(knowledges[counter]);
      free_knowledges(knowledges, counter);
      fclose(file);
      *fact_count = 0;
      return NULL;
    }

    // incrementing the counter;
    counter++;
  }

  *fact_count = counter;

  // closing the file.
  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DADDD!!! I can't close the knowledge.null!!! HELPPPPP MEEEE!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
  }

  return knowledges;
}

// NOTE: loading subjects.null to char **subjects
char **load_subjects(int *subjects_count) {
  FILE *file = fopen("data/subjects.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDDD!!! I can't open subjects.nulllll!!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    *subjects_count = 0;
    return NULL;
  }

  int n = count_lines(file);
  int counter = 0;

  char **s = malloc(sizeof(char *) * n);
  if (s == NULL) {
    printf("%sPHILIA:%s DADDDDD!!! AHHHHH, I can't allocate memories for the subjects!!\n\n", BRIGHT_GREEN, RESET);
    fclose(file);
    *subjects_count = 0;
    return NULL;
  }

  char line[64];

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    
    s[counter] = NULL;
    s[counter] = strdup(line);

    if (s[counter] == NULL) {
      free_string_array(s, counter);
      fclose(file);
      *subjects_count = 0;
      return NULL;
    }

    counter++;
  }

  *subjects_count = counter;

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DADDDD!! I can't close ittt!! The subjects.null file!! UGGHHHH!!!\n\n", BRIGHT_GREEN, RESET);
  }

  return s;
}

// NOTE: loading attributes.null to char attributes[][].
char **load_attributes(int *attributes_count) {
  FILE *file = fopen("data/attributes.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDD!!! I can't open the attributes.nullllll!!!!\n\n", BRIGHT_GREEN, RESET);
    *attributes_count = 0;
    return NULL;
  }

  int n = count_lines(file);
  int counter = 0;

  char **a = malloc(sizeof(char *) * n);

  if (a == NULL) {
    printf("%sPHILIA:%s DADDDD!!! I can't allocate memories for the attributes!!!\n\n", BRIGHT_GREEN, RESET);
    fclose(file);
    *attributes_count;
    return NULL;
  }

  char line[64];

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    a[counter] = NULL;
    a[counter] = strdup(line);

    if (a[counter] == NULL) {
      free_string_array(a, counter);
      fclose(file);
      *attributes_count = 0;
      return NULL;
    }

    counter++;
  }

  *attributes_count = counter;

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DADDD!!! UGGGGHHH!!! I can't close the attributes.null file!!!\n\n", BRIGHT_GREEN, RESET);
  }

  return a;
}

char **load_dictionary(int *dictionary_count) {
  FILE *file = fopen("data/dictionary.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s Dad? I can't seem to open the dictionary.null, sorry, hehe.\n\n", BRIGHT_GREEN, RESET);
    *dictionary_count = 0;
    return NULL;
  }

  int n = count_lines(file);
  int counter = 0;

  char **s = malloc(sizeof(char *) * n);

  if (s == NULL) {
    printf("%sPHILIA:%s DADDD! HUHU, I can't allocate memory for the words in the load_dictionary() function!!\n\n", BRIGHT_GREEN, RESET);
    *dictionary_count = 0;
    fclose(file);
    return NULL;
  }

  char line[64];

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    s[counter] = NULL;
    s[counter] = strdup(line);

    if (s[counter] == NULL) {
      free_string_array(s, counter);
      fclose(file);
      *dictionary_count = 0;
      return NULL;
    }

    counter++;
  }

  *dictionary_count = counter;

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DADDD!!! I can't close it, help me!! The dictionary.nullll!!!\n\n", BRIGHT_GREEN, RESET);
  }

  return s;
}

// NOTE: loading synonyms.null to Synonyms[]
Synonym *load_synonyms(int *synonyms_count) {
  FILE *file = fopen("data/synonyms.null", "r");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDDD!!! I can't open the synonyms fileeeee!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    return NULL;
  }

  int n = count_lines(file);
  int counter = 0;

  Synonym *synonyms = calloc(n, sizeof(Synonym));

  if (synonyms == NULL) {
    printf("%sPHILIA:%s Hehe, hey dad, looks like I wasn't able to allocate memory for the synonyms here, hehe. ['.']\n\n", BRIGHT_GREEN, RESET);
    fclose(file);
    *synonyms_count = 0;
    return NULL;
  }

  char line[128];
  char *token;

  while(fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    token = strtok(line, "=");
    synonyms[counter].synonym = strdup(token);

    if (synonyms[counter].synonym == NULL) {
      free_synonym(synonyms[counter]);
      free_synonyms(synonyms, counter);
      fclose(file);
      *synonyms_count = 0;
      return NULL;
    }

    token = strtok(NULL, "");
    synonyms[counter].canonical = strdup(token);

    if (synonyms[counter].canonical == NULL) {
      free_synonym(synonyms[counter]);
      free_synonyms(synonyms, counter);
      fclose(file);
      *synonyms_count = 0;
      return NULL;
    }

    counter++;
  }

  *synonyms_count = counter;

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DAD? Don't get mad, okay? But I can't close synonyms.null, hehe.\n\n", BRIGHT_GREEN, RESET);
  }

  return synonyms;
}

// NOTE: saving knowledges array (Fact[]) back to the knowledge.null
void save_knowledges(Fact *knowledges, int knowledges_count) {
  FILE *file = fopen("data/knowlede.null", "w");

  if (file == NULL) {
    printf("%sPHILIA:%s DADDD!!! I can't open the knowledge.null!!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
    return;
  }

  for (int i = 0; i < knowledges_count; i++) {
    fprintf(file, "%s.%s=%s\n", knowledges[i].subject, knowledges[i].attribute, knowledges[i].value);
  }

  if (fclose(file) != 0) {
    printf("%sPHILIA:%s DADDD!!! Help MEEEE!!! I can't close knowledge.nullllll!!! (%s)\n\n", BRIGHT_GREEN, RESET, strerror(errno));
  }

  return;
}
