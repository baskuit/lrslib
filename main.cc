#include "src/lib.h"

int main()
{

	int rows = 2;
	int cols = 2;

	int row_num[4] = {1, 0, 0, 1};
	int row_den[4] = {1, 1, 1, 1};
	int col_num[4] = {0, 1, 1, 0};
	int col_den[4] = {1, 1, 1, 1};

	solve(rows, cols, 
		row_num, row_den,
		col_num, col_den);
	return 0;
}