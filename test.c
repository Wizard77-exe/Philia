#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlp.h"
#include "stdphilia.h"
#include "files.h"
#include "typo.h"

int main() {
  int subjects_count, attributes_count, knowledges_count, synonyms_count, indexed_facts_count;

  Fact *knowledges = load_knowledges(&knowledges_count);

  if (knowledges == NULL) {
    return 1;
  }

  IndexedFact *index_fact = build_indexed_fact(knowledges, knowledges_count, &indexed_facts_count);

  if (index_fact == NULL) {
    free_knowledges(knowledges, knowledges_count);
    return 1;
  }

  // compute_tf_index_fact; --> for (int i = 0; i < numbers of index_fact; i++) {commpute_tf(index_fact.tokens)}

  char **subjects = load_subjects(&subjects_count);

  if (subjects == NULL) {
    free_knowledges(knowledges, knowledges_count);
    free_indexed_fact(index_fact, indexed_facts_count);
    return 1;
  }

  char **attributes = load_attributes(&attributes_count);

  if (attributes == NULL) {
    free_knowledges(knowledges, knowledges_count);
    free_string_array(subjects, subjects_count);
    free_indexed_fact(index_fact, indexed_facts_count);
    return 1;
  }

  Synonym *synonyms = load_synonyms(&synonyms_count);

  if (synonyms == NULL) {
    free_knowledges(knowledges, knowledges_count);
    free_indexed_fact(index_fact, indexed_facts_count);
    free_string_array(subjects, subjects_count);
    free_string_array(attributes, attributes_count);
    return 1;
  }

  Dictionary d;
  d.words = load_dictionary(&d.count);

  if (d.words == NULL) {
    free_knowledges(knowledges, knowledges_count);
    free_indexed_fact(index_fact, indexed_facts_count);
    free_string_array(subjects, subjects_count);
    free_string_array(attributes, attributes_count);
    free_synonyms(synonyms, synonyms_count);
    return 1;
  }

  char prompt[LINE_MAX];

  while(1) {
    printf("%sHAPPY:%s ", BRIGHT_CYAN, RESET);
    
    fgets(prompt, sizeof(prompt), stdin);
    prompt[strcspn(prompt, "\n")] = '\0';
    printf("\n");

    if (strcmp(prompt, "exit") == 0) {
      printf("BYEEEE\n\n");
      break;
    }

    normalize(prompt);
    Tokens t = tokenize(prompt);
    canonicalize_tokens(&t, synonyms, synonyms_count);
    
    recover_typos(&t, &d);
    score_tokens(&t, subjects, subjects_count, attributes, attributes_count);

    Keywords k = extract_keywords(&t);
    
    Intent intent = detect_intent(&t);

    switch(intent) {
      case INTENT_QUERY:
        Query q = detect_query(&k, subjects, subjects_count, attributes, attributes_count);
        Fact *fact = search_knowledges(&q, knowledges, knowledges_count);

        if (fact != NULL) {
          generate_response(fact);
        } else {
          Fact new_fact = learn_fact(q);
          append_fact(&knowledges, &knowledges_count, new_fact);                  // make this function return a bool or int to check whether the realloc succeed or not. for safety.
          free_fact(new_fact);
          free_indexed_fact(index_fact, indexed_facts_count);
          index_fact = build_indexed_fact(knowledges, knowledges_count, &indexed_facts_count);
        }
        free_query(&q);
        break;
      case INTENT_UPDATE:
        printf("UPDATE\n\n");
        break;
      case INTENT_GREETING:
        printf("GREETING\n\n");
        break;
      case INTENT_THANKS:
        printf("THANKS\n\n");
        break;
      case INTENT_SORRY:
        printf("SORRY\n\n");
        break;
      case INTENT_GOODBYE:
        printf("GOODBYE\n\n");
        break;
    }

    free_keywords(&k);
    free_tokens(&t);
  }

  save_knowledges(knowledges, knowledges_count);
  free_synonyms(synonyms, synonyms_count);
  free_indexed_fact(index_fact, indexed_facts_count);
  free_knowledges(knowledges, knowledges_count);
  free_string_array(attributes, attributes_count);
  free_string_array(subjects, subjects_count);
  free_string_array(d.words, d.count);
  
  return 0;
}
