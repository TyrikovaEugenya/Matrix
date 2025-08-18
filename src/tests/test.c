#include <check.h>

#include "../s21_matrix.h"

#define OK 0
#define INCORRECT_MATRIX 1
#define CALCULATION_ERROR 2

START_TEST(tc_create_1) {
  int res = s21_create_matrix(-1, -1, NULL);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
}
END_TEST

START_TEST(tc_create_2) {
  int res = s21_create_matrix(0, 0, NULL);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
}
END_TEST

START_TEST(tc_create_3) {
  matrix_t it;
  int res = s21_create_matrix(1, 1, &it);
  ck_assert_int_eq(res, OK);
  s21_remove_matrix(&it);
}
END_TEST

START_TEST(tc_create_4) {
  matrix_t it;
  int res = s21_create_matrix(10, 10, &it);
  ck_assert_int_eq(res, OK);
  s21_remove_matrix(&it);
}
END_TEST

START_TEST(tc_eq_1) {
  matrix_t it, that;
  s21_create_matrix(4, 4, &it);
  s21_create_matrix(4, 4, &that);
  int res = s21_eq_matrix(&it, &that);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_eq_2) {
  matrix_t it, that;
  s21_create_matrix(4, 4, &it);
  s21_create_matrix(4, 4, &that);
  *(it.matrix)[0] = 1e-8;
  int res = s21_eq_matrix(&it, &that);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_eq_3) {
  matrix_t it, that;
  s21_create_matrix(4, 4, &it);
  s21_create_matrix(4, 4, &that);
  (*it.matrix)[9] = 1e-8;
  int res = s21_eq_matrix(&it, &that);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_eq_4) {
  matrix_t it, that;
  s21_create_matrix(4, 4, &it);
  s21_create_matrix(4, 4, &that);
  (*it.matrix)[9] = 1e-5;
  int res = s21_eq_matrix(&it, &that);
  ck_assert_int_eq(res, 0);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_add_1) {
  matrix_t it, that, sum, expected;
  s21_create_matrix(3, 3, &it);
  s21_create_matrix(3, 3, &that);
  s21_create_matrix(3, 3, &expected);
  double src1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double src2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  for (int i = 0; i < 9; i++)
    (*it.matrix)[i] = src1[i], (*that.matrix)[i] = src2[i],
    (*expected.matrix)[i] = 10;
  s21_sum_matrix(&it, &that, &sum);
  int res = s21_eq_matrix(&sum, &expected);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
  s21_remove_matrix(&sum);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(tc_add_2) {
  matrix_t it, that, sum, expected;
  s21_create_matrix(3, 3, &it);
  s21_create_matrix(3, 3, &that);
  s21_create_matrix(3, 3, &expected);
  double src1[] = {0.1, 0.4, 0.1, -1.1, -1.5, 1.02, 4.4, 6.1, -11.2};
  double src2[] = {1.1, -1.1, 6.5, 7.55, 4.2, 7.1, -10.04, 16, 653.3};
  for (int i = 0; i < 9; i++)
    (*it.matrix)[i] = src1[i], (*that.matrix)[i] = src2[i],
    (*expected.matrix)[i] = src1[i] + src2[i];
  s21_sum_matrix(&it, &that, &sum);
  int res = s21_eq_matrix(&sum, &expected);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
  s21_remove_matrix(&sum);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(tc_add_3) {
  matrix_t it = {.rows = 2, .columns = 2}, that = {.rows = 2, .columns = 3};
  int res = s21_sum_matrix(&it, &that, NULL);
  ck_assert_int_eq(res, CALCULATION_ERROR);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_add_4) {
  matrix_t it = {.rows = -2, .columns = 2}, that = {.rows = 2, .columns = 2};
  int res = s21_sum_matrix(&it, &that, NULL);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_add_5) {
  matrix_t it = {.rows = 2, .columns = 2}, that = {.rows = 2, .columns = -2};
  int res = s21_sum_matrix(&it, &that, NULL);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_sub_1) {
  matrix_t it, that, sum, expected;
  s21_create_matrix(3, 3, &it);
  s21_create_matrix(3, 3, &that);
  s21_create_matrix(3, 3, &expected);
  double src1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double src2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  for (int i = 0; i < 9; i++)
    (*it.matrix)[i] = src1[i], (*that.matrix)[i] = src2[i],
    (*expected.matrix)[i] = src1[i] - src2[i];
  s21_sub_matrix(&it, &that, &sum);
  int res = s21_eq_matrix(&sum, &expected);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
  s21_remove_matrix(&sum);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(tc_sub_2) {
  matrix_t it, that, sum, expected;
  s21_create_matrix(3, 3, &it);
  s21_create_matrix(3, 3, &that);
  s21_create_matrix(3, 3, &expected);
  double src1[] = {0.1, 0.4, 0.1, -1.1, -1.5, 1.02, 4.4, 6.1, -11.2};
  double src2[] = {1.1, -1.1, 6.5, 7.55, 4.2, 7.1, -10.04, 16, 653.3};
  for (int i = 0; i < 9; i++)
    (*it.matrix)[i] = src1[i], (*that.matrix)[i] = src2[i],
    (*expected.matrix)[i] = src1[i] - src2[i];
  s21_sub_matrix(&it, &that, &sum);
  int res = s21_eq_matrix(&sum, &expected);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
  s21_remove_matrix(&sum);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(tc_sub_3) {
  matrix_t it = {.rows = 2, .columns = 2}, that = {.rows = 2, .columns = 3};
  int res = s21_sub_matrix(&it, &that, NULL);
  ck_assert_int_eq(res, CALCULATION_ERROR);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_sub_4) {
  matrix_t it = {.rows = -2, .columns = 2}, that = {.rows = 2, .columns = 2};
  int res = s21_sub_matrix(&it, &that, NULL);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_sub_5) {
  matrix_t it = {.rows = 2, .columns = 2}, that = {.rows = 2, .columns = -2};
  int res = s21_sub_matrix(&it, &that, NULL);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
}
END_TEST

START_TEST(tc_mult_number_1) {
  double src[] = {1, 2, -2, -1};
  matrix_t it, that, expected;
  s21_create_matrix(2, 2, &it);
  s21_create_matrix(2, 2, &expected);
  for (int i = 0; i < 4; i++)
    (*it.matrix)[i] = src[i], (*expected.matrix)[i] = src[i] * -2.0;
  s21_mult_number(&it, -2, &that);
  int res = s21_eq_matrix(&that, &expected);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(tc_mult_matrix_1) {
  matrix_t it, that, sum, expected;
  s21_create_matrix(3, 2, &it);
  s21_create_matrix(2, 3, &that);
  s21_create_matrix(3, 3, &expected);
  double src1[] = {1, 4, 2, 5, 3, 6};
  double src2[] = {1, -1, 1, 2, 3, 4};
  double src3[] = {9, 11, 17, 12, 13, 22, 15, 15, 27};
  for (int i = 0; i < 6; i++)
    (*it.matrix)[i] = src1[i], (*that.matrix)[i] = src2[i];
  for (int i = 0; i < 9; i++) (*expected.matrix)[i] = src3[i];
  s21_mult_matrix(&it, &that, &sum);
  int res = s21_eq_matrix(&sum, &expected);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
  s21_remove_matrix(&sum);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(tc_mult_matrix_2) {
  matrix_t it = {.rows = 3, .columns = 2};
  matrix_t that = {.rows = 3, .columns = 3};
  int res = s21_mult_matrix(&it, &that, NULL);
  ck_assert_int_eq(res, CALCULATION_ERROR);
}
END_TEST

START_TEST(tc_mult_matrix_3) {
  matrix_t it = {.rows = 3, .columns = 2};
  matrix_t that = {.rows = 4, .columns = 3};
  int res = s21_mult_matrix(&it, &that, NULL);
  ck_assert_int_eq(res, CALCULATION_ERROR);
}
END_TEST

START_TEST(tc_mult_matrix_4) {
  matrix_t it = {.rows = -1, .columns = 2};
  matrix_t that = {.rows = 3, .columns = -1};
  int res = s21_mult_matrix(&it, &that, NULL);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
}
END_TEST

START_TEST(tc_cc_1) {
  matrix_t it = {.rows = -1, .columns = 2};
  int res = s21_calc_complements(&it, NULL);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
}
END_TEST

START_TEST(tc_cc_2) {
  matrix_t it = {.rows = 3, .columns = 2};
  int res = s21_calc_complements(&it, NULL);
  ck_assert_int_eq(res, CALCULATION_ERROR);
}
END_TEST

START_TEST(tc_cc_3) {
  matrix_t it, that, expected;
  s21_create_matrix(3, 3, &it);
  s21_create_matrix(3, 3, &expected);
  double src1[] = {1, 2, 3, 0, 4, 2, 5, 2, 1};
  double src2[] = {0, 10, -20, 4, -14, 8, -8, -2, 4};
  for (int i = 0; i < 9; i++) (*it.matrix)[i] = src1[i];
  for (int i = 0; i < 9; i++) (*expected.matrix)[i] = src2[i];
  s21_calc_complements(&it, &that);
  int res = s21_eq_matrix(&that, &expected);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(tc_cc_4) {
  matrix_t it, that, expected;
  s21_create_matrix(1, 1, &it);
  s21_create_matrix(1, 1, &expected);
  it.matrix[0][0] = 10.04;
  expected.matrix[0][0] = 10.04;
  s21_calc_complements(&it, &that);
  int res = s21_eq_matrix(&that, &expected);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(tc_dt_1) {
  matrix_t it;
  s21_create_matrix(1, 1, &it);
  it.matrix[0][0] = 10.04;
  double res;
  s21_determinant(&it, &res);
  ck_assert_double_eq(res, 10.04);
  s21_remove_matrix(&it);
}
END_TEST

START_TEST(tc_dt_2) {
  matrix_t it;
  s21_create_matrix(2, 2, &it);
  double src[] = {3, 2, 4, 5};
  for (int i = 0; i < 4; i++) (*it.matrix)[i] = src[i];
  double res;
  s21_determinant(&it, &res);
  ck_assert_double_eq(res, 7.0);
  s21_remove_matrix(&it);
}
END_TEST

START_TEST(tc_dt_3) {
  matrix_t it;
  s21_create_matrix(3, 3, &it);
  double src[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (int i = 0; i < 9; i++) (*it.matrix)[i] = src[i];
  double res;
  s21_determinant(&it, &res);
  ck_assert_double_eq(res, 0.0);
  s21_remove_matrix(&it);
}
END_TEST

START_TEST(tc_dt_4) {
  matrix_t it;
  s21_create_matrix(4, 4, &it);
  double src[] = {4, 6, 7, 2, 1, 8, 6, 4, 7, 5, 8, 6, 4, 7, 9, 5};
  for (int i = 0; i < 16; i++) (*it.matrix)[i] = src[i];
  double res;
  s21_determinant(&it, &res);
  ck_assert_double_eq(res, -233.0);
  s21_remove_matrix(&it);
}
END_TEST

START_TEST(tc_dt_5) {
  matrix_t it;
  s21_create_matrix(5, 5, &it);
  double src[] = {7, 4, 5, 7, 6, 1, 4, 5, 7, 2, 1, 8, 2,
                  7, 6, 7, 5, 4, 6, 2, 4, 8, 7, 3, 4};
  for (int i = 0; i < 25; i++) (*it.matrix)[i] = src[i];
  double res;
  s21_determinant(&it, &res);
  ck_assert_double_eq(res, 7824.0);
  s21_remove_matrix(&it);
}
END_TEST

START_TEST(tc_inverse_1) {
  double src1[] = {2, 5, 7, 6, 3, 4, 5, -2, -3};
  double src2[] = {1, -1, 1, -38, 41, -34, 27, -29, 24};
  matrix_t it, that, expected;
  s21_create_matrix(3, 3, &it);
  s21_create_matrix(3, 3, &expected);
  for (int i = 0; i < 9; i++)
    (*it.matrix)[i] = src1[i], (*expected.matrix)[i] = src2[i];
  s21_inverse_matrix(&it, &that);
  int res = s21_eq_matrix(&that, &expected);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&it);
  s21_remove_matrix(&that);
  s21_remove_matrix(&expected);
}
END_TEST

#undef OK
#undef INCORRECT_MATRIX
#undef CALCULATION_ERROR

int main() {
  Suite *suite = suite_create("s21_matrix");
  TCase *tc_core = tcase_create("Core");

  const TTest *create[] = {tc_create_1, tc_create_2, tc_create_3, tc_create_4};
  const TTest *eq[] = {tc_eq_1, tc_eq_2, tc_eq_3, tc_eq_4};
  const TTest *add[] = {tc_add_1, tc_add_2, tc_add_3, tc_add_4, tc_add_5};
  const TTest *sub[] = {tc_sub_1, tc_sub_2, tc_sub_3, tc_sub_4, tc_sub_5};
  const TTest *mult_number[] = {tc_mult_number_1};
  const TTest *mult_matrix[] = {tc_mult_matrix_1, tc_mult_matrix_2,
                                tc_mult_matrix_3, tc_mult_matrix_4};
  const TTest *calc_complements[] = {tc_cc_1, tc_cc_2, tc_cc_3, tc_cc_4};
  const TTest *determinant[] = {tc_dt_1, tc_dt_2, tc_dt_3, tc_dt_4, tc_dt_5};
  const TTest *inverse[] = {tc_inverse_1};

  for (int i = 0; i < sizeof(create) / sizeof(TTest *); i++)
    tcase_add_test(tc_core, create[i]);
  for (int i = 0; i < sizeof(eq) / sizeof(TTest *); i++)
    tcase_add_test(tc_core, eq[i]);
  for (int i = 0; i < sizeof(add) / sizeof(TTest *); i++)
    tcase_add_test(tc_core, add[i]);
  for (int i = 0; i < sizeof(sub) / sizeof(TTest *); i++)
    tcase_add_test(tc_core, sub[i]);
  for (int i = 0; i < sizeof(mult_number) / sizeof(TTest *); i++)
    tcase_add_test(tc_core, mult_number[i]);
  for (int i = 0; i < sizeof(mult_matrix) / sizeof(TTest *); i++)
    tcase_add_test(tc_core, mult_matrix[i]);
  for (int i = 0; i < sizeof(calc_complements) / sizeof(TTest *); i++)
    tcase_add_test(tc_core, calc_complements[i]);
  for (int i = 0; i < sizeof(determinant) / sizeof(TTest *); i++)
    tcase_add_test(tc_core, determinant[i]);
  for (int i = 0; i < sizeof(inverse) / sizeof(TTest *); i++)
    tcase_add_test(tc_core, inverse[i]);

  suite_add_tcase(suite, tc_core);
  SRunner *sr = srunner_create(suite);
  srunner_run_all(sr, CK_NORMAL | CK_NOFORK);
  srunner_free(sr);

  return 0;
}
