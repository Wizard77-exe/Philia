#ifndef TOKENS_H
#define TOKENS_H

#include "structures.h"

#define ERROR_TOKEN       ((Tokens) { \
                                .tokens = NULL,       \
                                .count = 0,           \
                                .capacity = 0,        \
                                .success = false      \
                          })

Tokens tokenize(char *input);
// Tokens tokenize_file(const char *filename);

#endif
