#include "matrix.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OK 0
#define INCORRECT_MATRIX 1
#define CALCULATION_ERROR 2

int create_matrix(int rows, int columns, matrix_t *result) {
  if ((rows <= 0) || (columns <= 0)) return INCORRECT_MATRIX;
  result->rows = rows;
  result->columns = columns;
  double *matrix = malloc(sizeof(double) * rows * columns);
  if (!matrix) return INCORRECT_MATRIX;
  result->matrix = malloc(sizeof(double) * rows);
  if (!result->matrix) return INCORRECT_MATRIX;
  for (int i = 0; i < rows * columns; i++) matrix[i] = 0;
  for (int i = 0; i < rows; i++) result->matrix[i] = matrix + i * columns;
  return OK;
}

void remove_matrix(matrix_t *this) {
  if (this->matrix == NULL) return;
  if (this->matrix[0] == NULL) return;
  free(this->matrix[0]);
  free(this->matrix);
  this->matrix = NULL;
}

#define FAILURE 0
#define SUCCESS 1
int eq_matrix(matrix_t *A, matrix_t *B) {
  if (A->rows != B->rows) return FAILURE;
  if (A->columns != B->columns) return FAILURE;
  for (int i = 0; i < A->columns * B->rows; i++)
    if ((fabs((*A->matrix)[i] - (*B->matrix)[i])) > 1e-7) return FAILURE;
  return SUCCESS;
}
#undef FAILURE
#undef SUCCESS

int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if ((A->rows <= 0) || (A->columns <= 0)) return INCORRECT_MATRIX;
  if ((B->rows <= 0) || (B->columns <= 0)) return INCORRECT_MATRIX;
  if ((A->rows != B->rows) || (A->columns != B->columns))
    return CALCULATION_ERROR;
  (void)create_matrix(A->rows, B->columns, result);
  for (int i = 0; i < A->rows * B->columns; i++)
    (*result->matrix)[i] = (*A->matrix)[i] + (*B->matrix)[i];
  return OK;
}

int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if ((A->rows <= 0) || (A->columns <= 0)) return INCORRECT_MATRIX;
  if ((B->rows <= 0) || (B->columns <= 0)) return INCORRECT_MATRIX;
  if ((A->rows != B->rows) || (A->columns != B->columns))
    return CALCULATION_ERROR;
  create_matrix(A->rows, B->columns, result);
  for (int i = 0; i < A->rows * B->columns; i++)
    (*result->matrix)[i] = (*A->matrix)[i] - (*B->matrix)[i];
  return OK;
}

int mult_number(matrix_t *A, double number, matrix_t *result) {
  if ((A->rows <= 0) || (A->columns <= 0)) return INCORRECT_MATRIX;
  create_matrix(A->rows, A->columns, result);
  for (int i = 0; i < A->rows * A->columns; i++)
    (*result->matrix)[i] = (*A->matrix)[i] * number;
  return OK;
}

int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if ((A->rows <= 0) || (A->columns <= 0)) return INCORRECT_MATRIX;
  if ((B->rows <= 0) || (B->columns <= 0)) return INCORRECT_MATRIX;
  if ((A->columns != B->rows)) return CALCULATION_ERROR;
  create_matrix(A->rows, B->columns, result);
  matrix_t B_t = {0};
  matrix_t *B_T = &B_t;
  transpose(B, B_T);
  int line_size = A->columns;
  for (int i = 0; i < result->rows * result->columns; i++) {
    double _result = 0.0;
    double *line1 = A->matrix[i / B_T->rows],
           *line2 = B_T->matrix[i % B_T->rows];
    for (int j = 0; j < line_size; j++) _result += line1[j] * line2[j];
    result->matrix[0][i] = _result;
  }
  remove_matrix(B_T);
}

int transpose(matrix_t *A, matrix_t *result) {
  if ((A->rows <= 0) || (A->columns <= 0)) return INCORRECT_MATRIX;
  create_matrix(A->columns, A->rows, result);
  for (int i = 0; i < A->columns; i++)
    for (int j = 0; j < A->rows; j++) result->matrix[i][j] = A->matrix[j][i];
  return OK;
}

void matrix_minor(matrix_t *source, matrix_t *minor, int x, int y) {
  int xs = 0, ys = 0, xm = 0, ym = 0;
  for (; xs < source->rows; xs++, xm++) {
    if (xs == x)
      if (!(++xs < source->rows)) break;
    for (; ys < source->columns; ys++, ym++) {
      if (ys == y)
        if (!(++ys < source->columns)) break;
      minor->matrix[xm][ym] = source->matrix[xs][ys];
    }
    ys = 0, ym = 0;
  }
}

int calc_complements(matrix_t *A, matrix_t *result) {
  if ((A->rows <= 0) || (A->columns <= 0)) return INCORRECT_MATRIX;
  if (A->rows != A->columns) return CALCULATION_ERROR;
  create_matrix(A->rows, A->columns, result);
  if (A->rows == 1) {
    result->matrix[0][0] = A->matrix[0][0];
    return OK;
  }
  matrix_t it;
  create_matrix(A->rows - 1, A->columns - 1, &it);
  for (int32_t i = 0; i < A->rows; i++)
    for (int32_t j = 0; j < A->columns; j++) {
      matrix_minor(A, &it, i, j);
      determinant(&it, &(result->matrix[i][j]));
      result->matrix[i][j] *= (1 - (i + j) % 2 * 2);
    }
  remove_matrix(&it);
  return OK;
}

int determinant(matrix_t *A, double *result) {
  if ((A->rows <= 0) || (A->columns <= 0)) return INCORRECT_MATRIX;
  if (A->rows != A->columns) return CALCULATION_ERROR;
  if (A->rows == 1) {
    *result = A->matrix[0][0];
    return OK;
  }
  if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    return OK;
  }
  if (A->rows >= 3) {
    *result = 0.0;
    int8_t sign = +1;
    matrix_t it = {0};
    create_matrix(A->rows - 1, A->columns - 1, &it);
    for (int i = 0; i < A->columns; i++) {
      matrix_minor(A, &it, 0, i);
      double that = 0.0;
      determinant(&it, &that);
      *result += sign * A->matrix[0][i] * that;
      sign *= -1;
    }
    remove_matrix(&it);
  }
  return OK;
}

int inverse_matrix(matrix_t *A, matrix_t *result) {
  if ((A->rows <= 0) || (A->columns <= 0)) return INCORRECT_MATRIX;
  if (A->rows != A->columns) return CALCULATION_ERROR;
  double det;
  determinant(A, &det);
  if (det == 0.0) return CALCULATION_ERROR;
  matrix_t it = {0};
  calc_complements(A, result);
  transpose(result, &it);
  remove_matrix(result);
  mult_number(&it, -1.0, result);
  remove_matrix(&it);
  return OK;
}

#undef OK
#undef INCORRECT_MATRIX
#undef CALCULATION_ERROR