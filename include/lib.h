#include <gmp.h>

#ifndef LIBLRSNASHGMP
#define LIBLRSNASHGMP

#ifdef __cplusplus
extern "C"
{
#endif

    extern void solve_gmp(
        int rows, int cols,
        mpq_t *row_payoff_data, mpq_t *col_payoff_data,
        mpz_t *row_solution_data, mpz_t *col_solution_data);

    extern void solve_gmp_pointer(
        int rows, int cols,
        mpq_t **row_payoff_data, mpq_t **col_payoff_data,
        mpz_t *row_solution_data, mpz_t *col_solution_data);

#ifdef __cplusplus
}
#endif

#endif // LIBLRSNASHGMP