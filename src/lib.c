#include "lib.h"

void solve(int rows, int cols, int *row, int *col)
{
    lrs_init("*lrsnash:");
    game Game;
    Game.nstrats[0] = rows;
    Game.nstrats[1] = cols;
    int flat_idx = 0;
    for (int row_idx = 0; row_idx < rows; ++row_idx)
    {
        for (int col_idx = 0; col_idx < cols; ++col_idx)
        {
            Game.payoff[row_idx][col_idx][0].num = row[flat_idx];
            Game.payoff[row_idx][col_idx][0].den = 1;
            Game.payoff[row_idx][col_idx][1].num = col[flat_idx];
            Game.payoff[row_idx][col_idx][1].den = 1;
            ++flat_idx;
        }
    }
    lrs_solve_nash(&Game);
}