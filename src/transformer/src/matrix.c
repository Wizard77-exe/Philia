#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "matrix.h"

void matrix_free(Matrix *matrix) {
  if (matrix == NULL)
    return;

  free(matrix->data);
  matrix->cols = 0;
  matrix->rows = 0;
  matrix->success = false;
}

void matrix_pretty_print(const Matrix matrix) {
  for (int i = 0; i < matrix.rows * matrix.cols; i++) {
    if (i % matrix.cols == 0)
      printf("\n");

    printf("%-2.6f ", matrix.data[i]);
  }
  printf("\n");
}

Matrix matrix_create(int rows, int cols) {
  Matrix matrix = {0};

  matrix.rows = rows;
  matrix.cols = cols;

  matrix.data = malloc(sizeof(float) * (matrix.rows * matrix.cols));

  if (matrix.data == NULL) {
    fprintf(stderr, "allocating memory for matrix.data.\n");
    matrix_free(&matrix);
    return MATRIX_ERROR;
  }

  matrix.success = true;
  
  return matrix;
}

void matrix_fill(Matrix *m, float value) {
  for (int i = 0; i < (m->cols * m->rows); i++) {
    m->data[i] = value;
  }
}

void matrix_random(Matrix *m) {
  for (int i = 0; i < (m->rows * m->cols); i++) {
    float random_val = (-0.1f) + (((float)rand() / (float)RAND_MAX) * (0.1 - (-0.1f)));
    m->data[i] = random_val;
  }
}

Matrix matrix_copy(const Matrix *src) {
  Matrix copy = matrix_create(src->rows, src->cols);
  if (!MATRIX_OK(copy)) {
    fprintf(stderr, "copying matrix.\n");
    matrix_free(&copy);
    return MATRIX_ERROR;
  }

  for (int i = 0; i < (src->rows * src->cols); i++) {
    copy.data[i] = src->data[i];
  }

  return copy;
}

Matrix matrix_transpose(const Matrix m) {
  Matrix m_t = matrix_create(m.cols, m.rows);
  if (!MATRIX_OK(m_t)) {
    fprintf(stderr, "creating matrix\n");
    matrix_free(&m_t);
    return MATRIX_ERROR;
  }

  for (int idx = 0; idx < m.rows * m.cols; idx++) {
    int row = idx / m.cols;
    int col = idx % m.cols;

    m_t.data[col * m.rows + row] = m.data[idx];
  }

  return m_t;
}

Matrix matrix_addition(const Matrix a, const Matrix b) {
  if (!MATRIX_EQUAL_SHAPE(a, b)) {
    fprintf(stderr, "ERROR: Matrices are not of equal dimensions and not applicable for matrix addition.\n");
    return MATRIX_ERROR;
  }

  Matrix c = matrix_create(a.rows, a.cols);
  if (!MATRIX_OK(c)) {
    fprintf(stderr, "ERROR: Matrix creation failure.\n");
    matrix_free(&c);
    return MATRIX_ERROR;
  }

  for (int i = 0; i < a.rows * a.cols; i++) {
    c.data[i] = a.data[i] + b.data[i];
  }

  return c;
}

Matrix matrix_subtraction(const Matrix a, const Matrix b) {
  if (!MATRIX_EQUAL_SHAPE(a, b)) {
    fprintf(stderr, "ERROR: Matrices are not of equal dimensions and not applicable for matrix subtraction.\n");
    return MATRIX_ERROR;
  }

  Matrix c = matrix_create(a.rows, a.cols);
  if (!MATRIX_OK(c)) {
    fprintf(stderr, "ERROR: Matrix creation failure.\n");
    matrix_free(&c);
    return MATRIX_ERROR;
  }

  for (int i = 0; i < a.rows * a.cols; i++) {
    c.data[i] = a.data[i] - b.data[i];
  }

  return c;
}

