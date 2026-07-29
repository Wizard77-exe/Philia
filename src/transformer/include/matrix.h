#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>

#define PI 3.14159265358979323846f

#define MATRIX_ERROR ((Matrix){      \
    .data = NULL,                    \
    .rows = 0,                       \
    .cols = 0,                       \
    .success = false                 \
})

#define MATRIX_OK(m) ((m).success)

#define MAT_AT(m,r,c) ((m).data[(r)*(m).cols+(c)])

#define MATRIX_SIZE(m) ((m).rows * (m).cols);

#define MATRIX_SET(m, r, c, val) \
  ((m).data[(r) * (m).cols + (c)] = (val))

#define MATRIX_EQUAL_SHAPE(m, n) \
  ((m).rows == (n).rows && (m).cols == (n).cols)

typedef struct {
  float *data;

  int rows;
  int cols;

  bool success;
} Matrix;

void matrix_free(Matrix *matrix);

void matrix_pretty_print(const Matrix matrix);

Matrix matrix_create(int rows, int cols);

void matrix_fill(Matrix *m, float value);
void matrix_random(Matrix *m);

Matrix matrix_copy(const Matrix *src);
Matrix matrix_transpose(const Matrix m);
Matrix matrix_addition(const Matrix a, const Matrix b);
Matrix matrix_subtraction(const Matrix a, const Matrix b);
Matrix matrix_scalar(const Matrix *a, float scalar);
Matrix matrix_multiply(const Matrix *a, const Matrix *b);
Matrix matrix_identity(unsigned int size);

float matrix_sum(const Matrix m);
float matrix_mean(const Matrix m);
float matrix_variance(const Matrix m);
float matrix_max(const Matrix m);
float matrix_min(const Matrix m);

int matrix_argmax(const Matrix m);

Matrix matrix_apply(const Matrix m, float (*func)(float));
Matrix matrix_hadamard(const Matrix a, const Matrix b);

void matrix_fill_uniform(Matrix *m, float min, float max);
void matrix_fill_normal(Matrix *m, float mean, float stddev);
void matrix_fill_xavier_uniform(Matrix *m);
void matrix_fill_xavier_normal(Matrix *m);
void matrix_fill_he_uniform(Matrix *m);
void matrix_fill_he_normal(Matrix *m);

float matrix_dot(const Matrix a, const Matrix b);

Matrix matrix_outer(const Matrix a, const Matrix b);

float matrix_norm(const Matrix x);

void matrix_normalize(Matrix *m);

#endif
