#include "../src/lrsdriver.h"
#include "../src/lrslib.h"
#include "../src/lrsnashlib.h"

#ifndef LIBLRSNASH128
#define LIBLRSNASH128

#ifdef __cplusplus
extern "C"
{
#endif

    extern void init_game(game *g, int rows, int cols, int *row_num, int *row_den, int *col_num, int *col_den);

    extern void solve(game *g, lrs_mp_vector row_strategy, lrs_mp_vector col_strategy);

    extern int lrs_solve_nash_(game *g, lrs_mp_vector row_strategy, lrs_mp_vector col_strategy);

    extern void prat_(const char *name, lrs_mp Nin, lrs_mp Din);

    extern lrs_mp_vector alloc_data(size_t size);

    extern void dealloc_data(lrs_mp_vector data, size_t size);

#ifdef __cplusplus
}
#endif

#endif // LIBLRSNASH128