Matrix matrix_scalar(const Matrix *a, float scalar) {
  Matrix b = matrix_create(a->rows, a->cols);
  if (!MATRIX_OK(b)) {
    fprintf(stderr, "ERROR: Matrix creation failure.\n");
    matrix_free(&b);
    return MATRIX_ERROR;
  }

  for (int i = 0; i < a->rows * a->cols; i++) {
    b.data[i] = a->data[i] * scalar;
  }

  return b;
}

Matrix matrix_multiply(const Matrix *a, const Matrix *b) {
  if (a->cols != b->rows) {
    fprintf(stderr, "ERROR: Matrices did not pass dimension rules check for matrix to matrix multiplication.\n");
    return MATRIX_ERROR;
  }

  Matrix c = matrix_create(a->rows, b->cols);
  if (!MATRIX_OK(c)) {
    fprintf(stderr, "ERROR: Matrix creation failure.\n");
    matrix_free(&c);
    return MATRIX_ERROR;
  }

  matrix_fill(&c, 0.0f);

  for (int row = 0; row < a->rows; row++) {
    for (int col = 0; col < b->cols; col++) {
      for (int k = 0; k < a->cols; k++) {
        //MATRIX_SET(c, row, col, MAT_AT(*a, row, k) * MAT_AT(*b, k, col));
        c.data[row * c.cols + col] += MAT_AT(*a, row, k) * MAT_AT(*b, k, col);
      }
    }
  }

  return c;
}

Matrix matrix_identity(unsigned int size) {
  Matrix I = matrix_create(size, size);
  if (!MATRIX_OK(I)) {
    fprintf(stderr, "ERROR: Matrix creation failure.\n");
    matrix_free(&I);
    return MATRIX_ERROR;
  }

  for (int idx = 0; idx < size * size; idx++) {
    int row = idx / size;
    int col = idx % size;
    
    MATRIX_SET(I, row, col, row == col ? 1.0f : 0.0f);
  }

  return I;
}

float matrix_sum(const Matrix m) {
  float sum = 0.0f;
  for (int i = 0; i < m.rows * m.cols; i++) {
    sum += m.data[i];
  }

  return sum;
}

float matrix_mean(const Matrix m) {
  return matrix_sum(m) / (m.rows * m.cols);
}

float matrix_max(const Matrix m) {
  float max = m.data[0];

  for (int i = 0; i < m.cols * m.rows; i++) {
    if (m.data[i] > max)
      max = m.data[i];
  }

  return max;
}

float matrix_min(const Matrix m) {
  float min = m.data[0];

  for (int i = 0; i < m.cols * m.rows; i++) {
    if (m.data[i] < min) 
      min = m.data[i];
  }

  return min;
}

int matrix_argmax(const Matrix m) {
  int idx = 0;
  float max = m.data[0];

  for (int i = 0; i < m.rows * m.cols; i++) {
    if (m.data[i] > max) {
      max = m.data[i];
      idx = i;
    }
  }

  return idx;
}

Matrix matrix_apply(const Matrix m, float (*func)(float)) {
  Matrix output = matrix_create(m.rows, m.cols);
  if (!MATRIX_OK(output)) {
    fprintf(stderr, "ERROR: Matrix creation failure.\n");
    matrix_free(&output);
    return MATRIX_ERROR;
  }

  for (int i = 0; i < output.rows * output.cols; i++) {
    output.data[i] = func(m.data[i]);
  }

  return output;
}

Matrix matrix_hadamard(const Matrix a, const Matrix b) {
  if (!MATRIX_EQUAL_SHAPE(a, b)) {
    fprintf(stderr, "ERROR: Matrices are not of equal dimensions and not applicable for hadamard multiplication.\n");
    return MATRIX_ERROR;
  }
  Matrix c = matrix_create(a.rows, a.cols);

  if (!MATRIX_OK(c)) {
    fprintf(stderr, "ERROR: Matrix creation failure.\n");
    matrix_free(&c);
    return MATRIX_ERROR;
  }

  for (int i = 0; i < c.rows * c.cols; i++) {
    c.data[i] = a.data[i] * b.data[i];
  }

  return c;
}
