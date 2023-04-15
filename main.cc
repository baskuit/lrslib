#include "src/lib.h"

#include <iostream>

void foo (int rows, int cols, int *row_num, int *row_den, int *col_num, int *col_den) {
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
}

int main()
{

	const int rows = 2;
	const int cols = 2;

	int row_num1[4] = {1, 0, 0, 1};
	int row_den1[4] = {2, 1, 1, 1};
	int col_num1[4] = {1, 1, 1, 0};
	int col_den1[4] = {1, 1, 1, 1};

	foo(rows, cols, row_num1, row_den1, col_num1, col_den1);

	int row_num2[4] = {1, 0, 0, 1};
	int row_den2[4] = {1, 1, 1, 1};
	int col_num2[4] = {0, 1, 1, 0};
	int col_den2[4] = {1, 1, 1, 1};

	foo(rows, cols, row_num2, row_den2, col_num2, col_den2);

	int row_num3[4] = {1, 0, 0, 1};
	int row_den3[4] = {2, 1, 1, 1};
	int col_num3[4] = {1, 1, 1, 0};
	int col_den3[4] = {2, 1, 1, 1};

	foo(rows, cols, row_num3, row_den3, col_num3, col_den3);

	return 0;
}