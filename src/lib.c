#include <stdio.h>
#include <string.h>
#include "lrsdriver.h"
#include "lrslib.h"
#include "solve.h"
#include "../include/lib.h"

void solve_gmp(
    int rows, int cols,
    mpq_t *row_payoff_data, mpq_t *col_payoff_data,
    mpz_t *row_solution_data, mpz_t *col_solution_data)
{
    lrs_init("");
    game Game;
    Game.nstrats[ROW] = rows;
    Game.nstrats[COL] = cols;
    Game.row_payoff = row_payoff_data;
    Game.col_payoff = col_payoff_data;

    solve(&Game, row_solution_data, col_solution_data);
}