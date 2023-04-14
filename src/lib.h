#include "lrsdriver.h"
#include "lrslib.h"
#include "lrsnashlib.h"

#ifndef LIBLRSNASH
#define LIBLRSNASH

#ifdef __cplusplus
extern "C"
{
#endif

    extern void solve(int rows, int cols, int *row, int *col);

    extern int lrs_solve_nash_(game * g);

    extern long nash2_main_(lrs_dic * P1, lrs_dat * Q1, lrs_dic * P2orig,
                lrs_dat * Q2, long *numequilib, lrs_mp_vector output, long linindex[]);

#ifdef __cplusplus
}
#endif

#endif // LIBLRSNASH