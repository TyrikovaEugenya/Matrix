#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

extern int create_matrix(int rows, int columns, matrix_t *result);
void remove_matrix(matrix_t *A);

int eq_matrix(matrix_t *A, matrix_t *B);

int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int mult_number(matrix_t *A, double number, matrix_t *result);
int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

int transpose(matrix_t *A, matrix_t *result);
int calc_complements(matrix_t *A, matrix_t *result);
int determinant(matrix_t *A, double *result);
int inverse_matrix(matrix_t *A, matrix_t *result);

#endif
