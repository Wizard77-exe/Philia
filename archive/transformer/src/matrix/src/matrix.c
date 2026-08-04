#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

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
  if (rows <= 0 || cols <= 0) {
    fprintf(stderr, "ERROR: Column or Row can't be zero or negative.\n");
    return MATRIX_ERROR;
  }
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

void matrix_move(Matrix *dest, Matrix *src) {
  if (dest == NULL || src == NULL) {
    fprintf(stderr, "ERROR: matrix_move() received a NULL pointer to a Matrix.\n");
    return;
  }

  if (MATRIX_OK(*dest)) {
    fprintf(stderr, "ERROR: matrix_move(). Destination owns allocated memory.");
    return;
  }

  *dest = *src;

  src->data = NULL;

  /* Source no longer owns the data. Reset it to an empty matrix. */
  matrix_free(src);
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

Matrix matrix_identity(int size) {
  if (size <= 0) {
    fprintf(stderr, "ERROR: matrix_identity() requires dimension greater than zero.\n");
    return MATRIX_ERROR;
  }
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

float matrix_variance(const Matrix m) {
  float mean = matrix_mean(m);
  float sum = 0.0f;

  for (int i = 0; i < m.rows * m.cols; i++) {
    sum += (m.data[i] - mean) * (m.data[i] - mean);
  }

  return sum / (m.cols * m.rows);
}

float matrix_min(const Matrix m) {
  float min = m.data[0];

  for (int i = 0; i < m.cols * m.rows; i++) {
    if (m.data[i] < min) 
      min = m.data[i];
  }

  return min;
}

unsigned int matrix_argmax(const Matrix m) {
  float max = matrix_max(m);

  for (int i = 0; i < m.rows * m.cols; i++) {
    if (m.data[i] == max)
      return i;
  }

  return -1;
}

unsigned int matrix_argmin(const Matrix m) {
  float min = matrix_min(m);

  for (int i = 0; i < m.rows * m.cols; i++) {
    if (m.data[i] == min) 
      return i;
  }

  return -1;
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

static float random_uniform(float min, float max) {
  float r = ((float)rand() + 1.0f) / ((float)RAND_MAX + 2.0f);
  return min + (max - min) * r;
}

void matrix_fill_uniform(Matrix *m, float min, float max) {
  for (int i = 0; i < m->cols * m->rows; i++) {
    m->data[i] = random_uniform(min, max);
  }
}

static float random_normal(void) {
  static bool has_cached = false;
  static float cached_normal;

  if (has_cached) {
    has_cached = false;
    return cached_normal;
  }

  float u1 = random_uniform(0.0f, 1.0f);
  float u2 = random_uniform(0.0f, 1.0f);

  float r = sqrtf(-2.0f * logf(u1));

  float z0 = r * cosf(2.0f * PI * u2);
  float z1 = r * sinf(2.0f * PI * u2);

  cached_normal = z1;
  has_cached = true;

  return z0;
}

void matrix_fill_normal(Matrix *m, float mean, float stddev) {
  for (int i = 0; i < m->cols * m->rows; i++) {
    m->data[i] = mean + stddev * random_normal();
  }
}

void matrix_fill_xavier_uniform(Matrix *m) {
  int fan_in = m->rows;
  int fan_out = m->cols;

  float limit = sqrtf(6.0f / (fan_in + fan_out));

  matrix_fill_uniform(m, -limit, limit);
}

void matrix_fill_xavier_normal(Matrix *m) {
  int fan_in = m->rows;
  int fan_out = m->cols;

  float sigma = sqrtf(2.0f / (fan_in + fan_out));

  matrix_fill_normal(m, 0.0f, sigma);
}

void matrix_fill_he_uniform(Matrix *m) {
  int fan_in = m->rows;

  float limit = sqrtf(6.0f / fan_in);

  matrix_fill_uniform(m, -limit, limit);
}

void matrix_fill_he_normal(Matrix *m) {
  int fan_in = m->rows;

  float sigma = sqrtf(2.0f / fan_in);

  matrix_fill_normal(m, 0.0f, sigma);
}

float matrix_dot(const Matrix a, const Matrix b) {
  if (a.rows * a.cols != b.rows * b.cols) {
    fprintf(stderr, "ERROR: Matrices don't have the same dimension and is not applicable for Matrix dot_product computation.\n");
    return NAN;
  }

  float sum = 0.0f;

  for (int i = 0; i < a.rows * a.cols; i++) {
    sum += a.data[i] * b.data[i];
  }

  return sum;
}

// NOTE: a and b should only be a vector. either Nx1 or 1xN. (our matrix is actually a linear one.)
Matrix matrix_outer(const Matrix a, const Matrix b) {
  bool a_vector = (a.rows == 1 || a.cols == 1);
  bool b_vector = (b.rows == 1 || b.cols == 1);

  if (!a_vector || !b_vector) {
    fprintf(stderr, "ERROR: matrix_outer requires vectors.\n");
    return MATRIX_ERROR;
  }
  int len_a = a.rows * a.cols;
  int len_b = b.rows * b.cols;

  Matrix c = matrix_create(len_a, len_b);
  if (!MATRIX_OK(c)) {
    fprintf(stderr, "ERROR: Matrix creation failure.\n");
    matrix_free(&c);
    return MATRIX_ERROR;
  }

  for (int i = 0; i < len_a; i++) {
    for (int j = 0; j < len_b; j++) {
      c.data[i * c.cols + j] = a.data[i] * b.data[j];
    }
  }

  return c;
}

// NOTE: x should be a vector, either Nx1 or 1xN.
float matrix_norm(const Matrix x) {
  if (!(x.cols == 1 || x.rows == 1)) {
    fprintf(stderr, "ERROR: matrix_norm() requires a vector.\n");
    return NAN;
  }

  return sqrtf(matrix_dot(x, x));
}

void matrix_normalize(Matrix *m) {
  float norm = matrix_norm(*m);

  if (isnan(norm) || norm  == 0) {
    fprintf(stderr, "ERROR: Can't normalize with NaN or 0 magnitude.\n");
    return;
  }

  for (int i = 0; i < m->rows * m->cols; i++) {
    m->data[i] /= norm;
  }
}

void matrix_clip(Matrix *m, float min, float max) {
  if (min > max) {
    fprintf(stderr,
        "ERROR: matrix_clip() min > max.\n");
    return;
  }

  int len = m->rows * m->cols;

  for (int i = 0; i < len; i++) {
    if (m->data[i] < min)
      m->data[i] = min;
    else if (m->data[i] > max)
      m->data[i] = max;
  }
}

bool matrix_equal(const Matrix a, const Matrix b, float epsilon) {
  if (a.rows != b.rows || a.cols != b.cols) return false;

  int len = a.rows * a.cols;

  for (int i = 0; i < len; i++) {
    float difference = fabsf(a.data[i] - b.data[i]);

    if (difference > epsilon)
      return false;
  }

  return true;
}

float matrix_trace(const Matrix a) {
  if (a.rows != a.cols) {
    fprintf(stderr, "ERROR: matrix_trace() requires a square matrix.\n");
    return NAN;
  }

  float trace = 0.0f;

  for (int i = 0; i < a.rows; i++) {
    trace += MAT_AT(a, i, i);
  }

  return trace;
}

bool matrix_is_square(const Matrix m) {
  return m.rows == m.cols;
}

bool matrix_is_identity(const Matrix m) {
  if (!matrix_is_square(m))
    return false;

  for (int row = 0; row < m.rows; row++) {
    for (int col = 0; col < m.cols; col++) {
      if (row != col && fabsf(MAT_AT(m, row, col) - 0.0f) > MATRIX_EPSILON)
        return false;

      if (row == col && fabsf(MAT_AT(m, row, col) - 1.0f) > MATRIX_EPSILON)
        return false;
    }
  }

  return true;
}

bool matrix_is_symmetric(const Matrix m) {
  if (!matrix_is_square(m))
    return false;

  for (int i = 0; i < m.rows; i++) {
    for (int j = 0; j < i; j++) {
      if (fabsf(MAT_AT(m, i, j) - MAT_AT(m, j, i)) > MATRIX_EPSILON)
        return false;
    }
  }

  return true;
}

void matrix_add_bias(Matrix *output, const Matrix bias) {
  if (output == NULL) {
    fprintf(stderr, "ERROR: Output is NULL.\n");
    return;
  }

  if (bias.rows != 1 || bias.cols != output->cols) {
    fprintf(stderr, "ERROR: bias shape mismatch.\n");
    return;
  }

  for (int row = 0; row < output->rows; row++) {
    for (int col = 0; col < output->cols; col++) {
      MAT_AT(*output, row, col) += MAT_AT(bias, 0, col);
    }
  }
}
