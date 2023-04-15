#include "lrsdriver.h"
#include "lrslib.h"
#include "lrsnashlib.h"

#ifndef LIBLRSNASH
#define LIBLRSNASH

#ifdef __cplusplus
extern "C"
{
#endif

    extern void init_game(game *g, int rows, int cols, int *row_num, int *row_den, int *col_num, int *col_den);

    extern void solve(game *g, int *row_strategy, int *col_strategy);

    extern int lrs_solve_nash_(game *g, int *row_strategy, int *col_strategy);

    extern long lrs_nashoutput_(lrs_dat *Q, lrs_mp_vector output, long player, int *strategy);

    extern void prat_(const char *name, lrs_mp Nin, lrs_mp Din);

#ifdef __cplusplus
}
#endif

#endif // LIBLRSNASH