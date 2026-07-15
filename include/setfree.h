#ifndef SETFREE_H
#define SETFREE_H

#include "structures.h"

// NOTE: freeing an array of string (char s[][])
void free_string_array(char **s, int count);

// NOTE: freeing fact (Fact)
void free_fact(Fact f);

// NOTE: freeing knowledges (Fact[])
void free_knowledges(Fact *knowledges, int knowledges_count);

// NOTE: freeing tokens (Tokens);
void free_tokens(Tokens *t);

// NOTE: freeing synonyms array (Synonym[])
void free_synonym(Synonym s);
void free_synonyms(Synonym *s, int count);

// NOTE: freeing DocumentTerms
void free_DT(DocumentTerms *dt);

// NOTE: freeing IndexedFact arrays (IndexedFact[])
void free_indexed_fact(IndexedFact *f, int indexedfacts_count);

// NOTE: freeing Vocabulary
void free_vocabulary(Vocabulary *v);

// NOTE: freeing Query
void free_query(Query *q);

#endif
