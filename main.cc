#include "src/lib.h"

#include <iostream>

int main()
{

	const int rows = 2;
	const int cols = 2;

	int row_num[4] = {1, 0, 0, 1};
	int row_den[4] = {2, 1, 1, 1};
	int col_num[4] = {1, 1, 1, 0};
	int col_den[4] = {1, 1, 1, 1};

	game Game;

	init_game(&Game,
		rows, cols, 
		row_num, row_den,
		col_num, col_den);

	int row_strategy[rows + 2];
	int col_strategy[cols + 2];

	solve(&Game, row_strategy, col_strategy);

	for (int i = 0; i < rows + 2; ++i) {
		std::cout << row_strategy[i] << ", ";
	}
	std::cout << std::endl;

	for (int i = 0; i < cols + 2; ++i) {
		std::cout << col_strategy[i] << ", ";
	}
	std::cout << std::endl;

	return 0;
}