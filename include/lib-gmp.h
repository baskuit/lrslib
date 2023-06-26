#include <gmp.h>

#include "../src/lrsdriver.h"
#include "../src/lrslib.h"
#include "../src/lrsnashlib.h"

#ifndef LIBLRSNASHGMP
#define LIBLRSNASHGMP

#ifdef __cplusplus
extern "C"
{
#endif

    extern void init_game_gmp(game *g, int rows, int cols, int *row_num, int *row_den, int *col_num, int *col_den);

    extern void solve_gmp(game *g, lrs_mp_vector row_strategy, lrs_mp_vector col_strategy);

    extern int solve_gmp_2(game *g, int rows, int cols, mpq_t *row_payoff_data, mpq_t *col_payoff_data, mpz_t *row_data, mpz_t *col_data);

    extern lrs_mp_vector alloc_data_gmp(size_t size);

    extern void dealloc_data_gmp(lrs_mp_vector data, size_t size);

#ifdef __cplusplus
}
#endif

#endif // LIBLRSNASHGMP