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

    extern void solve(game *g, long long *row_data, long long *col_data);

    extern int lrs_solve_nash_(game *g, long long *row_data, long long *col_data);

    extern void prat_(const char *name, lrs_mp Nin, lrs_mp Din);

#ifdef __cplusplus
}
#endif

#endif // LIBLRSNASH