#include <stdio.h>
#include <stdlib.h>

#include "persistence.h"

int save_embeddings(const char *filename, const SkipGram *model) {

    FILE *fp = fopen(filename, "wb");

    if (!fp) {
        perror("save_embeddings");
        return 0;
    }

    // header
    fwrite(&model->vocabulary_size, sizeof(int), 1, fp);
    fwrite(&model->embedding_dim, sizeof(int), 1, fp);

    //Input Embeddings

    for (int i = 0; i < model->vocabulary_size; i++) {
        fwrite(model->input.vectors[i].values, sizeof(float), model->embedding_dim, fp);
    }

    // Output Embeddings

    for (int i = 0; i < model->vocabulary_size; i++) {
        fwrite(model->output.vectors[i].values, sizeof(float), model->embedding_dim, fp);
    }

    fclose(fp);

    return 1;
}

int load_embeddings(const char *filename, SkipGram *model) {

    FILE *fp = fopen(filename, "rb");

    if (!fp) {
        perror("load_embeddings");
        return 0;
    }

    int vocabulary_size;
    int embedding_dim;

    // header

    fread(&vocabulary_size, sizeof(int), 1, fp);

    fread(&embedding_dim, sizeof(int), 1, fp);

    /* ---------------------------------
     * Verify dimensions
     * --------------------------------- */

    if (vocabulary_size != model->vocabulary_size || embedding_dim != model->embedding_dim) {
        printf("Embedding dimensions do not match.\n");

        fclose(fp);

        return 0;
    }

    /* ---------------------------------
     * Read Input Embeddings
     * --------------------------------- */

    for (int i = 0; i < model->vocabulary_size; i++) {
        fread(model->input.vectors[i].values, sizeof(float), model->embedding_dim, fp);
    }

    /* ---------------------------------
     * Read Output Embeddings
     * --------------------------------- */

    for (int i = 0; i < model->vocabulary_size; i++) {
        fread(model->output.vectors[i].values, sizeof(float), model->embedding_dim, fp);
    }

    fclose(fp);

    return 1;
}
