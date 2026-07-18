#ifndef FILES_H
#define FILES_H

#include "structures.h"

#define LINE_MAX 4096

// NOTE: Loads knowledges from the knowledge.null
Fact *load_knowledges(int *fact_count);

// NOTE: Loads subjects.null to char **subjects
char **load_subjects(int *subjects_count);

// NOTE: Loads attributes.null to char **attributes
char **load_attributes(int *attributes_count);

// NOTE: Loads dictionary.null to char **dictionary
char **load_dictionary(int *dictionary_count);

// NOTE: Loads synonyms.null to Synonyms[]
Synonym *load_synonyms(int *synonyms_count);

// NOTE: saving knowledges array (Fact[]) back to the knowledge.null.
void save_knowledges(Fact *knowledges, int knowledges_count);

#endif
