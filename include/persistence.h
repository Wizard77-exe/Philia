#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "structures.h"

int save_embeddings(
    const char *filename,
    const SkipGram *model
);

int load_embeddings(
    const char *filename,
    SkipGram *model
);

#endif
