#include "src/lib.h"openIO

char *Outfile = NULL;

int openIO(void) {
  if (!lrs_init("*lrsnash:"))
    return FALSE;
  fprintf(stderr, "\n");
	if(Outfile != NULL) {
	  if ((lrs_ofp = fopen(Outfile, "w")) == NULL) {
	    fprintf(stderr, "\nBad output file name\n");
	    return FALSE;
	  }
	}
	return TRUE;
}

int solve_game(int rows, int cols)
{
	game Game;
	rows = 2; cols = 2;
	Game.nstrats[0] = rows;
	Game.nstrats[1] = cols;

	// SUCKER
	Game.payoff[0][0][0].num = 1; Game.payoff[0][0][0].den = 2;
	Game.payoff[0][1][0].num = 1; Game.payoff[0][1][0].den = 1;
	Game.payoff[1][0][0].num = 1; Game.payoff[1][0][0].den = 1;
	Game.payoff[1][1][0].num = 0; Game.payoff[1][1][0].den = 1;

	Game.payoff[0][0][1].num = 1; Game.payoff[0][0][1].den = 2;
	Game.payoff[0][1][1].num = 0; Game.payoff[0][1][1].den = 1;
	Game.payoff[1][0][1].num = 0; Game.payoff[1][0][1].den = 1;
	Game.payoff[1][1][1].num = 1; Game.payoff[1][1][1].den = 1;

	// printGame(&Game);
	lrs_solve_nash(&Game);

	return 0;
}

int main()
{
	int open = openIO();
	solve_game(0, 0);
	return 0;
}