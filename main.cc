#include "src/lib.h"

int solve_game(int rows, int cols)
{
	game Game;
	game *g = &Game;
	gInfo GI;
	g->aux = &GI;
	// g->nstrats[ROW] = rows;
	// g->nstrats[COL] = cols;
	// initFwidth(g);

	// lrs_solve_nash(g);
	return 0;
}

int main()
{
	solve_game(0, 0);
	return 0;
}