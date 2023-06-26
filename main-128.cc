#include "include/lib-128.h"

#include <iostream>

int main()
{

	const int rows = 2;
	const int cols = 2;

	int row_num[4] = {1, 0, 0, 1};
	int row_den[4] = {2, 1, 1, 1};
	int col_num[4] = {1, 1, 1, 0};
	int col_den[4] = {2, 1, 1, 1};

	game Game;

	init_game(&Game,
		rows, cols, 
		row_num, row_den,
		col_num, col_den);

	lrs_mp_vector row_data = alloc_data(rows + 2);
	lrs_mp_vector col_data = alloc_data(cols + 2);

	solve(&Game, row_data, col_data);

    std::cout << "row_strategy: " << std::endl;
    for (int row_idx = 0; row_idx < rows + 2; ++row_idx) {
        std::cout << (int)*row_data[row_idx] << ' ';
    }
    std::cout << std::endl;

    std::cout << "col_strategy: " << std::endl;
    for (int col_idx = 0; col_idx < cols + 2; ++col_idx) {
        std::cout << (int)*col_data[col_idx] << ' ';
    }
    std::cout << std::endl;

	dealloc_data(row_data, rows + 2);
	dealloc_data(col_data, cols + 2);

	return 0;
}